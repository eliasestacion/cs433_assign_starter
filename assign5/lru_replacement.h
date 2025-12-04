/**
* Assignment 5: Page replacement algorithms
 * @file lru_replacement.h
 * @author Elias Estacion and Meliton Rojas 
 * @brief A class implementing the LRU page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#pragma once

// Remember to add comments to your code

#include "replacement.h"
#include <list>
#include <unordered_map>

/**
 * @brief A class to simulate the least recently used (LRU) page replacement algorithm.
 * @details This class is a subclass of Replacement and overrides the following virtual functions:
 * - touch_page()
 * - load_page()
 * - replace_page()
 */
class LRUReplacement : public Replacement
{
private:
    // Member variable to track the usage order of pages
    std::list<int> lru_list; // List to maintain LRU order
    std::unordered_map<int, std::list<int>::iterator> page_map; // Map to quickly access list iterators
public:
	/**
	 * @brief Construct a new LRUReplacement object.
	 * @param num_pages Total number of logical pages.
	 * @param num_frames Total number of available frames.
	 */
	LRUReplacement(int num_pages, int num_frames);
	
    /**
    * @brief Destructor
    */
    virtual ~LRUReplacement();

    /**
     * @brief Accesss a page already in physical memory
     * It may be overridden in a subclass 
     * @param page_num The logical page number.
     */
    virtual void touch_page(int page_num);

    /**
     * @brief Access an invalid page, but free frames are available.
     * Assign the page to an available  frame, not replacement needed
     * @param page_num The logical page number.
     */
    virtual void load_page(int page_num);

    /**
     * @brief Access an invalid page, and there is no free frame.
     * Replace the page with the page that has been in memory the longest.
     * @param page_num The logical page number.
     * @return Selected victim page #
     */
    virtual int replace_page(int page_num);

};