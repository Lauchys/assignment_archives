#include <stdio.h>
#include <stdbool.h>

bool useCounts = false;
int n = 10; // Number of elements in the array
int r1 = 1; // Range lower bound
int r2 = 200; // Range upper bound
int nn = 10 * 1.1; // Number of elements considering duplicates
int range;
int *counts;

int values[100]; // Array to hold values
int currentMaxIndex = -1, currentMinIndex = -1;

// Function prototypes
int add(int v);
void printArray();

int main() {
    range = r2 - r1 + 1;
    // Initialize the array with sorted numbers and placeholders
    for (int i = 0; i < nn; i++) {
        if (i % 4 == 3) {
            values[i] = -1; // Placeholder for every 4th number
        } else {
            values[i] = (r1 + (i * (r2 - r1)) / nn); // Sorted numbers
        }
    }

    printf("Original Array:\n");
    printArray();

    // Adding a new value
    int newValue = 100;
    printf("\nAdding %d...\n", newValue);
    add(newValue);

    printf("\nUpdated Array:\n");
    printArray();

    return 0;
}

int add(int v) {
    if (useCounts) {
        if (counts[v - r1 + 1] < range) {
            counts[v - r1 + 1]++;
            if (v - r1 + 1 > currentMaxIndex) {
                currentMaxIndex = v - r1 + 1;
            }
            if (v - r1 < currentMinIndex || currentMinIndex == -1) {
                currentMinIndex = v - r1;
            }
        }
    } int index = -1;

    // Find the correct index to insert the new value
    for (int i = 0; i < nn; i++) {
        if (values[i] == -1 || values[i] >= v) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        // If the value to insert is not -1, we need to replace the placeholder -1
        if (values[index] != -1) {
            // Shift numbers to the right to make space for the new value
            for (int i = nn - 2; i >= index; i--) {
                values[i + 1] = values[i];
            }
        }

        // Insert the new value at the correct index
        values[index] = v;
    }

    return 0;
}

void printArray() {
    for (int i = 0; i < nn; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");
}
