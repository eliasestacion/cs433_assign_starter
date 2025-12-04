/**
* Assignment 5: Page replacement algorithms
 * @file lifo_replacement.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief A class implementing the Last in First Out (LIFO) page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "lifo_replacement.h"

/**
 * @brief Constructor for LIFOReplacement
 * Calls base class constructor to initialize core fields.
 */
LIFOReplacement::LIFOReplacement(int num_pages, int num_frames)
: Replacement(num_pages, num_frames)
{

}

/**
 * @brief Destructor for LIFOReplacement
 */
LIFOReplacement::~LIFOReplacement() {
    
}

/**
 * @brief Loads a page into a free frame when available.
 * The page is pushed onto the stack as the newest element.
 * @param page_num The logical page number to load.
 */
void LIFOReplacement::load_page(int page_num) {
    int frame_number = next_free_frame;
    page_table[page_num].frame_num = frame_number;
    page_table[page_num].valid = true;
    // Track this page as the most recent one
    lifo_stack.push(page_num);
    
}

/**
 * @brief Performs page replacement using LIFO strategy.
 * The most recently loaded page is selected as the victim page.
 * @param page_num The logical page number to load.
 * @return Selected victim page #
 */
int LIFOReplacement::replace_page(int page_num) {
    int victim_page = lifo_stack.top();
    lifo_stack.pop();
    int frame_number = page_table[victim_page].frame_num;
    page_table[victim_page].valid = false;
    page_table[page_num].frame_num = frame_number;
    page_table[page_num].valid = true;

    // New page becomes the newest for future LIFO decisions
    lifo_stack.push(page_num);
    return 0;
}