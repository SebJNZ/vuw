// Import necessary header files...
#include <stdio.h>
#include <string.h>

#define SIZE 8 // Array Size

// Closest thing we have to tuples...
typedef struct {
    int min;
    int max;
} Size;

// Prints all numbers in an integer array.
void printArray(const int array[], int size) {
    printf("[");
    for (int i = 0; i < size - 1; i++) {
        printf("%d, ", array[i]);
    }
    printf("%d]\n", array[size-1]);
}

// Ask the user to enter numbers for the array.
void createArray(int array[]) {
    for (int i = 0; i < SIZE; i++) {
        int input;
        printf("Enter s[%d]: ", i);
        scanf("%d", &input);
        array[i] = input;
    }
}

Size MinMax(int size, int array[]) {
    // Check if numbers is less than 2, or not divisable by 2.
    if (size < 2 || size % 2 != 0) { Size size = {0, 0}; return size; }

    // Uncomment me to see what the array looks like at each step.
    // printArray(array, size);

    // Base Case,  we will return the smaller value
    if (size == 2) {
        Size minMax;
        if (array[0] < array[1]) {
            minMax.min = array[0];
            minMax.max = array[1];
        } else {
            minMax.min = array[1];
            minMax.max = array[0];
        }
        return minMax;
    } else {
        // Get the two halfs of the arrays.
        int half = size / 2;
        int firstHalf[half];
        int secondHalf[half];

        memcpy(firstHalf, array, sizeof(int) * half); // copy first half of array
        memcpy(secondHalf, array+half, sizeof(int) * half); // copy second half of array

        Size first = MinMax(half, firstHalf);
        Size second = MinMax(half, secondHalf);
        Size final;

        final.min = first.min < second.min ? first.min : second.min;
        final.max = first.max > second.max ? first.max : second.max;
        return final;
    }
}

int main(void) {
    // Create an array to store the numbers in.
    int s[SIZE];
    createArray(s); // Ask the numbers
    Size output = MinMax(SIZE, s);
    printf("Min: %d\nMax: %d\n", output.min, output.max);

    return 0;
}
