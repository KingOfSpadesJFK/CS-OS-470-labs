#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <limits.h>

void* thread_function(void* arg) {
    // This is the code that will run in the new thread
    printf("This is the New thread");
}

int min = INT_MAX;      // The maximum integer limit will be the min for now
int max = INT_MIN;      // The minimum integer limit will be the max for now

// Gets the min of the collection of numbers
void* getMin(void* arg) {
    int* elements = (int*)arg;
    int elementCount = *elements;
    for (int i = 1; i < elementCount; i++) {
        if (elements[i] < min) {
            min = elements[i];
        }
    }
    pthread_exit(NULL);
}

// Gets the max of the collection of numbers
void* getMax(void* arg) {
    int* elements = (int*)arg;
    int elementCount = *elements;
    for (int i = 1; i < elementCount; i++) {
        if (elements[i] > max) {
            max = elements[i];
        }
    }
    pthread_exit(NULL);
}

// Power function
int power(int base, int power) {
    int i = 1;
    while (power > 0) {
        i *= base;
        power--;
    }
    return i;
}

#define MAX_THREADS 2

int main(const int argc, const char* argv[]) {
    // Stop the program if there's no integers passed in
    if (argc == 1) {
        printf("You must pass a collection of integers!\n");
        return 1;
    }

    // Convert the strings into integers
    int nums[argc];
    nums[0] = argc;     // Set the first element to be the number of ints to sort through
    for (int i = 1; i < argc; i++) {    // Loop through the arguments
        int num = 0;
        for (int j = 0; j < strlen(argv[i]); j++) {    // Loop through the characters of the argument
            const char current = argv[i][j];
            // Stop if the arg isn't a valid integer
            if (current < '0' || current > '9') {
                printf("Argument %d must be a valid integer!\n", i);
                return 1;
            }
            num += ((current - '0') * power(10, strlen(argv[i]) - 1 - j));
        }
        nums[i] = num;
    }

    // Create the threads
    pthread_t my_thread[MAX_THREADS];
    if (pthread_create(&my_thread[0], NULL, getMin, nums))  {
        printf("Error creating the getMin thread!\n");
        return 1;
    }

    if (pthread_create(&my_thread[1], NULL, getMax, nums))  {
        printf("Error creating the getMax thread!\n");
        return 1;
    }

    // Then join the threads
    if (pthread_join(my_thread[0], NULL)) { // Wait for the min thread to finish
        printf("Error joining the getMin thread!\n");
        return 1;
    }
    if (pthread_join(my_thread[1], NULL)) { // Wait for the max thread to finish
        printf("Error joining the getMax thread!\n");
        return 1;
    }

    // Print the results!
    printf("The min is %d.\n", min);
    printf("The max is %d.\n", max);
    return 0;

}
