/**
* Assignment 5: Page replacement algorithms
 * @file replacement.cpp
 * @author Elias Estacion and Meliton Rojas 
 * @brief A base class for different page replacement algorithms.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
#include <iostream>
#include "replacement.h"

/**
 * @brief Constructor for the Replacement class.
 * @param num_pages Total number of logical pages for the simulation.
 * @param num_frames Total number of available free frames.
 * 
 * The member initializer list initializes the page_table with num_pages entries,
 * sets num_frames, free_frames, and next_free_frame accordingly, and initializes:
 * - num_frames to the total number of frames
 * - free_frames to the same value as num_frames (all frames are initially free)
 * - next_free_frame to 0 (the index of the next free frame to use)
 * - num_references, num_page_faults, and num_page_replacements to 0
 */
Replacement::Replacement(int num_pages, int num_frames)
    : page_table(num_pages), 
    num_frames(num_frames), 
    free_frames(num_frames),
    next_free_frame(0),
    num_references(0),
    num_page_faults(0),
    num_page_replacements(0)
{
}

/**
 * @brief Destructor for the Replacement class.
 */
Replacement::~Replacement()
{
}

/**
 * @brief Simulate a single page access.
 * @details If the page is valid, it calls the touch_page function.
 *         If the page is not valid but free frames are available, it calls the load_page function.
 *         If the page is not valid and there is no free frame, it calls the replace_page function.
 * @param page_num The logical page number.
 * @param is_write whether this access a memory write
 * @return whether it's a page fault: true if it's a page fault
 */
bool Replacement::access_page(int page_num, bool is_write)
{
    // If the page is valid, it calls the touch_page function. 
    ++num_references;

    PageEntry &entry = page_table[page_num];

    if (entry.valid) {
        // Page is already in physical memory
        if (is_write) {
            entry.dirty = true; // Mark the page as dirty on write access
        }
        touch_page(page_num);
        return false; 
    }

    ++num_page_faults;

    if (free_frames > 0) {
        load_page(page_num);
        --free_frames;
    } else {   
        ++num_page_replacements;
    }

    if (is_write) {
        page_table[page_num].dirty = true; // Mark the page as dirty on write access
    }
    return true; // Page fault occurred
}

/**
 * @brief Print the statistics of simulation
 * The statistics include:
 * - total number of memory references processed
 * - total number of page faults that occurred
 * - total number of page replacements that occurred
 */
void Replacement::print_statistics() const {
		std::cout << "Number of references: \t\t"  << num_references << ::endl;
		std::cout << "Number of page faults: \t\t" << num_page_faults << std::endl;
		std::cout << "Number of page replacements: \t"  << num_page_replacements << std::endl;
}