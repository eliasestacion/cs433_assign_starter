/**
* Assignment 3: CPU Scheduler
 * @file scheduler_fcfs.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief This Scheduler class implements the FCFS scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_fcfs.h"
#include <iostream>
#include <iomanip>

/**
 * @brief Default constructor for FCFS scheduler.
 * 
 */
SchedulerFCFS::SchedulerFCFS() = default;

/**
 * @brief Default destructor for FCFS scheduler.
 */
SchedulerFCFS::~SchedulerFCFS() = default;

/**
 * @brief To initialize the scheduler with the given process list.
 * @param process_list The Vector of PCBs read from input. All processes are assumed to arrive at time 0.
 * 
 */
void SchedulerFCFS::init(std::vector<PCB>& process_list) {
    tasks_.assign(process_list.begin(), process_list.end());
    stats_.clear();
    simulated_ = false;
}

/**
 * @brief To run the non-preemptive FCFS simulation until all the tasks finish.
 * @details The waiting time for current task equals the sum of bursts of all the tasks before it.
 */
void SchedulerFCFS::simulate() {
    stats_.clear();
    stats_.reserve(tasks_.size());

    int time = 0; // simulated clock
    for (const auto& p : tasks_) {
        Stat s;
        s.name = p.name;
        s.burst = static_cast<int>(p.burst_time);

        // Shows the FCFS execution
        std:: cout << "Running Process " << s.name
                   << " for " << s.burst << " time units" << endl;

        s.waiting = time;     // waited while earlier tasks ran
        time += s.burst;      // run to completion
        s.turnaround = time;  // completion - arrival(0)

        stats_.push_back(s);
    }

    simulated_ = true;
}

/**
 * @brief To print each task's waiting time and turnaround time, as well as the averages.
 * 
 */
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