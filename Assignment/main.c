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
int final_array[3] = {10, 50, 74};

double times[7] = {0}, avgtime[7] = {0};
int opCounts[7] = {0};
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
void print_array();
void test();
void drive();


int gen_rand()
{
    return RAND_MAX == 0x7fffffff ? rand() : rand() << 16 | rand() << 1 | rand() & 1;
}

void runMethod() {
    int tops = (NOPS * 0.1 * n), count = 0;
    memory = useCounts ? (range * sizeof(int) / 1000000.0) : (nn * (sizeof(int)) / 1000000.0);

    if (useCounts){
        printf("Using counts\n");
        // ./file 100m 1 100k
        // Generate n numbers and increment the frequency in the range
        for (int i = 0; i < n; i++){
            counts[gen_rand() % range - 1] += 1;
        }
    } else {
        printf("Using nums\n");
        // Generate 1.1 * n random numbers and sort them
        for (int i = 0; i < nn; i++) {
            nums[i] = gen_rand() % range + r1;
        }
        qsort(nums, nn, sizeof(int), comp_int);
        // Change every 10th number to -1
        int added = 0, expected = n * 0.1;
        for (int i = 9; i < nn; i += 11) {
            if (added == expected) break;
            // Do not change if the nums before or after are equal to it (duplicate)
            int vv = nums[i], ii = i;
            while (nums[ii] == vv) {
                ii++;
            }
            int distance = ii - i;
            if (distance > 1) { // actual duplicates not just a single one
                int needed = distance / 11; // how many we missed because of duplicates
                if (needed == 0) needed = 1; // we landed in duplicates, but we cant put one there, but once we get to the end of the duplicates, we can put it. need at least one where duplicates are
                for (int j=0; j<needed; j++) { // add the amount needed at end of duplicates, going backwards
                    nums[i-j]=-1;
                    added++;
                }
            } else { // no duplicates, safe to replace
                nums[i] = -1;
                added++;
            }

        }

    }

    clock_t start, end;
    while (count++ < tops) {
        int r = gen_rand() % range + r1;
        int op = gen_rand() % NOPS;
        start = clock();
        switch (op) {
            case 0:find(r);break;
            case 1:add(r);break;
            case 2:delete(r);break;
            case 3:succ(r);break;
            case 4:pred(r);break;
            case 5:nmin();break;
            case 6:nmax();break;
        }
        end = clock();
        double secs = ((double) end - (double) start) / CLOCKS_PER_SEC;
        opCounts[op]++; times[op] += secs;
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
    memory = (11 * (sizeof(int)) / 1000000.0);
    printf("\nn = %d, r1 = %d, r2 = %d, Memory used = %f\n", 10, r1, r2, memory);
    print_array();

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

        printf("find %d %d : delete %d %d:  find %d %d : delete %d %d : add %d %d : find %d %d : succ %d %d: pred %d %d \n",
                target, find_result, target, delete_num, target, find_result_after, target, delete_num_after,
                target, add_num, target, last_find, target, getting_succ, target, getting_pred);
    }
    print_array();
    for (int i = 0; i < 3; i++) {
        int target = final_array[i];
        int add_to_array = add(target);
        int find_new = find(target);
        printf("add %d %d : find %d %d\n", target, add_to_array, target, find_new);
    }
    print_array();


}

void drive() {
    runMethod();
    if (useCounts) { free(counts);
    } else { free(nums);}
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

//    sgen_rand(time(NULL));

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

        int amount = 0;
        int index = binary_search(v);
        if (index >=  0) {
            // Check for duplicates to the right
            while (index < opn && oparray[index] == v) {
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
    if (oparray[index] == -1) {
        oparray[index] = v;
        return 1;
    }
    int end = index;
    int a, b;
    a = b = end;
    while (a < opn && oparray[a] != -1) a++; // Search right
    while (b > 0 && oparray[b] != -1) b--; // Search left

    bool foundl = (b >= 0 && oparray[b] == -1);
    bool foundr = (a < opn && oparray[a] == -1);
    bool leftbetter = (index - b) <= (a - index);

    if (foundl && leftbetter) {
        end = b;
        for (int i = end; i < index; i++) {
            oparray[i] = oparray[i + 1];
        }
    } else if (foundr) {
        end = a;
        for (int i = end; i > index; i--) {
            oparray[i] = oparray[i - 1];
        }
    }
    else {
        return -1;
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

        if (index >= 0) {
            oparray[index] = -1;
            return 1;
        }
        return 0;
    }
}

int succ(int v) {
    if (useCounts) {
        int i = v - r1 + 1;
        if (i > range - 1)
            return -1;
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
        int i = v - r1 - 1;
        if (i < 0)
            return -1;
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
        int *oparray = (testing) ? array : nums;
        int opn = (testing) ? 11 : nn;
        int index = 0;
        while (oparray[index] == -1 && index < opn)
            index++;
        return oparray[index];
    }
}

int nmax() {
    if (useCounts) {
        int index = range -1;
        return counts[index];
    } else {
        int *oparray = (testing) ? array : nums;
        int opn = (testing) ? 11 : nn;
        int index = opn - 1;
        while (oparray[index] == -1 && index > 0)
            index--;
        return oparray[index];
    }
}

int comp_int(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

void print_array(){
    int size = sizeof(array) / sizeof(array[0]);
    printf("Numbers: ");
    for (int i = 0; i < size; i++) {
        if (array[i] == -1)
            continue;
        printf("%d ", array[i]);
    }
    printf(": min %d : max %d\n", nmin(), nmax());
}

int binary_search(int value) {
    int *oparray = (testing) ? array : nums;
    int opn = (testing) ? n : nn;
    int lower = 0, upper = opn - 1;
    int middle;
    int m1, m2;
    while (lower <= upper) {

        middle = (upper + lower) / 2;

        if (oparray[middle] == -1) {
            m1 = m2 = middle;
            while(m1>lower && oparray[m1] == -1) {
                m1--;
            }
            while(m2<upper && oparray[m2] == -1) {
                m2++;
            }

            if (oparray[m1] == -1 && oparray[m2] == -1)
                return -m1;
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
                            lower = m2 + 1; // now can move it,for v=6: [m1=3] -1 -1 [m2=5] 6
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
        } else {
            upper = middle - 1; // good
        }
    }
    if (upper < lower) {
        return upper;
    }
    return -1;
}
