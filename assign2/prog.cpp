
/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author Elias Estacion and Meliton Rojas
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
#include <sys/wait.h>
#include <errno.h>

using namespace std;

#define MAX_LINE 80 // The maximum length command

/**
 * @brief Parse out the command and arguments from the input command separated by spaces.
 *
 * @param command
 *       This function mutates the buffer by removing the trailing newline (if there is one) and inserts '\0'
 *       terminators between tokens using strtok().
 * @param args
 *        This is the output array of C-string pointers. The array is null-terminated
 * 
 * @return int
 *        return the number of tokens found. It returns 0 if the line is empty.
 */
int parse_command(char command[], char *args[])
{
    // Remove trailing newline, if any
    size_t n = strlen(command);
    if (n > 0 && command[n - 1] == '\n') command[n - 1] = '\0';

    int argc = 0;
    
    // Split on spaces/tabs; strtok skips consecutive delimiters
    char *tok = strtok(command, " \t");
    while (tok != nullptr && argc < (MAX_LINE / 2)) {
        args[argc++] = tok;
        tok = strtok(nullptr, " \t");
    }
    args[argc] = nullptr;
    return argc;
}

/** 
 * @brief Helper to print a conistent error message when execv() fails to find a command.
 * 
 * @note Keeps the user-facing error messaging in one place.
 */
static void print_cmd_not_found() {
    fprintf(stderr, "command not found \n");
}

/**
 * @brief This remove a contiguous slice (pos, pos+count) from an argv-like array.
 * 
 * @param args  This is the null-terminated argv array to modify in-place.
 * @param argc  This is the current argument count.
 * @param pos   This is the start index of slice to remove.
 * 
 * @return int  New argc agter removal.
 */
static int argv_remove_range(char *args[], int argc, int pos, int count) {
    if (pos < 0 || count < 0 || pos + count > argc)
    return argc;

    for (int i = pos; i + count <= argc; ++i) {
        args[i] = args[i + count];
    }
    return argc - count;
}

/**
 * @brief The main function of a simple UNIX Shell. 
 *        - It supports foreground/background execution via '&'
 *        - A history feature with '!!' which replays the last command
 *        - A single inpiut or output redirection ('<' ir '>') on pipeline commands
 *        - A single pipeling 'l | r' implemented for extra credit
 * 
 * @param argc The number of arguments
 * @param argv The array of arguments
 * 
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    char command[MAX_LINE];       // the command that was entered
    char *args[MAX_LINE / 2 + 1]; // hold parsed out command line arguments
    int should_run = 1;           /* Main loop flag to determine when to exit program */

    // Simple history buffer: It stores the last full command line for '!!'
    char last_cmd[MAX_LINE] = { 0 };
    bool has_history = false;

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);

        // It reads one line of input
        if (fgets(command, MAX_LINE, stdin) == nullptr) {
            printf("\n");
            break;
        }
        
         // History feature: If user typed "!!", it replaces input with the last command
         if ((command[0] == '!' && command[1] == '!' && (command[2] == '\n' || command[2] == '\0'))) {
            if (!has_history) {
                fprintf(stderr, "No command history. \n");
                continue;
            }

            // Echoes the command being replayed for clarity
            printf("%s\n", last_cmd);
            strncpy(command, last_cmd, sizeof(command) - 1);
            command[sizeof(command) - 1] = '\0';
         }

         // Parse the input command
         int num_args = parse_command(command, args);
         if (num_args == 0) continue;

        // Built-in: exit
        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }

        // Keep a copy of the raw command line entered by the user so that it can be recalled later using the "!!" feature.
        {
            last_cmd[0] = '\0';
            for (int i = 0; i < num_args; ++i) {
                if (i) strncat(last_cmd, " ", sizeof(last_cmd) - strlen(last_cmd) - 1);
                strncat(last_cmd, args[i], sizeof(last_cmd) - strlen(last_cmd) - 1);
            }
            has_history = true;
        }

        // Detect Background Execution '&': Checks if the command should be executed in the background.
        // The '&' can appear as a separate argument or attached to the end of the last argument.
        int run_bg = 0;
        if (num_args > 0 && strcmp(args[num_args - 1], "&") == 0) {
            // First case: '&' appears as its own separate argument
            run_bg = 1;
            num_args -= 1;
            args[num_args] = nullptr;
            if (num_args == 0) {
                fprintf(stderr, "syntax error: '&' needs a command\n");
                continue;
            }
        }
        else if (num_args > 0) {
            // Second case: '&' is appended to the last token
            char *last = args[num_args - 1];
            size_t L = strlen(last);
            if (L > 0 && last[L -1] == '&') {
                run_bg = 1;
                last[L - 1] = '\0';
                if (last[0] == '\0') {
                    num_args -= 1;
                    args[num_args] = nullptr;
                    if (num_args == 0) {
                        fprintf(stderr, "syntax error: '&' needs a command \n");
                            continue;
                    }
                }
            }
        }

        // Pipe Detection: Detect if the command line contains a pipe ('|') and only a single pipe is supported.
        int pipe_pos = -1;
        for (int i = 0; i < num_args; ++i) {
            if (strcmp(args[i], "|") == 0) {
                if (pipe_pos != -1) {
                    fprintf(stderr, "Only one pipe supported.\n");
                    pipe_pos = -2;
                    break;
                }
                pipe_pos = i;
            }
        }
        if (pipe_pos == -2) continue;

        // Redirection: Detect one redirection (< or >), only if there's no pipe
        const char* infile = nullptr; // input redirection filename for '<'
        const char* outfile = nullptr; // output redirection filename for '>'
        int redir_mode = 0; // 0 = none, 1 = '<', 2 = '>'

        if (pipe_pos == -1) {
            // Scans args for one redirection operator. It removes the operator + file from argv.
            for (int i = 0; i < num_args; ++i) {
                if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0) {
                    if (redir_mode != 0) {
                        fprintf(stderr, "Only one redirection supported.\n");
                        redir_mode = -1;
                        break;
                    }
                    if (i + 1 >= num_args) {
                        fprintf(stderr, "syntax error: missing file after '%s'\n", args[i]);
                        redir_mode = -1;
                        break;
                    }
                    if (strcmp(args[i], "<") == 0) {
                        redir_mode = 1; infile = args[i + 1];
                    } else {
                        redir_mode = 2; outfile = args[i + 1];
                    }
                    // Removes the operator and filename from argv
                    num_args = argv_remove_range(args, num_args, i, 2);
                    i -= 1;
                }
            }
            if (redir_mode == -1) continue;
        } else {
            // Pipe present: reject any redirection (per assignment)
            for (int i = 0; i < num_args; ++i) {
                if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0) {
                    fprintf(stderr, "Redirection with pipe not supported.\n");
                    pipe_pos = -2;
                    break;
                }
            }
            if (pipe_pos == -2) continue;
        }

        // Single pipeline execution for extra credit: left | right. 
        // If a pipe operator is found in the command, the arguments are split into left and right commands
        // then connect them with a UNIX pipe do that stdout of left becomes stdin of right.
        if (pipe_pos != -1) {
            if (pipe_pos == 0 || pipe_pos == num_args - 1) {
                fprintf(stderr, "syntax error near '|'\n");
                continue;
            }

            // Seaprate command arguments into the left and right of the pipe symbol '|'
            char **left_argv  = args;                   // argv for left command
            char **right_argv = &args[pipe_pos + 1];    // argv for right command
            int left_argc  = pipe_pos;                  // count for the left side
            args[left_argc] = nullptr;                  // terminate left argv at '|'

            int fds[2];
            if (pipe(fds) < 0) { perror("pipe"); continue; }

            // First child (left command)
            pid_t c1 = fork();
            if (c1 < 0) { 
                perror("fork"); 
                close(fds[0]); close(fds[1]); 
                continue; 
            }
            if (c1 == 0) {

                // Left child: Redirect standard output to the write end of the pipe.
                if (dup2(fds[1], STDOUT_FILENO) < 0) { perror("dup2"); _exit(1); }
                close(fds[0]); close(fds[1]);
                execvp(left_argv[0], left_argv);
                print_cmd_not_found();
                _exit(127);
            }

            // Second child (right command)
            pid_t c2 = fork();
            if (c2 < 0) {
                perror("fork");
                close(fds[0]); close(fds[1]);
                int st; (void)waitpid(c1, &st, 0);
                continue;
            }
            if (c2 == 0) {
                // Right child: Redirect standard input to the read end of the pipe.
                if (dup2(fds[0], STDIN_FILENO) < 0) { perror("dup2"); _exit(1); }
                close(fds[0]); close(fds[1]);
                execvp(right_argv[0], right_argv);
                print_cmd_not_found();
                _exit(127);
            }

            // Parent process: The parent does not use pipe directly, so close both ends.
            close(fds[0]); close(fds[1]);

            // Waits for both children to finish if not running in background.
            if (!run_bg) {
                int st;
                while (waitpid(c1, &st, 0) == -1 && errno == EINTR) {}
                while (waitpid(c2, &st, 0) == -1 && errno == EINTR) {}
            }
            continue;
        }

        // If no pipeline is used, fork a single child process and exec the command.
        if (num_args == 0) continue;

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }
        if (pid == 0) {
            // Apply inpuy/output redirection before executing the command.
            if (redir_mode == 1 && infile) {
                // Redirect standard input from file: "< infile"
                close(STDIN_FILENO);
                if (open(infile, O_RDONLY) < 0) { perror("open <"); _exit(1); }
            } else if (redir_mode == 2 && outfile) {
                // Redirect standard output to file: "> outfile"
                close(STDOUT_FILENO);
                if (open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644) < 0) { perror("open >"); _exit(1); }
            }

            // Replaces process image with requested command
            execvp(args[0], args);
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