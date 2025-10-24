/**
* Assignment 3: CPU Scheduler
 * @file scheduler_fcfs.h
 * @author Elias Estacion and Meliton Rojas
 * @brief This Scheduler class implements the SJF scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#ifndef ASSIGN3_SCHEDULER_SJF_H
#define ASSIGN3_SCHEDULER_SJF_H

#include "scheduler.h"

class SchedulerSJF : public Scheduler {
private:
    std::vector<PCB> tasks_;

    // Per-process accounting used for output.
    struct Stat {
        std::string name; // process name
        int burst = 0;    // CPU burst time
        int waiting = 0;  // total time spent waiting in ready queue
        int turnaround = 0; // completion time (since arrival is 0)
    };
    std::vector<Stat> stats_;

    // Indicates whether simulate() has been executed
    bool simulated_ = false;

public:
    /**
     * @brief Construct a new SchedulerSJF object
     */
    SchedulerSJF();
    /**
     * @brief Destroy the SchedulerSJF object
     */
    ~SchedulerSJF() override;
    /**
     * @brief This function is called once before the simulation starts.
     *        It is used to initialize the scheduler.
     * @param process_list The list of processes in the simulation.
     */
    void init(std::vector<PCB>& process_list) override;
    /**
     * @brief This function is called once after the simulation ends.
     *        It is used to print out the results of the simulation.
     */
    void print_results() override;

    /**
     * @brief This function simulates the scheduling of processes in the ready queue.
     *        It stops when all processes are finished.
     */
    void simulate() override;
};
#endif //ASSIGN3_SCHEDULER_SJF_H
