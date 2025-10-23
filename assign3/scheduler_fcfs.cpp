/**
* Assignment 3: CPU Scheduler
 * @file scheduler_fcfs.cpp
 * @author ??? (TODO: your name)
 * @brief This Scheduler class implements the FCSF scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_fcfs.h"

// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here
#include <iostream>
#include <iomanip>

// constructor / destructor

SchedulerFCFS::SchedulerFCFS() = default;

SchedulerFCFS::~SchedulerFCFS() = default;

// lifecycle 

void SchedulerFCFS::init(std::vector<PCB>& process_list) {
    // Preserve the input (file) order as the arrival order.
    // All processes are assumed to arrive at time 0.
    tasks_.assign(process_list.begin(), process_list.end());
    stats_.clear();
    simulated_ = false;
}

void SchedulerFCFS::simulate() {
    // Non-preemptive FCFS:
    //   - waiting time of a process = total CPU time of all earlier processes
    //   - turnaround time           = completion time - arrival (arrival = 0)
    stats_.clear();
    stats_.reserve(tasks_.size());

    int time = 0; // simulated clock
    for (const auto& p : tasks_) {
        Stat s;
        s.name = p.name;
        s.burst = static_cast<int>(p.burst_time);

        s.waiting = time;     // waited while earlier tasks ran
        time += s.burst;      // run to completion
        s.turnaround = time;  // completion - arrival(0)

        stats_.push_back(s);
    }

    simulated_ = true;
}

void SchedulerFCFS::print_results() {
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