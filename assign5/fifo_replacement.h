/**
* Assignment 5: Page replacement algorithms
 * @file fifo_replacement.h
 * @author Elias Estacion and Meliton Rojas
 * @brief A class implementing the FIFO page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#pragma once

// Remember to add comments to your code

#include "replacement.h"
#include <queue>

/**
 * @brief A class to simulate FIFO page replacement algorithm.
 * The oldest loaded page is always selected as the victim page for replacement.
 */
class FIFOReplacement : public Replacement {
private:
    std::queue<int> fifo_queue;  // Queue to keep track of pages in FIFO order
    
public:
    /**
     * @brief Constructor
     * @param num_pages Total number of available free frames.
     * @param num_frames Total number of free frames.
     */
    FIFOReplacement(int num_pages, int num_frames);

    /**
    * @brief Destructor
    */
    virtual ~FIFOReplacement();

    /**
     * @brief Assign the page to an available  frame, with no replacement needed
     * @param page_num The logical page number.
     */
    virtual void load_page(int page_num);

    /**
     * @brief Replace the page with the page that has been in memory the longest.
     * @param page_num The logical page number.
     * @return Selected victim page #
     */
    virtual int replace_page(int page_num);

};
