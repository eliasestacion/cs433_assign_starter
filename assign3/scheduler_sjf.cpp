/**
* Assignment 3: CPU Scheduler
 * @file scheduler_sjf.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief This Scheduler class implements the SJF scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_sjf.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>


/**
 * @brief Default constructor for the SJF scheduler.
 */
SchedulerSJF::SchedulerSJF() = default;

/**
 * @brief Default destructor for the SJF scheduler.
 */
SchedulerSJF::~SchedulerSJF() = default;

/**
 * @brief Initialize SJF with the process list in the order of arrival.
 * @param process_list Vector of PCBs and assumes arrival t=0 for all.
 */
void SchedulerSJF::init(std::vector<PCB>& process_list) {
    // Store the list of processes (all arrive at time 0)
    tasks_.assign(process_list.begin(), process_list.end());
    stats_.clear();
    simulated_ = false;
}

/**
 * @brief Simulate non-preemptive Shortest Job First scheduling
 * @details Sorted by burst time so ties keep input order.
 */
void SchedulerSJF::simulate() {
    const size_t n = tasks_.size();

    stats_.clear();
    stats_.resize(n);
    for (size_t i = 0; i < n; ++i) {
        stats_[i].name = tasks_[i].name;
        stats_[i].burst = static_cast<int>(tasks_[i].burst_time);
    }

    // Sorts indices by burst time
    std::vector<size_t> idx(n);
    std::iota(idx.begin(), idx.end(), 0);
    std::stable_sort(idx.begin(), idx.end(),
        [&](size_t a, size_t b) {
            return tasks_[a].burst_time < tasks_[b].burst_time;
        });

    int current_time = 0;

    for (size_t k : idx) {
        auto &s = stats_[k];

        // Shows the SJF execution
        std::cout << "Running Process " <<s.name  
                  << " for " << s.burst << " time units" << endl;

        // Waiting time is how long this process waited before running
        s.waiting = current_time;

        // Run this process (non-preemptive)
        current_time += s.burst;

        // Turnaround time = waiting + burst (since arrival = 0)
        s.turnaround = current_time;
    }

    simulated_ = true;

}

/**
 * @brief Prints the tasks turnaround/waiting time and their averages.
 */
void SchedulerSJF::print_results() {
    if (!simulated_) {
        std::cerr << "Error: simulate() has not been run.\n";
        return;
    }

    double total_turnaround = 0.0;
    double total_waiting = 0.0;

    for (const auto& s : stats_) {
        std::cout << s.name
                  << " turn-around time = " << s.turnaround
                  << ", waiting time = "   << s.waiting << "\n";

        total_turnaround += s.turnaround;
        total_waiting += s.waiting;
    }

    int n = static_cast<int>(stats_.size());
    double avg_tat = (n ? total_turnaround / n : 0.0);
    double avg_wt  = (n ? total_waiting / n : 0.0);

    std::cout << std::fixed << std::setprecision(3)
              << "Average turn-around time = " << avg_tat
              << ", Average waiting time = "   << avg_wt << "\n";
}