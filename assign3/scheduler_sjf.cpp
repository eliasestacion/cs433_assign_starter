/**
* Assignment 3: CPU Scheduler
 * @file scheduler_sjf.cpp
 * @author ??? (TODO: your name)
 * @brief This Scheduler class implements the SJF scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_sjf.h"

// TODO: add implementation of SchedulerSJF constructor, destrcutor and 
// member functions init, print_results, and simulate here
#include <algorithm>
#include <iostream>
#include <iomanip>


// Constructor & Destructor

SchedulerSJF::SchedulerSJF() = default;

SchedulerSJF::~SchedulerSJF() = default;

// Initialize scheduler with process list

void SchedulerSJF::init(std::vector<PCB>& process_list) {
    // Store the list of processes (all arrive at time 0)
    tasks_.assign(process_list.begin(), process_list.end());
    stats_.clear();
    simulated_ = false;
}

// Simulate SJF scheduling

void SchedulerSJF::simulate() {
    // Shortest Job First (non-preemptive):
    // 1. Sort processes by burst time.
    // 2. For ties, keep original order (stable sort).
    std::vector<PCB> order = tasks_;
    std::stable_sort(order.begin(), order.end(),
        [](const PCB& a, const PCB& b) {
            return a.burst_time < b.burst_time;
        });

    stats_.clear();
    stats_.reserve(order.size());

    int current_time = 0;

    for (const auto& p : order) {
        Stat s;
        s.name = p.name;
        s.burst = static_cast<int>(p.burst_time);

        // Waiting time is how long this process waited before running
        s.waiting = current_time;

        // Run this process (non-preemptive)
        current_time += s.burst;

        // Turnaround time = waiting + burst (since arrival = 0)
        s.turnaround = current_time;

        stats_.push_back(s);
    }

    simulated_ = true;
}

// Print results

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
    std::cout << std::fixed << std::setprecision(3)
              << "Average turn-around time = " << (n ? total_turnaround / n : 0.0)
              << ", Average waiting time = "   << (n ? total_waiting / n : 0.0)
              << "\n";
}