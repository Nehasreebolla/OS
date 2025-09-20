#include <iostream>
#include <fstream>      // Include for file operations
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <chrono>
#include <unistd.h>
#include <queue>

using namespace std;

// Global variables to hold input parameters
int NUM_READERS, NUM_WRITERS, NUM_READS_PER_READER, NUM_WRITES_PER_WRITER;

// File names for input and output
const string inputFileName = "inp-params.txt";
const string outputFileName = "RW-log.txt";         // Output file name
const string outputFileName1 = "Average_time.txt";  // Output file name

// Output file stream
ofstream outputFile;
ofstream outputFile1;

// Function to read values from input file
void readInputFromFile() {
    ifstream inputFile(inputFileName); // Open input file
    if (!inputFile.is_open()) {       // Check if file opened successfully
        cerr << "Failed to open input file." << endl;
        exit(EXIT_FAILURE);
    }
    
    // Read values from file
    inputFile >> NUM_READERS >> NUM_WRITERS >> NUM_READS_PER_READER >> NUM_WRITES_PER_WRITER;
    
    // Close the input file
    inputFile.close();
}

// Function to get system time
string getSysTime() {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char buffer[6]; // HH:MM format
    strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);
    return string(buffer);
}

// Semaphore and variables for synchronization
sem_t reader_mutex, writer_mutex, resource_mutex;
int readers = 0, writers_waiting = 0;
queue<int> writer_queue;

// Variables to calculate average times
double total_reader_time = 0.0;
double total_writer_time = 0.0;

// Reader thread function
void reader(int id) {
    for (int i = 0; i < NUM_READS_PER_READER; ++i) {
        // Requesting critical section
        string reqTime = getSysTime();
        outputFile << i << "th CS request by Reader Thread " << id << " at " << reqTime << endl;
        clock_t start_time = clock();
        sem_wait(&resource_mutex);
        readers++;
        if (readers == 1) {
            sem_wait(&writer_mutex);
        }
        sem_post(&resource_mutex);
        
        // Entering critical section
        if (!writer_queue.empty()) {
            sem_wait(&reader_mutex);
        }
        if (writer_queue.empty()) {
            sem_post(&reader_mutex);
        }
        string enterTime = getSysTime();
        total_reader_time += (double)(clock() - start_time) / CLOCKS_PER_SEC;
        outputFile << i << "th CS Entry by Reader Thread " << id << " at " << enterTime << endl;
        usleep(10 * 1000); // Simulate a thread reading from CS (convert to microseconds)

        // Exiting critical section
        string exitTime = getSysTime();
        outputFile << i << "th CS Exit by Reader Thread " << id << " at " << exitTime << endl;
        usleep(5 * 1000); // Simulate a thread executing in the remainder section (convert to microseconds)

        // Exiting critical section
        sem_wait(&resource_mutex);
        readers--;
        if (readers == 0) {
            sem_post(&writer_mutex);
        }
        sem_post(&resource_mutex);
    }
}

// Writer thread function
void writer(int id) {
    for (int i = 0; i < NUM_WRITES_PER_WRITER; ++i) {
        // Requesting critical section
        string reqTime = getSysTime();
        outputFile << i << "th CS request by Writer Thread " << id << " at " << reqTime << endl;
        clock_t start_time = clock();
        sem_wait(&writer_mutex); // Lock the writer mutex
        writers_waiting++;
        sem_post(&writer_mutex); // Unlock the writer mutex
        
        if (writers_waiting == 1) {
            sem_wait(&reader_mutex); // Wait for readers to finish
        }
        
        sem_wait(&writer_mutex); // Lock the writer mutex
        if (!writer_queue.empty() || readers > 0) {
            writer_queue.push(id);
        } else {
            sem_post(&writer_mutex); // Unlock the writer mutex
            // Writing
            string enterTime = getSysTime();
            total_writer_time += (double)(clock() - start_time) / CLOCKS_PER_SEC;
            outputFile << i << "th CS Entry by Writer Thread " << id << " at " << enterTime << endl;
            usleep(10 * 1000); // Simulate a thread writing in CS (convert to microseconds)
            string exitTime = getSysTime();
            outputFile << i << "th CS Exit by Writer Thread " << id << " at " << exitTime << endl;
            usleep(5 * 1000); // Simulate a thread executing in the remainder section (convert to microseconds)
        }
        
        sem_post(&writer_mutex); // Unlock the writer mutex
        sem_wait(&writer_mutex); // Lock the writer mutex
        writers_waiting--;
        if (writers_waiting == 0) {
            sem_post(&reader_mutex); // Signal readers
        }
        sem_post(&writer_mutex); // Unlock the writer mutex
    }
}

int main() {
    readInputFromFile();
    sem_init(&reader_mutex, 0, 1);
    sem_init(&writer_mutex, 0, 1);
    sem_init(&resource_mutex, 0, 1);

    // Open the output files
    outputFile.open(outputFileName);
    outputFile1.open(outputFileName1);
    if (!outputFile.is_open()) { // Check if file opened successfully
        cerr << "Failed to open output file." << endl;
        exit(EXIT_FAILURE);
    }

    // Arrays to hold reader and writer threads
    thread readers[NUM_READERS];
    thread writers[NUM_WRITERS];

    // Creating reader threads
    for (int i = 0; i < NUM_READERS; ++i) {
        readers[i] = thread(reader, i);
    }
    
    // Creating writer threads
    for (int i = 0; i < NUM_WRITERS; ++i) {
        writers[i] = thread(writer, i);
    }
    
    // Joining reader threads
    for (int i = 0; i < NUM_READERS; ++i) {
        readers[i].join();
    }
    
    // Joining writer threads
    for (int i = 0; i < NUM_WRITERS; ++i) {
        writers[i].join();
    }

    // Destroying semaphores
    sem_destroy(&reader_mutex);
    sem_destroy(&writer_mutex);
    sem_destroy(&resource_mutex);

    // Calculate and print average times
    double avg_reader_time = total_reader_time / (NUM_READERS * NUM_READS_PER_READER);
    double avg_writer_time = total_writer_time / (NUM_WRITERS * NUM_WRITES_PER_WRITER);
    outputFile1 << "Average time for each reader to enter CS: " << avg_reader_time << " seconds" << endl;
    outputFile1 << "Average time for each writer to enter CS: " << avg_writer_time << " seconds" << endl;
    outputFile1 << "Average time for a thread to enter CS: " << (avg_reader_time+avg_writer_time)/2 << " seconds" << endl;

    // Close the output files
    outputFile.close();
    outputFile1.close();

    return 0;
}
