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

// TODO: Add your implementation here
LIFOReplacement::LIFOReplacement(int num_pages, int num_frames)
: Replacement(num_pages, num_frames)
{
    // TODO: Add additional implementation code
}

// TODO: Add your implementations for desctructor, load_page, replace_page here
LIFOReplacement::~LIFOReplacement() {
    // TODO: Add necessary code here
}

// Access an invalid page, but free frames are available
void LIFOReplacement::load_page(int page_num) {
    // TODO: Add necessary code here
    int frame_number = next_free_frame;
    page_table[page_num].frame_num = frame_number;
    page_table[page_num].valid = true;
    lifo_stack.push(page_num);
    
}

// Access an invalid page and no free frames are available
int LIFOReplacement::replace_page(int page_num) {
    // TODO: Add necessary code here
    int victim_page = lifo_stack.top();
    lifo_stack.pop();
    int frame_number = page_table[victim_page].frame_num;
    page_table[victim_page].valid = false;
    page_table[page_num].frame_num = frame_number;
    page_table[page_num].valid = true;
    lifo_stack.push(page_num);
    return 0;
}