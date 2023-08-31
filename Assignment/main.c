#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define NOPS 7

bool useCounts = true, testing = false;
int n, r1, r2, range, currentMaxIndex = -1, currentMinIndex = -1, nn;
int *nums, *counts;
int array[11] = {10, 10, 24, 31, 41, 50, 50, 59, 73, 74, 74};
int testnum[8] = {10, 24, 31, 41, 50, 59, 73, 74};

double times[7] = {0};
int opCounts[7] = {0};
double avgtime[7] = {0};
float memory;

// Prototypes for operations
int find(int v); // Find instances of a value
int add(int v); // Add value to some integer
int delete(int v); // Delete some value
int succ(int v); // Value after it in index
int pred(int v); // Value before it in index
int nmin(); // Minimum value in array
int nmax(); // Max value in array
int comp_int(const void *a, const void *b);
int binary_search(int value);
void test();
void drive();

void runMethod() {
    int tops = NOPS * 0.1 * n;
    int count = 0;
    memory = useCounts ? (range * sizeof(int) / 1000000.0) : (nn * (sizeof(int)) / 1000000.0);

    if (useCounts){
        printf("Using counts\n");
        // ./file 100m 1 100k
        // Generate n numbers and increment the frequency in the range
        for (int i = 0; i < n; i++){
            counts[rand() % range - 1] += 1;
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
            int vv = nums[i];
            int ii = i;
            while (nums[ii] != vv) {
                ii++;
            }
            int distance = ii - i;
            if (distance > 1) { // 1 1 2
                int d = distance / 11;
                if (d == 0) d = 1;
                for (int j=1; j<=d; j++) {
                    nums[i-j]=-1;
                    tmp++;
                }
            } else {
                nums[i] = -1;
                tmp++;
            }

//            // 1111111111122222x222222222222x22222222222x222225555555555555555
//            if ((i > 0 && nums[i - 1] == nums[i]) || (i < nn - 1 && nums[i + 1] == nums[i])) {
//                int j = i;
//                while (j > 0 && (nums[j - 1] == nums[i] || nums[j - 1] == -1)) {
//                    j--;
//                }
//                nums[j] = -1;
//                tmp++;
//
//                continue;
//            }
            //nums[i] = -1;
            //tmp++;
        }
        printf("Current count of -1 in array: %d \n", tmp);
        printf("Expected amount of -1 in array: %f \n", n * 0.1);

        for (int i = 109999000; i < nn; i++) {
            if(nums[i] == -1){
                printf("\n");
            }
            printf("%d ", nums[i]);
        }
        printf("\n");

    }

    // At this point in time I have an array of n numbers with every 10th number being -1
    // {1, 2, 3, 4, 5, 6, 7, 8, 9, -1, 10, 11, 12, 13, 13, 13, 16, 17, 18, 19, -1, 20}

    clock_t start, end;
    while (count++ < tops) {
        int r = rand() % range + r1;
        int op = rand() % NOPS;
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
    for (int i = 0; i < NOPS; i++) {
        printf("%-10s \t %-13.6d \t %lf \t %.10e \n", operations[i], opCounts[i], times[i], (times[i] / opCounts[i]));
    }



}

void test() {
    printf("Doing test run\n");
    n = abs(n);
    int size = sizeof(array) / sizeof(array[0]);
    memory = (11 * (sizeof(int)) / 1000000.0);
    printf("\nn = %d, r1 = %d, r2 = %d, Memory used = %f\n", n, r1, r2, memory);
    printf("Numbers: ");
    for (int i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf(": min %d : max %d\n", nmin(), nmax());
    printf("\n");

    /*
     * TODO: Start operations here
     */
    for (int i = 0; i < 8; i++)
    {
        int target = testnum[i];
        int find_result = find(target);
        int delete_num = delete(target);
        int find_result_after = find(target);
        int delete_num_after = delete(target);
        int add_num = add(target);
        int last_find = find(target);
        int getting_succ = succ(target);
        int getting_pred = pred(target);

//        printf("succ %d %d ", target, getting_succ);
        printf("Find %d %d: Delete %d %d: Find %d %d: Delete %d %d: Add %d %d: Find %d %d: succ %d %d: pred %d %d \n",
                target, find_result, target, delete_num, target, find_result_after, target, delete_num_after,
                target, add_num, target, last_find, target, getting_succ, target, getting_pred);


    }
}

void drive() {
    runMethod();
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


    if (n < 0) {
        testing = true;
        n = 11;
        test();
        return 0;
    }
    drive();
    return 0;
}

int find(int v) {
    if (useCounts) {
        return counts[v - r1];
    } else {

        int *oparray = (testing) ? array : nums;
        int opn = (testing) ? n : nn;

        int amount = 1;
        int index = binary_search(v);
        //printf("BINARY SEARCH FIND \n");

        if (index >=  0) {
            // Check for duplicates to the right
            //printf("index + 1 = %d\n", index+1);
            while (index < opn - 1 && oparray[index + 1] == v) {
                index++;
                amount++;
            }
            return amount;
        }
        return 0;
    }
}

int add(int v) {
    if (useCounts) {
        counts[v - r1]++;
        return 1;
    }

    int *oparray = (testing) ? array : nums;
    int opn = (testing) ? n : nn;

    int index = binary_search(v);

    index = abs(index);
    int end = index;
    while (oparray[end] != -1 && end < opn) end++; // Search right
    //printf("end = %d, index = %d\n", end, index);

    if (end == opn) {
        while (oparray[end] != -1 && end > 0) end--;

        for (int i = end; i < index; i++)
            oparray[i] = oparray[i + 1];
    }
    else {// Move right
        //printf("MOVING RIGHT: %d\n", end-index);
        for (int i = end; i > index; i--)
            oparray[i] = oparray[i - 1];
    }
    oparray[index] = v;
    return 1;

}
int delete(int v) {
    if (useCounts) {
        if (counts[v - r1] > 0) {
            counts[v - r1]--;
        }
        return 1;
    } else {
        int *oparray = (testing) ? array : nums;
        int index = binary_search(v);
        //printf("BINARY SEARCH DELETE %d,%d\n", v, index);
        if (index >= 0) {
            //printf("d: %d\n", index);
            oparray[index] = -1;
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
        int *oparray = (testing) ? array : nums;
        int opn = (testing) ? n : nn;

        int index = binary_search(v);
        //printf("BINARY SEARCH SUCC (%d>%d) \n", v, index);
        if (index >= 0) {
            int successor = index + 1;
            while (successor < opn && (oparray[successor] == -1 || oparray[successor] == v)) {
                successor++;
            }
            while (successor < opn && successor > 0 && oparray[successor - 1] == oparray[successor]) {
                successor++;
            }
            if (successor < opn) {
                return oparray[successor];
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
        int *oparray = (testing) ? array : nums;
        int opn = (testing) ? n : nn;

        int index = binary_search(v);
        //printf("BINARY SEARCH PRED (%d>%d)\n", v, index);


        if (index >= 0) {
            int predecessor = index - 1;
            while (predecessor >= 0 && (oparray[predecessor] == -1 || oparray[predecessor] == v)) {
                predecessor--;
            }
            while (predecessor >= 0 && predecessor < opn - 1 && oparray[predecessor + 1] == oparray[predecessor]) {
                predecessor--;
            }
            if (predecessor >= 0) {
                return oparray[predecessor];
            }
        }
    }
    return -1; // Return -1 if predecessor not found
}


int nmin() {
    if (useCounts) {
        return counts[0];
    } else {

        if (testing){
            return array[0];
        }
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
        if (testing) {
            int size = sizeof(array) / sizeof(array[0]);
            int index = size - 1;
            return array[index];
        }
        int index = nn - 1;
        while (nums[index] == -1)
            index--;

        return nums[index];
    }
}

int comp_int(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

int binary_search(int value) { // Nope
    int *oparray = (testing) ? array : nums;
    int opn = (testing) ? n : nn;
    int lower = 0, upper = opn - 1;
    int middle;
    int m1, m2;
    // bool empty = false;
    while (lower <= upper) {

        middle = (upper + lower) / 2;
        //printf("middle: %d, lower: %d, upper: %d\n", middle, lower, upper);
//        if (lower >= upper && oparray[middle] != value) {
//            if (value < oparray[middle])
//                while (oparray[middle] > value || oparray[middle] != -1)
//                    middle--;
//            return -middle;
//        }

        if (oparray[middle] == -1) {
            m1 = m2 = middle;
            //printf("m1: %d, m2: %d\n", m1 ,m2);
            while(m1>lower && oparray[m1] == -1) {
                m1--;
            }
            while(m2<upper && oparray[m2] == -1) {
                m2++;
            }
            //printf("m1: %d, m2: %d\n", m1 ,m2);

            if (oparray[m1] == -1 && oparray[m2] == -1)
                return -m1;
            //printf("m1: %d, m2: %d\n", m1 ,m2);
            if (oparray[m1] == value) {
                return m1;
            }
            else if (oparray[m2] == value) {
                return m2;
            }
            else {
                if (m1 >= 0) {
                    if (oparray[m1] < value) {
                        if (m2 < opn && oparray[m2] < value) {
                            lower = m2 + 1; // now can move it
                        } else {
                            return -middle;
                        }
                    }
                    else if (oparray[m1] > value) {
                        upper = m1 - 1; // good
                    }
                }
                if (m2 < opn) {
                    if (oparray[m2] < value) {
                        lower = m2 + 1; // good
                    }
                    else if (oparray[m2] > value) {
                        if (m1 > 0 && oparray[m1] > value) {
                            upper = m1 - 1; // good
                        } else {
                            return -middle;
                        }
                    }
                }
            }
            continue;
        }

        if (oparray[middle] == value) {
            return middle;
        }
        else if (oparray[middle] < value) {
            lower = middle + 1; // good
//            while (oparray[lower] == -1)
//                lower++;
        } else {
            upper = middle - 1; // good
//            while (oparray[upper] == -1)
//                upper--;
        }
    }
    if (upper < lower/*&& oparray[lower] != -1*/) {
        return upper;
    }
//    printf("death %d - %d\n", lower, upper);
    return -1;
}
