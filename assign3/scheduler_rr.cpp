/**
* Assignment 3: CPU Scheduler
 * @file scheduler_rr.h
 * @author Elias Estacion and Meliton Rojas
 * @brief This Scheduler class implements the RoundRobin (RR) scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
//

#include "scheduler_rr.h"
#include <queue>
#include <iostream>
#include <iomanip>

// constructor destructor
/**
 * @brief Default constructor for a Round Robin scheduler.
 * @param time_quantum Quantum size in time units
 */
SchedulerRR::SchedulerRR(int time_quantum)
    : quantum_(time_quantum > 0 ? time_quantum : 1) {}

/**
 * @brief Default constructor for a Round Robin scheduler.
 */
SchedulerRR::~SchedulerRR() = default;

/**
 * @brief Initialize the Round Robin scheduler with the process list.
 * @param process_list Vector of PCBs and assumes all arrive at t=0 in file order.
 */
void SchedulerRR::init(std::vector<PCB>& process_list) {
    tasks_.assign(process_list.begin(), process_list.end());
    stats_.clear();
    simulated_ = false;
}

/**
 * @brief Simulate preemptive Roun Robin with a fixed quantum until all tasks complete.
 * @details It also uses a ready queue of indices. Waiting time accumulates between dispatches.
 */
void SchedulerRR::simulate() {
    stats_.clear();
    stats_.reserve(tasks_.size());

    std::queue<int> rq; 

    // Initialize per-process stats and enqueue in input order.
    for (size_t i = 0; i < tasks_.size(); ++i) {
        Stat s;
        s.name       = tasks_[i].name;
        s.burst      = static_cast<int>(tasks_[i].burst_time);
        s.remaining  = s.burst;
        s.waiting    = 0;
        s.turnaround = 0;
        s.last_finish = 0;
        stats_.push_back(s);
        rq.push(static_cast<int>(i));
    }

    int time = 0;

    while (!rq.empty()) {
        int idx = rq.front(); rq.pop();
        auto& ps = stats_[idx];

        // Accumulate waiting since last time it left the CPU (or since t=0 initially).
        ps.waiting += (time - ps.last_finish);

        // Run for one quantum or until completion, whichever is smaller.
        int slice = (ps.remaining < quantum_) ? ps.remaining : quantum_;

        // Shows the Round Robin execution
        std::cout << "Running Process " << ps.name
                  << " for " << slice << " time units" << endl;
                  
        time += slice;
        ps.remaining -= slice;

        if (ps.remaining == 0) {
            // Finished now
            ps.turnaround = time; // completion - arrival(0)
        } else {
            // Preempt and requeue; note when it left CPU
            ps.last_finish = time;
            rq.push(idx);
        }
    }

    simulated_ = true;
}

/**
 * @brief Prints the task results and averages for the RR scheduler.
 */
void SchedulerRR::print_results() {
    if (!simulated_) {
        std::cerr << "Error: simulate() has not been run.\n";
        return;
    }

    double sum_tat = 0.0, sum_wt = 0.0;

    for (const auto& s : stats_) {
        std::cout << s.name
                  << " turn-around time = " << s.turnaround
                  << ", waiting time = "   << s.waiting << "\n";
        sum_tat += s.turnaround;
        sum_wt  += s.waiting;
    }

    const int n = static_cast<int>(stats_.size());
    std::cout << std::fixed << std::setprecision(3)
              << "Average turn-around time = " << (n ? sum_tat / n : 0.0)
              << ", Average waiting time = "   << (n ? sum_wt  / n : 0.0)
              << "\n";
}