/**
* Assignment 5: Page replacement algorithms
 * @file lifo_replacement.h
 * @author Elias Estacion and Meliton Rojas
 * @brief A class implementing the LIFO page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#pragma once

// Remember to add comments to your code

#include "replacement.h"
#include <stack>

/**
 * @brief A class to simulate LIFO (last in first out) page replacement algorithm.
 * Uses a stack where the newest page is replaced first.
 */
class LIFOReplacement : public Replacement {
private:
    std::stack<int> lifo_stack;  // Stack to keep track of pages in LIFO order

public:
    /**
     * @brief Constructor
     * @param num_pages Total number of available free frames.
     * @param num_frames Total number of free frames.
     */
    LIFOReplacement(int num_pages, int num_frames);

    /**
    * @brief Destructor
    */
    virtual ~LIFOReplacement();

    /**
     * @brief Assign the page to an available  frame, not replacement needed
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
