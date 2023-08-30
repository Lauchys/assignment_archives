#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

bool useCounts = true, testing = false;
int n, r1, r2, range, r, currentMaxIndex = -1, currentMinIndex = -1, nn;
int *values, *counts;
double times[7] = {0};
int opCounts[7] = {0};
double avgtime[7] = {0};
float memory;

// Prototypes for operations
int binarySearch(int v); // Return -1 if not there, or index of values for the first one
int find(int v); // Find instances of a value
int add(int v); // Add value to some integer
int delete(int v); // Delete some value
int succ(int v); // Value after it in index
int pred(int v); // Value before it in index
int nmin(); // Minimum value in array
int nmax(); // Max value in array
int comp_int(const void *a, const void *b);

int binary_search(int array[], int n, int value);

void runMethod(int n, int r1, int r2, bool useCounts) {
    const int nops = 7;
    int tops = nops * 0.1 * n;
    int count = 0;
    memory = useCounts ? (range * sizeof(int) / 1000000.0) : (nn * (sizeof(int)) / 1000000.0);
    r = useCounts ? (rand() % range + r1) : (rand() % nn);


    if (useCounts){
        // Generate n numbers and increment the frequency in the range
        for (int i = 0; i < n; i++){
            int index = rand() % range + 1;
            counts[index] += 1;
        }
    }
    else{
        // Generate 1.1 * n random numbers and sort them
        for (int i = 0; i < nn; i++) {
            values[i] = rand() % range + r1;
        }
        qsort(values, nn, sizeof(int), comp_int);

        // Change every 10th number to -1
        for (int i = 9; i < nn; i += 10) {
            // Do not change if the values before or after are equal to it (duplicate)
            if ((i > 0 && values[i - 1] == values[i]) || (i < nn - 1 && values[i + 1] == values[i])) {
                continue;
            }
            values[i] = -1;
        }
    }

    // At this point in time I have an array of n numbers with every 10th number being -1
    // {1, 2, 3, 4, 5, 6, 7, 8, 9, -1, 10, 11, 12, 13, 13, 13, 16, 17, 18, 19, -1, 20}

    clock_t start, end, s, e;
    double sec = ((double) e - (double) s) / CLOCKS_PER_SEC;
    while (count++ < tops) {
        int op = rand() % nops;
        start = clock();
        switch (op) {
            case 0: // find
                s = clock();
                find(r);
                e = clock();
                sec = ((double) e - (double) s) / CLOCKS_PER_SEC;
                avgtime[op] += sec;
                break;

            case 1: // add
                s = clock();
                add(r);

                e = clock();
                sec = ((double) e - (double) s) / CLOCKS_PER_SEC;
                avgtime[op] += sec;
                break;

            case 2: // delete
                s = clock();
                delete(r);
                e = clock();
                sec = ((double) e - (double) s) / CLOCKS_PER_SEC;
                avgtime[op] += sec;
                break;

            case 3: // succ
                s = clock();
                succ(r);
                e = clock();
                sec = ((double) e - (double) s) / CLOCKS_PER_SEC;
                avgtime[op] += sec;
                break;
            case 4: // pred
                s = clock();
                pred(r);
                e = clock();
                sec = ((double) e - (double) s) / CLOCKS_PER_SEC;
                avgtime[op] += sec;
                break;
            case 5: // nmin
                s = clock();
                nmin();
                e = clock();
                sec = ((double) e - (double) s) / CLOCKS_PER_SEC;
                avgtime[op] += sec;
                break;

            case 6: // nmax
                s = clock();
                nmax();
                e = clock();
                sec = ((double) e - (double) s) / CLOCKS_PER_SEC;
                avgtime[op] += sec;
                break;

        }

        end = clock();
        double secs = ((double) end - (double) start) / CLOCKS_PER_SEC;
        opCounts[op]++;
        times[op] += secs;
    }
    char *operations[] = {"find", "add", "delete", "succ", "pred", "min", "max"};

    printf("\nn = %d, r1 = %d, r2 = %d, Memory used = %f\n", n, r1, r2, memory);
    printf(" \t\t Op Counts \t Total time \t Avg. Time \n");
    for (int i = 0; i < nops; i++) {
        printf("%-10s \t %-13.6d \t %f \t %.10e \n", operations[i], opCounts[i], times[i], (avgtime[i] / tops));


    }

}


void drive() {
    useCounts = (range < n) ? true : false;
    runMethod(n, r1, r2, useCounts);

    if (useCounts) {
        free(counts);
    } else {
        free(values);
    }
}

int main(int argc, char *argv[]) {
    n = atoi(argv[1]);
    r1 = atoi(argv[2]);
    r2 = atoi(argv[3]);
    nn = n * 1.1;

//    srand(time(NULL));

    range = r2 - r1 + 1; // r1 = 3, r2 = 10, range = 8

    // Allocate memory for counts and initialize it to zero
    counts = (int *) malloc(range * sizeof(int));
    values = (int *) malloc(nn * sizeof(int));

    drive();

    // Free the allocated memory for counts
    return 0;
}


// TODO: Sample run 3
void test() {
}


/**
 *
 * Functions for operations using @param v to @return value
 *
 * TODO: Add the functions for the Array method (for Sample 2)
 *
 */
int find(int v) {
    if (useCounts) {
        return counts[v - r1];
    } else {
        int amount = 1;
        int index = binary_search(values, n, v);
        if (index != -1) {
            // Check for duplicates to the right
            while (index < nn - 1 && values[index + 1] == v) {
                index++;
                amount++;
            }

            return amount;
        }
        return -1;
    }
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
    }
    int index = binary_search(values, nn, r);
    index = abs(index);

    int end = index;
    while (values[end] != -1 && end < n) end++; // Search right
    if (end == n) {
        while (values[end] != -1 && end > 0) end--;
        for (int i = end; i < index; i++)
            values[i] = values[i + 1];
    }
    else // Move right
        for (int i = end; i > index; i--)
            values[i] = values[i - 1];
    values[index] = v;
    return 1;

}



int delete(int v) {
    if (useCounts) {
        if (counts[v - r1] > 0) {
            counts[v - r1]--;
            if (v - r1 == currentMaxIndex && counts[v - r1] == 0) {
                for (int i = currentMaxIndex - 1; i >= 0; i--) {
                    if (counts[i] != 0) {
                        currentMaxIndex = i;
                        break;
                    }
                }
            }
            if (v - r1 == currentMinIndex && counts[v - r1] == 0) {
                for (int i = currentMinIndex + 1; i < range; i++) {
                    if (counts[i] != 0) {
                        currentMinIndex = i;
                        break;
                    }
                }
            }
        }
    } else {
        int index = binary_search(values, n, v);
        values[index] = -1;
    }
    return -1;
}

int succ(int v) {
    if (useCounts) {
        int i = v - r1 + 1;
        while (counts[i] == 0) {
            if (i > range - 1) {
                break;
            }
            i++;
        }
        return i;
    } else {
        int index = binary_search(values, n, v);
        if (index != -1) {
            int successor = index + 1;
            while (successor < n && (values[successor] == -1 || values[successor] == v)) {
                successor++;
            }
            while (successor < n && successor > 0 && values[successor - 1] == values[successor]) {
                successor++;
            }
            if (successor < n) {
                return values[successor];
            }
        }
    }
    return -1; // Return -1 if successor not found
}

int pred(int v) {
    if (useCounts) {
        int i = v - r1 + 1;
        while (counts[i] == 0) {
            if (i < 0) {
                break;
            }
            i--;
        }
        return i;
    } else {
        int index = binary_search(values, n, v);
        if (index != -1) {
            int predecessor = index - 1;
            while (predecessor >= 0 && (values[predecessor] == -1 || values[predecessor] == v)) {
                predecessor--;
            }
            while (predecessor >= 0 && predecessor < n - 1 && values[predecessor] == values[predecessor + 1]) {
                predecessor--;
            }
            if (predecessor >= 0) {
                return values[predecessor];
            }
        }
    }
    return -1; // Return -1 if predecessor not found
}

int nmin() {
    if (useCounts && currentMinIndex != -1) {
        return counts[currentMinIndex];
    } else {
        int index = 0;
        while (values[index] == -1)
            index++;
        return values[index];
    }
}

int nmax() {
    if (useCounts && currentMaxIndex != -1) {
        return counts[currentMaxIndex];
    } else {
        int index = nn - 1;
        while (values[index] == -1)
            index--;

        return values[index];
    }
}

int comp_int(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

int binary_search(int array[], int n, int value) {
    int lower = 0, upper = n - 1;

    while (lower <= upper) {
        int middle = (upper + lower) / 2;
        while (array[middle] == -1) {
            middle--;
        }
        if (array[middle] == value) {
            while (middle > 0 && array[middle - 1] == value) {
                middle--;
            }
            return middle;
        } else if (array[middle] < value) {
            lower = middle + 1;
        } else {
            upper = middle - 1;
        }
    }
    return -1;
}