#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

bool useCounts = true, testing = false;
int n, r1, r2, range, r, currentMaxIndex = -1, currentMinIndex = -1, nn;
int *nums, *counts;
int array[11] = {10, 10, 24, 31, 41, 50, 50, 59, 73, 74, 74};
double times[7] = {0};
int opCounts[7] = {0};
double avgtime[7] = {0};
float memory;

// Prototypes for operations
int binarySearch(int v); // Return -1 if not there, or index of nums for the first one
int find(int v); // Find instances of a value
int add(int v); // Add value to some integer
int delete(int v); // Delete some value
int succ(int v); // Value after it in index
int pred(int v); // Value before it in index
int nmin(); // Minimum value in array
int nmax(); // Max value in array
int comp_int(const void *a, const void *b);

int binary_search(int value);

void runMethod(int n, int r1, int r2, bool useCounts) {
    const int nops = 7;
    int tops = nops * 0.1 * n;
    int count = 0;
    memory = useCounts ? (range * sizeof(int) / 1000000.0) : (nn * (sizeof(int)) / 1000000.0);
    r = useCounts ? (rand() % range - 1) : (rand() % nn);


    if (useCounts){
        printf("Using counts\n");
        // ./file 100m 1 100k
        // Generate n numbers and increment the frequency in the range
        for (int i = 0; i < n; i++){
            counts[r] += 1;
        }
    } else {
        printf("Using nums\n");
        // Generate 1.1 * n random numbers and sort them
        for (int i = 0; i < nn; i++) {
            nums[i] = rand() % range + r1;
        }
        qsort(nums, nn, sizeof(int), comp_int);
        // Change every 10th number to -1
        int tmp = 0, expected = n * 0.1;
        for (int i = 9; i < nn; i += 11) {
            if (tmp == expected) break;
            // Do not change if the nums before or after are equal to it (duplicate)
            if ((i > 0 && nums[i - 1] == nums[i]) || (i < nn - 1 && nums[i + 1] == nums[i])) {
                int j = i;
                while (j > 0 && (nums[j - 1] == nums[i] || nums[j - 1] == -1)) {
                    j--;
                }
                nums[j] = -1;
                tmp++;

                continue;
            }
            nums[i] = -1;
            tmp++;
        }
        printf("Current count of -1 in array: %d \n", tmp);
        printf("Expected amount of -1 in array: %f \n", n * 0.1);

//        for (int i = 0; i < nn; i++) {
//            if(nums[i] == -1){
//                printf("\n");
//            }
//            printf("%d ", nums[i]);
//        }
//        printf("\n");
//
    }

    // At this point in time I have an array of n numbers with every 10th number being -1
    // {1, 2, 3, 4, 5, 6, 7, 8, 9, -1, 10, 11, 12, 13, 13, 13, 16, 17, 18, 19, -1, 20}

    clock_t start, end;
    while (count++ < tops) {
        int op = rand() % nops;
        start = clock();
        switch (op) {
            case 0: // find
                find(r);
                break;
            case 1: // add
                add(r);
                break;
            case 2: // delete
                delete(r);
                break;
            case 3: // succ
                succ(r);
                break;
            case 4: // pred
                pred(r);
                break;
            case 5: // nmin
                nmin();
                break;
            case 6: // nmax
                nmax();
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
        printf("%-10s \t %-13.6d \t %lf \t %.10e \n", operations[i], opCounts[i], times[i], (times[i] / opCounts[i]));
    }



}

void test() {
    printf("Doing test run");
//    n = abs(n);
//    int size = sizeof(array) / sizeof(array[0]);
//    qsort(array, size, sizeof(int), comp_int);
//
//    printf("Numbers: ");
//    for (int i = 0; i < size; i++)
//    {
//        printf("%d ", array[i]);
//    }
////    printf(": min %d : max %d\n", nmin(), nmax());
//
//    for (int i = 0; i < n; i++)
//    {
//        int target = array[i];
//        printf("%d\n", target);
//
//    }
}

void drive() {
    runMethod(n, r1, r2, useCounts);
    if (useCounts) {
        free(counts);
    } else {
        free(nums);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: <PROGRAM NAME> n r1 r2\n");
        return -1;
    }
    n = atoi(argv[1]);
    r1 = atoi(argv[2]);
    r2 = atoi(argv[3]);
    nn = n * 1.1;



//    srand(time(NULL));

    range = r2 - r1 + 1; // r1 = 3, r2 = 10, range = 8
    useCounts = (range < n) ? true : false;

    // Allocate memory for counts and initialize it to zero
    if (useCounts) {
        counts = (int *) malloc(range * sizeof(int));
    } else {
        nums = (int *) malloc(nn * sizeof(int));
    }

//    if (n < 0)
//        testing = true;
//        test();
    drive();
    return 0;
}

int find(int v) {
    if (useCounts) {
        return counts[v - r1];
    } else {
        int amount = 1;
        int index = binary_search(v);
        if (index != -1) {
            // Check for duplicates to the right
            while (index < nn - 1 && nums[index + 1] == v) {
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
        counts[v - r1]++;
        return 1;
    }
    int index = binary_search(v);
    index = abs(index);

    int end = index;
    while (nums[end] != -1 && end < nn) end++; // Search right
    if (end == nn) {
        while (nums[end] != -1 && end > 0) end--;
        
        for (int i = end; i < index; i++)
            nums[i] = nums[i + 1];
    }
    else // Move right
        for (int i = end; i > index; i--)
            nums[i] = nums[i - 1];
    nums[index] = v;
    return 1;

}
int delete(int v) {

    if (useCounts) {
        if (counts[v - r1] > 0) {
            counts[v - r1]--;
        }
        return 1;
    } else {
        int index = binary_search(v);
        if (index >= 0) {
            nums[index] = -1;
            return 1;
        }
        return 0;
    }
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
        int index = binary_search(v);
        if (index != -1) {
            int successor = index + 1;
            while (successor < n && (nums[successor] == -1 || nums[successor] == v)) {
                successor++;
            }
            while (successor < n && successor > 0 && nums[successor - 1] == nums[successor]) {
                successor++;
            }
            if (successor < n) {
                return nums[successor];
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
        int index = binary_search(v);
        if (index != -1) {
            int predecessor = index - 1;
            while (predecessor >= 0 && (nums[predecessor] == -1 || nums[predecessor] == v)) {
                predecessor--;
            }
            while (predecessor >= 0 && predecessor < n - 1 && nums[predecessor] == nums[predecessor + 1]) {
                predecessor--;
            }
            if (predecessor >= 0) {
                return nums[predecessor];
            }
        }
    }
    return -1; // Return -1 if predecessor not found
}

int nmin() {
    if (useCounts) {
        return counts[0];
    } else {
//        if (testing){
//            return array[0];
//        }
        int index = 0;
        while (nums[index] == -1)
            index++;
        return nums[index];
    }
}

int nmax() {
    if (useCounts) {
        int index = range -1;
        return counts[index];
    } else {
//        if (testing) {
//            int size = sizeof(array) / sizeof(array[0]);
//            int index = size - 1;
//            return array[index];
//        }
        int index = nn - 1;
        while (nums[index] == -1)
            index--;

        return nums[index];
    }
}

int comp_int(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

int binary_search(int value) {
    int lower = 0, upper = nn - 1;
    int tmp = 0;
    bool empty = false;
    while (lower <= upper) {
        int middle = (upper + lower) / 2;
        while (nums[middle] == -1) {
            middle--;
        }
        if (nums[middle] == value) {
            while (middle > 0 && nums[middle - 1] == value) {
                middle--;
            }
            return middle;
        }
        else if (nums[middle] < value) {
            while(nums[middle] == -1 || nums[middle] < value)
                middle++;
            // TODO: This is latest implementation
            if (nums[middle] > value)
                return -middle;
            // ----

            lower = middle + 1;

        } else {
            upper = middle - 1;
        }
    }
    return -1;
}