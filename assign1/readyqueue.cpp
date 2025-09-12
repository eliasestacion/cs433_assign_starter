#include <iostream>
#include "readyqueue.h"

using namespace std;

// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
//  Remember to add sufficient comments to your code
int ReadyQueue::getPriority(PCB* p) {
    if (p == nullptr)
        return -1; // lower than any valid [1..50] priority
    if (p->getState() != ProcState::READY)
        return -1;
    return static_cast<int>(p->getPriority());
}

void ReadyQueue::swap(PCB *&a, PCB *&b) {
    PCB *t = a;
    a = b;
    b = t;
}

void ReadyQueue::ensureCapacity() {
    if (size_ < capacity)
        return; // still space available, nothing to do
    int newCap = (capacity == 0 ? 16 : capacity * 2);
    PCB **newHeap = new PCB *[newCap];

    // Copy over the old elements
    for (int i = 0; i < size_; ++i) {
        newHeap[i] = heap_[i];
    }

    // Initialize any extra slots to nullptr (optional, but safer)
    for (int i = size_; i < newCap; ++i) {
        newHeap[i] = nullptr;
    }

    // Free old storage
    delete[] heap_;

    heap_ = newHeap;
    capacity = newCap;
}

void ReadyQueue::shiftUp(int idx) {
    // 0-based max-heap
    // Continue until we reach the root (index 0)
    while (idx > 0) {
        int parent = (idx - 1) / 2; // parent index in 0-based heap

        // If current node has higher priority than its parent → swap
        if (getPriority(heap_[idx]) > getPriority(heap_[parent])) {
            swap(heap_[idx], heap_[parent]);
            idx = parent; // move up to parent's position
        }
        else {
            break; // heap property satisfied
        }
    }
}

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
 */
ReadyQueue::ReadyQueue() : heap_(nullptr), size_(0), capacity(0) {
    //  All slots are set to nullptr inside ensureCapacity().
    ensureCapacity();
}

/**
 *@brief Destructor
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
 * @param pcbPtr: the pointer to the PCB to be added
 */
void ReadyQueue::addPCB(PCB *pcbPtr) {
    //  When adding a PCB to the queue, you must change its state to READY.
    if (!pcbPtr)
        return; // ignore null inserts

    // This clamps the priority into 1..50
    pcbPtr->setPriority(pcbPtr->getPriority());

    // Any process added to the ready queue must be marked READY
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
 * @return PCB*: the pointer to the PCB with the highest priority
 */
PCB *ReadyQueue::removePCB() {
    //  When removing a PCB from the queue, you must change its state to RUNNING.
    if (size_ == 0) {
        return nullptr; // queue empty
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
 */
void ReadyQueue::displayAll() {
    // Print header for test1 output
    std::cout << "Display Processes in ReadyQueue:\n";

    // Print the heap it its current array order
    for (int i = 0; i < size_; ++i) {
        if (heap_[i]) {
            std::cout << "\t";
            heap_[i]->display();
        }
    }
}