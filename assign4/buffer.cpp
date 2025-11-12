/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief Implementation file for the buffer class
 * @version 0.1
 */

// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
#include "buffer.h"
#include <iostream>
// TODO: Add your implementation of the buffer class here

Buffer::Buffer(int sz) : buffer(new buffer_item[sz]), size(sz), count(0), in(0), out(0) {
    pthread_mutex_init(&mtx, nullptr);
    pthread_cond_init(&not_full, nullptr);
    pthread_cond_init(&not_empty, nullptr);
}
Buffer::~Buffer() {
    
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
    delete[] buffer;
}
void Buffer::insert(buffer_item item) {
    pthread_mutex_lock(&mtx);
    while (count == size) {
        pthread_cond_wait(&not_full, &mtx);
    }
    buffer[in] = item;
    in = (in + 1) % size;
    count++;
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mtx);
}

buffer_item Buffer::remove() {
    pthread_mutex_lock(&mtx);
    while (count == 0) {
        pthread_cond_wait(&not_empty, &mtx);
    }
    buffer_item item = buffer[out];
    out = (out + 1) % size;
    count--;
    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&mtx);
    return item;
}
int Buffer::get_size() {
    pthread_mutex_lock(&mtx);
    int s = size;
    pthread_mutex_unlock(&mtx);
    return s;
}

int Buffer::get_count() {
    pthread_mutex_lock(&mtx);
    int c = count;
    pthread_mutex_unlock(&mtx);
    return c;
}

bool Buffer::is_empty() {
    pthread_mutex_lock(&mtx);
    bool empty = (count == 0);
    pthread_mutex_unlock(&mtx);
    return empty;
}

bool Buffer::is_full() {
    pthread_mutex_lock(&mtx);
    bool full = (count == size);
    pthread_mutex_unlock(&mtx);
    return full;
}
void Buffer::print_buffer() {
    pthread_mutex_lock(&mtx);
    std::cout << "Buffer contents: ";
    for (int i = 0; i < count; i++) {
        std::cout << buffer[(out + i) % size] << " ";
    }
    std::cout << std::endl;
    pthread_mutex_unlock(&mtx);
}

