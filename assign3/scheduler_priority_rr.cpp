/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority_rr.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief This Scheduler class implements the Priority RR scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_priority_rr.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include <iomanip>

/**
 * @brief Default constructor for a priority based Round-Robin scheduler.
 * @param time_quantum The quantum size (>=1) used within each priority level.
 */
SchedulerPriorityRR::SchedulerPriorityRR(int time_quantum)
    : quantum_(time_quantum > 0 ? time_quantum : 1) {}

/**
 * @brief Default destructor for priority based Round-Robin scheduler
 */
SchedulerPriorityRR::~SchedulerPriorityRR() = default;

/**
 * @brief Initialize the scheduler with process in input (file) order
 * @param process_list Vector of PCBs assuming arrival of all processes at t=0.
 */
void SchedulerPriorityRR::init(std::vector<PCB>& process_list) {
    tasks_.assign(process_list.begin(), process_list.end());
    stats_.clear();
    simulated_ = false;
}

/**
* @brief Simulate priority within Round Robin: makes sure that higher priorities run first;
* within the same priority, apply Round Robin using the quantum that has been configured.
* @details This preserves the input order among equal priorities.
*/
void SchedulerPriorityRR::simulate() {
    // Build per-process accounting table.
    stats_.clear();
    stats_.reserve(tasks_.size());
    for (size_t i = 0; i < tasks_.size(); ++i) {
        const PCB& p = tasks_[i];
        Stat s;
        s.name         = p.name;
        s.priority     = static_cast<int>(p.priority);
        s.burst        = static_cast<int>(p.burst_time);
        s.remaining    = s.burst;
        s.waiting      = 0;
        s.turnaround   = 0;
        s.last_finish  = 0;         
        s.original_idx = i;         
        stats_.push_back(s);
    }

    // Collect unique priority levels and sort DESC (higher first).
    std::vector<int> levels;
    levels.reserve(stats_.size());
    for (const auto& s : stats_) levels.push_back(s.priority);
    std::sort(levels.begin(), levels.end());
    levels.erase(std::unique(levels.begin(), levels.end()), levels.end());
    std::sort(levels.begin(), levels.end(), std::greater<int>());

    int time = 0;

    // For each priority level, run RR among its tasks until they finish.
    for (int pr : levels) {
        // Get indices of tasks with this priority in original file order.
        std::vector<size_t> idxs;
        idxs.reserve(stats_.size());
        for (size_t i = 0; i < stats_.size(); ++i)
            if (stats_[i].priority == pr) idxs.push_back(i);

        std::sort(idxs.begin(), idxs.end(),
                  [&](size_t a, size_t b){
                      return stats_[a].original_idx < stats_[b].original_idx;
                  });

        std::deque<size_t> rq(idxs.begin(), idxs.end());

        while (!rq.empty()) {
            size_t j = rq.front(); rq.pop_front();
            auto& ps = stats_[j];

            if (ps.remaining == 0) continue; // should not happen, but safe

            // Waiting accumulated since last time it left CPU (or since t=0).
            ps.waiting += (time - ps.last_finish);

            // Shows the priority based Round Robin execution
            std::cout << "Running Process " << ps.name
                      << " for " << ((ps.remaining < quantum_) ? ps.remaining : quantum_)
                      << " time units" << endl;

            // Run for a quantum or until completion.
            int slice = (ps.remaining < quantum_) ? ps.remaining : quantum_;
            time += slice;
            ps.remaining -= slice;

            if (ps.remaining == 0) {
                ps.turnaround = time;        // completion - arrival(0)
            } else {
                ps.last_finish = time;       // preempted now
                rq.push_back(j);             // cycle to the back
            }
        }
    }

    simulated_ = true;
}

/**
 * @brief Print per-task results and averages for priority based Round Robin sched
 * 
 */
void SchedulerPriorityRR::print_results() {
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