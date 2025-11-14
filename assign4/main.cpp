/**
* Assignment 4: Producer Consumer Problem
 * @file main.cpp
 * @author Elias Estacion and Meliton Rojas
 * @brief The main program for the producer consumer problem.
 * @version 0.1
 */

#include <iostream>
#include "buffer.h"
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <pthread.h> 

using namespace std;

// global buffer object used by all threads
Buffer buffer;

// A print mutex to prevent overlapping thread messages
pthread_mutex_t io_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Producer thread function
 * 
 * Each producer sleeps for an amount of time under 1 second
 * and then inserts its own ID into the buffer.
 * 
 */
void *producer(void *param) {
    buffer_item item = *((int *) param);

    while (true) {
        usleep(rand() % 1000000);

        if (buffer.insert_item(item)) {
            pthread_mutex_lock(&io_mutex);
            cout << "Producer " << item << ": Inserted item " << item << endl;
            buffer.print_buffer();
            pthread_mutex_unlock(&io_mutex);
        } else {
            pthread_mutex_lock(&io_mutex);
            cout << "Producer error condition" << endl; // shouldn't come here
            pthread_mutex_unlock(&io_mutex);
        }
    }
    return nullptr;
}

/**
 * @brief Consumer thread function
 * 
 * Each consumer sleeps for an amount of time under 1 second
 * and then removes an item from the buffer.
 * 
 */
void *consumer(void *param) {
    (void) param;
    buffer_item item;

    while (true) {
        /* sleep for a random period of time */
        usleep(rand() % 1000000);

        if (buffer.remove_item(&item)) {
            pthread_mutex_lock(&io_mutex);
            cout << "Consumer " << item << ": Removed item " << item << endl;
            buffer.print_buffer();
            pthread_mutex_unlock(&io_mutex);
        } else {
            pthread_mutex_lock(&io_mutex);
            cout << "Consumer error condition" << endl; // shouldn't come here
            pthread_mutex_unlock(&io_mutex);
        }
    }
    return nullptr;

}

int main(int argc, char *argv[]) {

    // Check correct argument count
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <sleep_time> <num_producers> <num_consumers>" << endl;
        return 1;
    }

    // Convert arguments
    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    // Initialize buffer and seed random number generator
    srand((time(nullptr)));

    // Create producer threads
    pthread_t *producer_threads = new pthread_t[num_producers];
    int       *producer_ids = new int[num_producers];

    for (int i = 0; i < num_producers; ++i) {
        producer_ids[i] = i + 1;

        pthread_create(&producer_threads[i], nullptr, producer, &producer_ids[i]);

        pthread_detach(producer_threads[i]);
    }

    // Create consumer threads
    pthread_t *consumer_threads = new pthread_t[num_consumers];

    for (int i = 0; i < num_consumers; ++i) {
        pthread_create(&consumer_threads[i], nullptr, consumer, nullptr);

        pthread_detach(consumer_threads[i]);
    }

    // Main thread sleeps for the user's time
    sleep(sleep_time);

    // Threads exit automatically when main ends
    delete[] producer_threads;
    delete[] producer_ids;
    delete[] consumer_threads;
    
    return 0;
}
