/**
* Assignment 5: Page replacement algorithms
 * @file fifo_replacement.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief A class implementing the FIFO page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "fifo_replacement.h"

/**
 * @brief Constructor for FIFOReplacement
 * Initializes the base Replacement class with the given number of pages and frames.
 * @param num_pages Total number of logical pages.
 * @param num_frames Total number of available free frames.
 */
FIFOReplacement::FIFOReplacement(int num_pages, int num_frames)
: Replacement(num_pages, num_frames) 
{
}

/**
 * @brief Destructor for FIFOReplacement
 */
FIFOReplacement::~FIFOReplacement() {
    
}

/**
 * @brief Loads a page into the next available free frame.
 * This is used when the accessed page is invalid but there are free frames available.
 * FIFO inserts the page into the queue to track arrival order.
 * @param page_num The logical page number to load.
 */
void FIFOReplacement::load_page(int page_num) {
    int frame_number = next_free_frame;
    page_table[page_num].frame_num = frame_number;
    page_table[page_num].valid = true;
    fifo_queue.push(page_num);
    
}

/**
* @brief Replaces a page using FIFO strategy when no free frames are available.
* THe page that entered the memory first is selected as the victim page.
* @param page_num The logical page number to load.
 */
int FIFOReplacement::replace_page(int page_num) {
    int victim_page = fifo_queue.front();
    fifo_queue.pop();

    int frame_number = page_table[victim_page].frame_num;

    // Mark victim page as invalid
    page_table[victim_page].valid = false;

    // Assign the frame to the new page
    page_table[page_num].frame_num = frame_number;
    page_table[page_num].valid = true;

    fifo_queue.push(page_num);    
    return 0;
}