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

/**
 * @brief Constructor for a PageTable with a fixed number of logical pages.
 * @param num_pages Total number of logical pages in the system.
 * 
 * The vector pages is resized to hold num_pages entries. Each entry has the following:
 * - frame_num initialized to -1 (indicating no frame assigned)
 * - valid bit initialized to false (indicating page is not in physical memory)
 * - dirty bit initialized to false (indicating page is clean)
 */
PageTable::PageTable(int num_pages) : pages(num_pages) {
    // Initialize all page entries as invalid pages and unmapped
    for (int i = 0; i < num_pages; ++i) {
        pages[i].frame_num = -1;
        pages[i].valid = false; 
        pages[i].dirty = false; 
    }
}

/**
 * @brief Destructor for the PageTable.
 * @details No manual cleanup is needed because the vector will automatically
 * release its memory when this object is destroyed.
 */
PageTable::~PageTable() {
    
}