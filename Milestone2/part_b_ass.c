#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILENAME "dictionary.txt"

int sorted_words;
char ***words;// STAR(*) 1 = STRING (LIST O' CHARACTER) -> STAR(*) 2 = LIST OF WORD -> STAR(*) 3 = LIST OF LIST OF WORD
int *word_counts;
int min, max;
int solutions;


// Numbers 0-9, assign a letter to a number, only 1 letter per number
char letters[10] = {'\0'};

// Counts of how many times numbers 0-9 are in use
int counts[10] = {0};

// Letters A-Z, checking if letter is in use
bool in_use[26] = {false};

void read_file();
void assign(int argc, char *argv[], int input_index);

int main(int argc, char *argv[]) {
    read_file();
    assign(argc, argv, 1);
    printf("Solutions = %d\n", solutions);


//    index = length - min;
//    for (int i = 0; i < word_counts[index]; i++) {
//        printf("word_counts[ind][i] = word_counts[%d][%d] = %s\n", index, i, words[index][i]);
//        printf("%s\n", words[index][i]);
//    }

    return 0;
}

void assign(int argc, char *argv[], int input_index)
{
    printf("Input Index = %d \n argc = %d\n", input_index, argc);
    // BASE CASE
    if (input_index == argc) {
        solutions++;
        return;
    }


    // PROCESS EACH WORD IN THE LEVEL
    char *current_input = argv[input_index];
    int word_length = strlen(current_input);
    int word_length_index = word_length - min;
    // 100 -> word_length_index ->
    // words_count[n-min] = length of n letter words
    for (int i = 0; i < word_counts[word_length_index]; i++) {
        // PROCESS EACH LETTER
        int j;
        for (j = 0; j < word_length; j++) {
            // Attempt to assign letter
            char current_digit = current_input[j];
            int current_digit_index = current_digit - '0';
            char current_letter = words[word_length_index][i][j];
//            printf("1 -> j = %d\n", j);

            // IF CANT ASSIGN LETTER -> STOP PROCESSING LETTERS
            // if letter not in use && letters[current_digit_index] == \0 -> can use
            // or letter is in use && letters[current_digit_index] is current letter -> can use

            if ((in_use[current_letter - 'a'] || letters[current_digit_index != '\0']) &&
            (!in_use[current_letter - 'a'] || letters[current_digit_index] != current_letter))
                break;

//            printf(" 2 -> j = %d\n", j);

            letters[current_digit_index] = current_letter;
            counts[current_digit_index] += 1;
            in_use[current_letter - 'a'] = true;
        }
        // if j == word_length -> Successful
        // IF SUCCESSFUL -> RECURSION NEXT LEVEL
        printf("j = %d\n", j);
        if (j == word_length)
        {
            printf("Successful\n");
            assign(argc, argv,  input_index+1);

        }

        // UNDO PROCESSED LETTERS
        for (j = j-1; j != -1; j--) {
            char current_digit = current_input[j];
            int current_digit_index = current_digit - '0';
            char current_letter = words[word_length_index][i][j];
            printf("CURRENT LETTER = %c\n", current_letter);
            counts[current_digit_index] -= 1;
            if (counts[current_digit_index] == 0)
            {
                letters[current_digit_index] = '\0';
                in_use[current_letter - 'a'] = false;
            }
        }


    }
}








void read_file() {
    FILE *fptr = fopen(FILENAME, "r");
    if (!fptr) {
        printf("Could not open file");
        exit(1);
    }
    char buffer[128];
    fscanf(fptr, "%127s", buffer);
    min = strlen(buffer);
    max = min;
    while (fscanf(fptr, "%127s", buffer) == 1) {

        int len = strlen(buffer);
        if (len < min)
            min = len;
        if (len > max)
            max = len;
    }
//    printf("Shortest word length: %d, Longest word length: %d\n", min, max);
    int range = max - min + 1;
    //
    words = (char ***) malloc(range * sizeof(char **));
    word_counts = (int*) malloc(range * sizeof(int));
    for (int i = 0; i < range; i++) {
        int count = 0;
        int length = i + min;
        rewind(fptr);
        while (fscanf(fptr, "%127s", buffer) == 1)
            if (strlen(buffer) == length) {
//                printf("Count = %d   Buffer = %s     Length = %d\n", count, buffer, length);
                count++;
            }
        int wl = strlen(buffer);
        word_counts[i] = count;
        words[i] = (char **) malloc(count * sizeof(char *));

        rewind(fptr);
        int j = 0;
        while (fscanf(fptr, "%127s", buffer) == 1)
            if (strlen(buffer) == length) {
                words[i][j] = strdup(buffer);
                j++;
            }
    }
    fclose(fptr);
}