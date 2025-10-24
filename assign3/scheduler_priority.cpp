/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief This Scheduler class implements the Priority scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code


#include "scheduler_priority.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>


// Constructor / Destructor
/**
 * @brief Default constructor for Priority scheduler.
 */
SchedulerPriority::SchedulerPriority() = default;

/**
 * @brief Default destructor for Priority scheduler.
 */
SchedulerPriority::~SchedulerPriority() = default;


/**
 * @brief Initialize with the list of processes (arrival order = file order).
 * @param process_list Vector of PCBs and assumes arrival at t=0 in file order.
 */
void SchedulerPriority::init(std::vector<PCB>& process_list) {
    tasks_.assign(process_list.begin(), process_list.end());
    stats_.clear();
    simulated_ = false;
}

/**
 * @brief Simulates Priority scheduling
 * Higher priority value runs earlier. In case of ties, it keeps the original order.
 * 
 */
void SchedulerPriority::simulate() {
    const size_t n = tasks_.size();

    stats_.clear();
    stats_.resize(n);
    for (size_t i = 0; i < n; ++i) {
        stats_[i].name = tasks_[i].name;
        stats_[i].burst = static_cast<int>(tasks_[i].burst_time);
        stats_[i].priority = static_cast<int>(tasks_[i].priority);
    }

    // Sorts indices by burst time
    std::vector<size_t> idx(n);
    std::iota(idx.begin(), idx.end(), 0);
    std::stable_sort(idx.begin(), idx.end(),
        [&](size_t a, size_t b) {
            if (tasks_[a].priority != tasks_[b].priority)
                return tasks_[a].priority > tasks_[b].priority;
            return false;
        });

    int time = 0;
    for (size_t k : idx) {
        auto &s = stats_[k];
    
        // Shows the priority scheduling execution
        std::cout << "Running Process " << s.name
                  << " for " << s.burst << " time units" << endl;

        s.waiting    = time;      // waited while earlier (higher-priority) jobs ran
        time        += s.burst;   // run to completion (non-preemptive)
        s.turnaround = time;      // completion time (arrival = 0)
    }

    simulated_ = true;
}

/**
 * @brief Prints per-process turnaround/waiting times and their averages.
 */
void SchedulerPriority::print_results() {
    if (!simulated_) {
        std::cerr << "Error: simulate() has not been run.\n";
        return;
    }

    double sum_tat = 0.0;
    double sum_wt  = 0.0;

    for (const auto& s : stats_) {
        std::cout << s.name
                  << " turn-around time = " << s.turnaround
                  << ", waiting time = "   << s.waiting
                  << "\n";
        sum_tat += s.turnaround;
        sum_wt  += s.waiting;
    }

    const int n = static_cast<int>(stats_.size());
    double avg_tat  = (n ? sum_tat / n : 0.0);
    double avg_wt   = (n ? sum_wt / n : 0.0);

    std::cout << std::fixed << std::setprecision(3)
              << "Average turn-around time = " << avg_tat
              << ", Average waiting time = "   << avg_wt << "\n";
}