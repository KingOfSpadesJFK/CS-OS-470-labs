#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

typedef struct Matrix {
    // Row first, column second
    // index = row * colCount + col
    int* data;
    int rowCount;
    int colCount;
} Matrix;

typedef struct ThreadData {
    const Matrix* matrixA;
    const Matrix* matrixB;
    Matrix result;
} ThreadData;

// Three rows, four columns
int data1[] = {
    1,  2,  3,  4,
    5,  6,  7,  8,
    9, 10, 11, 12,
};

// Four rows, three columns
int data2[] = {
    21, 21,  5,
     1,  7, 63,
     2,  4,  9,
    32,  6, 24
};

// Three rows, four columns
int data3[] = {
    12, 43,  4, 55,
     1, 27,  8,  9,
    28, 10, 32,  3,
};

const Matrix neo = {
    data1,
    3, 4
};

const Matrix trinity = {
    data2,
    4, 3
};

const Matrix morpheus = {
    data3,
    3, 4
};

void* add(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    const Matrix* a = data->matrixA;
    const Matrix* b = data->matrixB;
    // Make sure these matrices have the same dimensions
    assert(a->colCount == b->colCount && a->rowCount == b->rowCount);
    int* ret = data->result.data;

    // Add the matrices together
    for (int i = 0; i < a->rowCount; i++) {
        for (int j = 0; j < a->colCount; j++) {
            int index = i * a->colCount + j;
            // Add the numbers
            int sum = a->data[index] + b->data[index];
            ret[index] = sum;
        }
    }

    // Set the return matrix to the appropriate data
    data->result.data = ret;
    data->result.colCount = a->colCount;
    data->result.rowCount = a->rowCount;
    pthread_exit(NULL);
}

void* sub(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    const Matrix* a = data->matrixA;
    const Matrix* b = data->matrixB;
    // Make sure these matrices have the same dimensions
    assert(a->colCount == b->colCount && a->rowCount == b->rowCount);
    int* ret = data->result.data;

    // Subtract the matrices together
    for (int i = 0; i < a->rowCount; i++) {
        for (int j = 0; j < a->colCount; j++) {
            int index = i * a->colCount + j;
            // Subtract the numbers
            int diff = a->data[index] - b->data[index];
            ret[index] = diff;
        }
    }

    // Set the return matrix to the appropriate data
    data->result.colCount = a->colCount;
    data->result.rowCount = a->rowCount;
    pthread_exit(NULL);

}

void* multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    const Matrix* a = data->matrixA;
    const Matrix* b = data->matrixB;
    // Make sure these matrices have the appropriate dimensions
    // Matrix A has the same number of rows as the number of columns in B
    assert(a->colCount == b->rowCount && a->rowCount == b->colCount);
    int* ret = data->result.data;

    int rowCount = a->rowCount;
    int colCount = b->colCount;
    // Multiply the matrices
    for (int rowA = 0; rowA < a->rowCount; rowA++) {
        for (int colB = 0; colB < b->colCount; colB++) {
            int dot = 0;
            for (int colA = 0; colA < a->colCount; colA++) {
                int indexA = rowA * a->colCount + colA;
                int indexB = colA * b->colCount + colB;
                // Get the dot product
                dot += a->data[indexA] * b->data[indexB];
            }
            ret[rowA * colCount + colB] = dot;
        }
    }

    // Set the return matrix to the appropriate data
    data->result.colCount = colCount;
    data->result.rowCount = rowCount;
    pthread_exit(NULL);
}

void printMatrix(Matrix m) {
    printf("[");
    for (int i = 0; i < m.rowCount; i++) {
        for (int j = 0; j < m.colCount; j++) {
            printf("%d, ", m.data[i * m.colCount + j]);
        }
        if (i < m.rowCount - 1) {printf("\n");}
    }
    printf("]\n");
}

#define MAX_THREADS 3

int main() {
    // Print the initial matrices
    printf("neo:\n");
    printMatrix(neo);
    printf("morpheus:\n");
    printMatrix(morpheus);
    printf("trinity:\n");
    printMatrix(trinity);
    printf("\n------------------------------------------\n\n");

    int ret1[neo.rowCount * neo.colCount];
    int ret2[neo.rowCount * neo.colCount];
    int ret3[neo.rowCount * trinity.colCount];
    memset(ret1, 0, neo.rowCount * neo.colCount);
    memset(ret2, 0, neo.rowCount * neo.colCount);
    memset(ret3, 0, neo.rowCount * trinity.colCount);
    ThreadData addData;
    addData.matrixA = &neo;
    addData.matrixB = &morpheus;
    addData.result.data = ret1;
    ThreadData subData = addData;
    subData.result.data = ret2;
    ThreadData multData = addData;
    multData.matrixB = &trinity;
    multData.result.data = ret3;

    // Create the threads
    pthread_t my_thread[MAX_THREADS];
    if (pthread_create(&my_thread[0], NULL, add, &addData))  {
        printf("Error creating the add thread!\n");
        return 1;
    }
    if (pthread_create(&my_thread[1], NULL, sub, &subData))  {
        printf("Error creating the sub thread!\n");
        return 1;
    }
    if (pthread_create(&my_thread[2], NULL, multiply, &multData))  {
        printf("Error creating the multiply thread!\n");
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
    if (pthread_join(my_thread[2], NULL)) { // Wait for the max thread to finish
        printf("Error joining the getMax thread!\n");
        return 1;
    }

    // Print the results!
    printf("neo + morpheus:\n");
    printMatrix(addData.result);
    printf("neo - morpheus:\n");
    printMatrix(subData.result);
    printf("neo * trinity:\n");
    printMatrix(multData.result);
    return 0;

    return 0;
}
