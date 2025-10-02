
/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author ??? (TODO: your name)
 * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @version 0.1
 */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#define MAX_LINE 80 // The maximum length command

/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command
 * @param args
 * @return int
 */
int parse_command(char command[], char *args[])
{
    // TODO: implement this function
    // Remove trailing newline, if any
    size_t n = strlen(command);
    if (n > 0 && command[n - 1] == '\n') command[n - 1] = '\0';

    int argc = 0;
    char *tok = strtok(command, " \t");
    while (tok != nullptr && argc < (MAX_LINE / 2)) {
        args[argc++] = tok;
        tok = strtok(nullptr, " \t");
    }
    args[argc] = nullptr;
    return argc;
}

// TODO: Add additional functions if you need

/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    char command[MAX_LINE];       // the command that was entered
    char *args[MAX_LINE / 2 + 1]; // hold parsed out command line arguments
    int should_run = 1;           /* flag to determine when to exit program */

    // TODO: Add additional variables for the implementation.
    // Additional variables for the implementation.
    char* argv_exec[MAX_LINE / 2 + 1];      // cleaned argv for exec

    // Simple history buffer: only the most recent full command line
    char last_cmd[MAX_LINE] = { 0 };
    bool has_history = false;

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);
        // Read the input command
        fgets(command, MAX_LINE, stdin);
        // Parse the input command
        if (fgets(command, MAX_LINE, stdin) == nullptr) {
            printf("\n");
            break;
        }

        // TODO: Add your code for the implementation
        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) parent will invoke wait() unless command included &
         */

         if (num_args == 0) continue;

        // Built-in: exit
        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }

        // History (!!)
        if (strcmp(args[0], "!!") == 0 && num_args == 1) {
            if (!has_history) {
                fprintf(stderr, "No commands in history.\n");
                continue;
            }
            // Echo the most recent command and re-parse it
            printf("%s\n", last_cmd);
            char replay_buf[MAX_LINE];
            strncpy(replay_buf, last_cmd, sizeof(replay_buf) - 1);
            replay_buf[sizeof(replay_buf) - 1] = '\0';
            num_args = parse_command(replay_buf, args);
            if (num_args <= 0) continue;
            // last_cmd already is the most recent; repeating keeps it as "next" too
        } else {
            // Save current command (rebuilt from tokens) as history
            last_cmd[0] = '\0';
            for (int i = 0; i < num_args; ++i) {
                if (i) strncat(last_cmd, " ", sizeof(last_cmd) - strlen(last_cmd) - 1);
                strncat(last_cmd, args[i], sizeof(last_cmd) - strlen(last_cmd) - 1);
            }
            has_history = true;
        }

        // Build argv, detect &, redirection, and a single pipe
        int run_bg = 0;
        const char* infile = nullptr;
        const char* outfile = nullptr;
        int redir_mode = 0; // 0 none, 1 '<', 2 '>'
        int pipe_pos = -1;

        int argc_clean = build_argv(args, num_args, argv_exec,
                                    run_bg, infile, outfile, redir_mode, pipe_pos);
        if (argc_clean < 0) {
            fprintf(stderr, "syntax error\n");
            continue;
        }

        // Handle pipeline (no redirection allowed with a pipe per spec)
        if (pipe_pos != -1) {
            if (redir_mode != 0) {
                fprintf(stderr, "Redirection with pipe not supported\n");
                continue;
            }
            if (pipe_pos == 0 || pipe_pos >= argc_clean) {
                fprintf(stderr, "syntax error near '|'\n");
                continue;
            }

            // Split argv_exec into left and right
            char* left_argv[MAX_LINE / 2 + 1];
            for (int i = 0; i < pipe_pos; ++i) left_argv[i] = argv_exec[i];
            left_argv[pipe_pos] = nullptr;

            char* right_argv[MAX_LINE / 2 + 1];
            int rlen = 0;
            for (int i = pipe_pos; i < argc_clean; ++i) right_argv[rlen++] = argv_exec[i];
            right_argv[rlen] = nullptr;

            int fds[2];
            if (pipe(fds) < 0) {
                perror("pipe");
                continue;
            }

            pid_t c1 = fork();
            if (c1 < 0) {
                perror("fork");
                close(fds[0]); close(fds[1]);
                continue;
            }
            if (c1 == 0) {
                // Left child: stdout -> pipe write
                if (dup2(fds[1], STDOUT_FILENO) < 0) { perror("dup2"); _exit(1); }
                close(fds[0]); close(fds[1]);
                execvp(left_argv[0], left_argv);
                print_cmd_not_found();
                _exit(127);
            }

            pid_t c2 = fork();
            if (c2 < 0) {
                perror("fork");
                close(fds[0]); close(fds[1]);
                int st; waitpid(c1, &st, 0);
                continue;
            }
            if (c2 == 0) {
                // Right child: stdin <- pipe read
                if (dup2(fds[0], STDIN_FILENO) < 0) { perror("dup2"); _exit(1); }
                close(fds[0]); close(fds[1]);
                execvp(right_argv[0], right_argv);
                print_cmd_not_found();
                _exit(127);
            }

            // Parent
            close(fds[0]); close(fds[1]);
            if (!run_bg) {
                int st;
                while (waitpid(c1, &st, 0) == -1 && errno == EINTR) {}
                while (waitpid(c2, &st, 0) == -1 && errno == EINTR) {}
            }
            continue;
        }

        // No pipe: handle (at most one) redirection and background
        if (argc_clean == 0) continue;

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }
        if (pid == 0) {
            // Child: apply redirection (if any), then exec
            if (apply_redirection(infile, outfile, redir_mode) != 0) _exit(1);
            execvp(argv_exec[0], argv_exec);
            print_cmd_not_found();
            _exit(127);
        } else {
            if (!run_bg) {
                int status = 0;
                while (waitpid(pid, &status, 0) == -1 && errno == EINTR) {}
            }
        }
    

    }
    return 0;
}
