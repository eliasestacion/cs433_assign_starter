/**
* Assignment 5: Page replacement algorithms
 * @file lru_replacement.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief A class implementing the LRU page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "lru_replacement.h"

/**
 * @brief Construct a new LRUReplacement object.
 * @param num_pages Total number of logical pages.
 * @param num_frames Total number of available frames.
 */
LRUReplacement::LRUReplacement(int num_pages, int num_frames)
: Replacement(num_pages, num_frames)
{
}

/**
 * @brief Destructor for LRUReplacement.
 */
LRUReplacement::~LRUReplacement()
{
}

/**
 * @brieg Handle an access to a page that is already in physical memory.
 * @param page_num The logical page number.
 * 
 * For the LRU algorithm, we need to update the usage order of pages.
 */
void LRUReplacement::touch_page(int page_num) {
    auto it = page_map.find(page_num);
    if (it == page_map.end()) {
        // Page not found in the map which should not happen
        return;
    }

    // Remove the page's current node from the list
    lru_list.erase(it->second);
    // Re-insert the page at the back of the list (most recently used)
    lru_list.push_back(page_num);
    // Update the iterator in the map
    auto new_it = lru_list.end();
    --new_it;
    it->second = new_it;
}

/**
 * @brief Load a page into a free fram
 * @param page_num The logical page number.
 * Assign the page to the next free frame and update LRU tracking structures.
 */
void LRUReplacement::load_page(int page_num) {
    // Get the page table entry for this logical page
    PageEntry &entry = page_table[page_num];

    // Assign the page to the next free frame.
    entry.frame_num = next_free_frame;
    entry.valid = true;
    entry.dirty = false;

    // Insert the page at the back of the list as the most recently used.
    lru_list.push_back(page_num);
    auto it = lru_list.end();
    --it;
    // Store the iterator in the map for quick access later.
    page_map[page_num] = it;

    // Advance to the next free frame.
    ++next_free_frame;
}

/**
 * @brief Replace a page using the LRU algorithm.
 * @param page_num The logical page number of the desired page.
 * @return Selected victim page number
 */
int LRUReplacement::replace_page(int page_num) {
    // Thw victim is the least recently used page at the front of the list
    int victim_page = lru_list.front();
    lru_list.pop_front();
    // Remove the victim from the map
    page_map.erase(victim_page);

    // Get the frame number of the victim page
    PageEntry &victim_entry = page_table[victim_page];
    int frame = victim_entry.frame_num;

    // Mark the victim page as invalid and clean
    victim_entry.valid = false;
    victim_entry.dirty = false;

    // Load the new page into the victim's frame
    PageEntry &new_entry = page_table[page_num];
    new_entry.frame_num = frame;
    new_entry.valid = true;
    new_entry.dirty = false;

    // Add the new page at the back of the list as the most recently used.
    lru_list.push_back(page_num);
    auto it = lru_list.end();
    --it;
    page_map[page_num] = it;

    return victim_page;
}