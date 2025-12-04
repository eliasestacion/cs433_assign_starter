/**
* Assignment 5: Page replacement algorithms
 * @file replacement.h
 * @author Elias Estacion and Meliton Rojas
 * @brief A base class for different page replacement algorithms.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
#pragma once

#include "pagetable.h"


/**
 * @brief A base class to simulate page replacement algorithms.
 * A specific page replacement algorithm, e.g. FIFO or LRU, should be subclass of this and override the virtual functions:
 * - touch_page()
 * - load_page()
 * - replace_page()
 */
class Replacement
{
protected: 
    // Member variable for the page table
    PageTable page_table;

    int num_frames;         // Total number of available frames
    int free_frames;        // Number of free frames available
    int next_free_frame;    // Index of the next free frame to use

    unsigned long long num_references = 0;  // total number of memory references processed.
    unsigned long long num_page_faults = 0; // total number of page faults that occurred.
    unsigned long long num_page_replacements = 0; // total number of page replacements that occurred.
	
public:
	/**
	 * @brief Constructor for the Replacement class.
	 * @param num_pages Total number of logical pages for the simulation.
	 * @param num_frames Total number of available free frames.
     * 
     * This constructor initializes the page table with num_pages entries and sets up
     * the counters for free frames and statistics.
	 */
	Replacement(int num_pages, int num_frames);

    /**
     * @brief Destructor for the Replacement class.
     */
    virtual ~Replacement();

    /**
	 * @brief Simulate a single page access.
     * @details If the page is valid, it calls the touch_page function. 
     *          If the page is not valid but free frames are available, it calls the load_page function.
     *          If the page is not valid and there is no free frame, it calls the replace_page function.
     * @param page_num The logical page number.
	 * @param is_write whether this access a memory write
	 * @return whether it's a page fault: true if it's a page fault
	 */
    virtual bool access_page(int page_num, bool is_write = false);

    /**
	 * @brief Accesss a page already in physical memory
	 * It may be overridden in a subclass 
	 * @param page_num The logical page number that was accessed and is already valid.
     */
    virtual void touch_page(int page_num) {}

    /**
     * @brief Access an invalid page, but free frames are available.
     * Assign the page to an available frame, not replacement needed
     * It may be overridden in a subclass 
     * @param page_num The logical page number.
     */
    virtual void load_page(int page_num) {}


    /**
	 * @brief Access an invalid page and no free frame is available.
     * Select a victim page to be replaced.
	 * It is a pure virtual function to be implemented in specific replacement subclasses.
     * @param page_num  The logical page number of the desired page.
	 * @return Selected victim page #
	 */
    virtual int replace_page(int page_num) = 0;

    /**
	 * @brief Get the ith entry of the page table
     * @param page_num The logical page number.
     * @return A copy of the PageEntry object for the given page.
	 */
    PageEntry getPageEntry(int page_num) {
        return page_table[page_num];
    }

    /**
	 * @brief Print the statistics of simulation
	 */
    void print_statistics() const;
};