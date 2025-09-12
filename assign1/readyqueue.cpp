/**
 * Assignment 1: priority queue of processes
 * @file readyqueue.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief Implementation of ReadyQueue, a priority queue of PCB objects in the READY state.
 *        The highest-priority READY process is selected next.
 * 
 * @version 0.1
 */

#include <iostream>
#include "readyqueue.h"

using namespace std;

// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
//  Remember to add sufficient comments to your code

/**
 * @brief Return the effective priority used by the ready queue heap.
 *
 * Returns -1 for nullptr or for PCBs that are not in the READY state so they
 * will be below any valid [1..50] priority in the max-heap.
 *
 * @param p Pointer to a PCB whose priority is queried.
 * @return int The PCB's priority if READY, otherwise -1.
 */
int ReadyQueue::getPriority(PCB* p) {
    if (p == nullptr)
        return -1; 
    if (p->getState() != ProcState::READY)
        return -1;
    return static_cast<int>(p->getPriority());
}

/**
 * @brief Swap two PCB* references.
 *
 * Utility used by heap maintenance routines.
 * Does not modify PCB contents, only exchanges the pointers in the heap array.
 *
 * @param a Reference to first PCB*.
 * @param b Reference to second PCB*.
 */
void ReadyQueue::swap(PCB *&a, PCB *&b) {
    PCB *t = a;
    a = b;
    b = t;
}

/**
 * @brief Ensure the underlying array has capacity for at least one more element.
 *
 * If full, allocate a new array with doubled capacity (or 16 if starting from 0),
 * copy existing elements, initialize extra slots to nullptr, and free the old storage.
 */
void ReadyQueue::ensureCapacity() {
    if (size_ < capacity)
        return;
    int newCap = (capacity == 0 ? 16 : capacity * 2);
    PCB **newHeap = new PCB *[newCap];

    // Copy over the old elements
    for (int i = 0; i < size_; ++i) {
        newHeap[i] = heap_[i];
    }

    // Initialize any extra slots to nullptr
    for (int i = size_; i < newCap; ++i) {
        newHeap[i] = nullptr;
    }

    // Free old storage
    delete[] heap_;

    heap_ = newHeap;
    capacity = newCap;
}

/**
 * @brief Percolate an element up the heap to restore the max-heap property.
 *
 * Compares the node at @p idx with its parent and swaps while the child has
 * higher effective priority than the parent.
 *
 * @param idx 0-based index of the node to shift up.
 */
void ReadyQueue::shiftUp(int idx) {
    // Continue until we reach the root (index 0)
    while (idx > 0) {
        int parent = (idx - 1) / 2; // parent index in 0-based heap

        // If current node has higher priority than its parent → swap
        if (getPriority(heap_[idx]) > getPriority(heap_[parent])) {
            swap(heap_[idx], heap_[parent]);
            idx = parent; // move up to parent's position
        }
        else {
            break;
        }
    }
}

/**
 * @brief Percolate an element down the heap to restore the max-heap property.
 *
 * Chooses the higher-priority child and swaps while the child outranks the
 * current node by effective priority.
 *
 * @param idx 0-based index of the node to shift down.
 */
void ReadyQueue::shiftDown(int idx)
{
    while (true) {
        int left = 2 * idx + 1;  // left child
        int right = 2 * idx + 2; // right child
        int largest = idx;

        // Compare with left child
        if (left < size_ && getPriority(heap_[left]) > getPriority(heap_[largest])) {
            largest = left;
        }

        // Compare with right child
        if (right < size_ && getPriority(heap_[right]) > getPriority(heap_[largest])) {
            largest = right;
        }

        // If parent is larger than both children → heap property satisfied
        if (largest == idx)
            break;

        // Otherwise swap and continue down the tree
        swap(heap_[idx], heap_[largest]);
        idx = largest;
    }
}

/**
 * @brief Constructor for the ReadyQueue class.
 * Initializes an empty heap and allocates initial capacity.
 * All slots are set to nullptr inside ensureCapacity().
 */
ReadyQueue::ReadyQueue() : heap_(nullptr), size_(0), capacity(0) {
    ensureCapacity();
}

/**
 * @brief Destructor
 * Cleans up the dynamically allocated array. Does NOT delete PCB objects
 * since they are owned by the PCBTable.
 */
ReadyQueue::~ReadyQueue() {
    //  Do NOT delete PCB objects (owned by PCBTable). Only free our array.
    delete[] heap_;
    heap_ = nullptr;
    size_ = 0;
    capacity = 0;
}

/**
 * @brief Add a PCB representing a process into the ready queue.
 *
 * Normalizes the PCB's priority to the allowed range, marks
 * the PCB state as READY, inserts it into the heap, and restores heap order.
 *
 * @param pcbPtr: the pointer to the PCB to be added
 */
void ReadyQueue::addPCB(PCB *pcbPtr) {
    if (!pcbPtr)
        return;

    pcbPtr->setPriority(pcbPtr->getPriority());
    // When adding a PCB to the queue, you must change its state to READY.
    pcbPtr->setState(ProcState::READY);

    // Make sure there is space in the heap
    ensureCapacity();

    // Insert at the end
    heap_[size_] = pcbPtr;

    // Restore heap property by shifting up
    shiftUp(size_);

    // Update size
    ++size_;
}

/**
 * @brief Remove and return the PCB with the highest priority from the queue
 *
 * Replaces the root with the last element, re-heapifies, and marks the removed
 * PCB's state as RUNNING. Returns nullptr if the queue is empty.
 *
 * @return PCB*: the pointer to the PCB with the highest priority, or nullptr if empty.
 */
PCB *ReadyQueue::removePCB() {
    if (size_ == 0) {
        return nullptr;
    }

    // Highest-priority PCB is always at the root (index 0)
    PCB *top = heap_[0];

    // Replace root with last element
    --size_;
    if (size_ > 0)  {
        heap_[0] = heap_[size_];
        shiftDown(0); // restore heap property
    }

    // Mark this PCB as now RUNNING
    top->setState(ProcState::RUNNING);

    return top;
}

/**
 * @brief Returns the number of elements in the queue.
 *
 * @return int: the number of PCBs in the queue
 */
int ReadyQueue::size() {
    return size_;
}

/**
 * @brief Display the PCBs in the queue.
 *
 * Prints a header and then each PCB in the heap's current array order
 */
void ReadyQueue::displayAll() {
    std::cout << "Display Processes in ReadyQueue:\n";

    // Print the heap it its current array order
    for (int i = 0; i < size_; ++i) {
        if (heap_[i]) {
            std::cout << "\t";
            heap_[i]->display();
        }
    }
}