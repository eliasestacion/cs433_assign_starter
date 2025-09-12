/**
 * Assignment 1: priority queue of processes
 * @file pcbtable.h
 * @author Elias Estacion and Meliton Rojas
 * @brief This is the implementation file for the PCBTable class.
 * //You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
 * // Remember to add sufficient comments to your code
 */

#include "pcbtable.h"

/**
 * @brief Construct a new PCBTable object of the given size (number of PCBs)
 *
 * This function creates a dynamic array of PCB pointers with the capacity that has been requested.
 * All entries are initialized to nullptr.
 * 
 * @param size: the capacity of the PCBTable
 */
PCBTable::PCBTable(int size)
   : pcbArray(nullptr), tableSize(size) {
    if (tableSize < 1) tableSize = 1;

    // allocate the array of PCB pointers
    pcbArray = new PCB*[tableSize];

    // initialize all slots to nullptr
    for (int i = 0; i < tableSize; ++i) {
        pcbArray[i] = nullptr;
    }
}

/**
 * @brief Destroy the PCBTable object. Make sure to delete all the PCBs in the table.
 *
 */
PCBTable::~PCBTable() {
   if (pcbArray) {
    // delete all PCB objects stored in the tble
    for (int i = 0; i < tableSize; ++i) {
        delete pcbArray[i];
        pcbArray[i] = nullptr;
    }

    // free the array of PCB* itself
    delete[] pcbArray;
    pcbArray = nullptr;
   }

   // mark table as empty
   tableSize = 0;
}

/**
 * @brief Get the PCB at index "idx" of the PCBTable.
 *
 * @param idx: the index of the PCB to get
 * @return PCB*: pointer to the PCB at index "idx"
 */
PCB* PCBTable::getPCB(unsigned int idx) {
    // check that array exists and that the index is within bounds
    if (!pcbArray || idx >= static_cast<unsigned int>(tableSize)) {
        return nullptr;
    }
    
    // return the PCB pointer stored at idx
    return pcbArray[idx];
}

/**
 * @brief Add a PCB pointer to the PCBTable at index idx.
 *
 * @param pcb: the PCB to add
 */
void PCBTable::addPCB(PCB *pcb, unsigned int idx) {
    if (!pcbArray || idx >= static_cast<unsigned int>(tableSize)) {
        delete pcb;
        return;
    }

    // delete old PCB if slot is occupied
    delete pcbArray[idx];

    // store new PCB in this slot
    pcbArray[idx] = pcb;
}
