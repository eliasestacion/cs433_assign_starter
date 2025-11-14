/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief Implementation file for the buffer class
 * @version 0.1
 */

#include "buffer.h"
#include <iostream>

/**
 * @brief Constructor for the Buffer
 * 
 * Creates an empty circular buffer and initializes the mutex
 * and condition variables used for thread synchronization
 */
Buffer::Buffer(int sz) : buffer(new buffer_item[sz]), size(sz), count(0), in(0), out(0) {
    pthread_mutex_init(&mtx, nullptr);
    pthread_cond_init(&not_full, nullptr);
    pthread_cond_init(&not_empty, nullptr);
}

/**
 * @brief Destroyer for the  Buffer
 * 
 * Cleans up the mutex, condition variables, and deletes
 * the dynamically allocated array
 */
Buffer::~Buffer() {
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
    delete[] buffer;
}

/**
 * @brief Inter an item into the buffer
 * If the buffer is full, the thread waits until space is available
 * 
 * @param item The item to insert
 *
 */
bool Buffer::insert_item(buffer_item item) {
    pthread_mutex_lock(&mtx);

    while (count == size) {
        pthread_cond_wait(&not_full, &mtx);
    }
    buffer[in] = item;
    in = (in + 1) % size;
    count++;

    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mtx);
    return true;
}

/**
 * @brief Remove an item from the buffer
 * If the buffer is empty, the thread waits until an item appears
 * 
 * @param item Pointer to store the removed item
 * 
 */
bool Buffer::remove_item(buffer_item *item) {
    pthread_mutex_lock(&mtx);
    while (count == 0) {
        pthread_cond_wait(&not_empty, &mtx);
    }
    *item = buffer[out];
    out = (out + 1) % size;
    --count;

    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&mtx);
    return true;
}

/**
 * @brief Get the maximum size of the buffer
 */
int Buffer::get_size() {
    pthread_mutex_lock(&mtx);
    int s = size;
    pthread_mutex_unlock(&mtx);
    return s;
}

/**
 * @brief Get the current count of items stored
 */
int Buffer::get_count() {
    pthread_mutex_lock(&mtx);
    int c = count;
    pthread_mutex_unlock(&mtx);
    return c;
}

/**
 * @brief Check if the buffer is empty
 */
bool Buffer::is_empty() {
    pthread_mutex_lock(&mtx);
    bool empty = (count == 0);
    pthread_mutex_unlock(&mtx);
    return empty;
}

/**
 * @brief Check if the buffer is full
 */
bool Buffer::is_full() {
    pthread_mutex_lock(&mtx);
    bool full = (count == size);
    pthread_mutex_unlock(&mtx);
    return full;
}

/**
 * @brief Print the current content of the buffer (FIFO order)
 */
void Buffer::print_buffer() {
    pthread_mutex_lock(&mtx);

    std::cout << "Buffer: [";
    for (int i = 0; i < count; ++i) {
        int idx = (out + i) % size;
        std::cout << buffer[idx];
        if (i < count - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
    pthread_mutex_unlock(&mtx);
}

