/**
* Assignment 5: Page replacement algorithms
 * @file pagetable.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief This class represents a traditional pagetable data structure.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "pagetable.h"

// TODO: Add your implementation of PageTable
// Constructor
// TODO: Add your code
PageTable::PageTable(int num_pages) : pages(num_pages) {
    
    // Initialize all page entries as invald pages and unmapped
    for (int i = 0; i < num_pages; ++i) {
        pages[i].frame_num = -1; // no frame yet
        pages[i].valid = false; // not in physical memory
        pages[i].dirty = false; // clean page
    }
}

// Destructor
// TODO: Add your code
PageTable::~PageTable() {
    
}