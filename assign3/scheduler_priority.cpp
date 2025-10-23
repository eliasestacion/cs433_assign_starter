/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority.cpp
 * @author ??? (TODO: your name)
 * @brief This Scheduler class implements the Priority scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code


#include "scheduler_priority.h"

// TODO: add implementation of SchedulerPriority constructor, destrcutor and 
// member functions init, print_results, and simulate here

#include <algorithm>
#include <iostream>
#include <iomanip>


// Constructor / Destructor

SchedulerPriority::SchedulerPriority() = default;
SchedulerPriority::~SchedulerPriority() = default;


// Initialize with the list of processes (arrival order = file order).
// All processes are assumed to arrive at time 0.

void SchedulerPriority::init(std::vector<PCB>& process_list) {
    tasks_.assign(process_list.begin(), process_list.end());
    stats_.clear();
    simulated_ = false;
}


// Simulate non-preemptive Priority scheduling.
// Higher priority value runs earlier. Ties keep original order (stable).
// Turnaround = completion time (since arrival = 0). Waiting = time before start.

void SchedulerPriority::simulate() {
    // Build the execution order by descending priority.
    std::vector<PCB> order = tasks_;
    std::stable_sort(order.begin(), order.end(),
        [](const PCB& a, const PCB& b) {
            if (a.priority != b.priority) return a.priority > b.priority; // higher first
            return false; // preserve original order on ties
        });

    stats_.clear();
    stats_.reserve(order.size());

    int time = 0; // simulated clock
    for (const auto& p : order) {
        Stat s;
        s.name     = p.name;
        s.priority = static_cast<int>(p.priority);
        s.burst    = static_cast<int>(p.burst_time);

        s.waiting    = time;      // waited while earlier (higher-priority) jobs ran
        time        += s.burst;   // run to completion (non-preemptive)
        s.turnaround = time;      // completion time (arrival = 0)

        stats_.push_back(s);
    }

    simulated_ = true;
}


// Print per-process turnaround / waiting times and their averages.

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
    std::cout << std::fixed << std::setprecision(3)
              << "Average turn-around time = " << (n ? sum_tat / n : 0.0)
              << ", Average waiting time = "   << (n ? sum_wt  / n : 0.0)
              << "\n";
}