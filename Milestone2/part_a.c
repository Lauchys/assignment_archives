#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MAXLEN 100

int total_guesses, letters, used_guesses;
char *guess, letters_used[26];
int letterCount[letters] = {0};
bool usedLetters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                        'i', 'j', 'k', 'l', 'm', 'n', 'o','p',
                        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

char *arr[25] = {"count", "suit", "medal", "memory", "research", "roof",
                 "fibre", "crack", "load", "roll", "scale","rose","cook",
                 "children","sand","body","church","air","price","reward",
                 "protest","pest","flight","cable", "stew"};

void wordprint();
void makeGuess(char v);
void askGuess();
void checkGuess(char v);
void startGame();
int initFile();

int numWords = sizeof(arr) / sizeof(arr[0]);

int main(int argc, char *argv[])
{
    letters = atoi(argv[1]);
    total_guesses = atoi(argv[2]);
    used_guesses = 0;
    guess = (char *) malloc(total_guesses * sizeof(char));
    startGame();

    return 0;
}

void startGame()
{
    initFile();
//    while (used_guesses < total_guesses) {
//        wordprint();
//        printf(" Enter letter: ");
//        scanf("%s", guess);
//        makeGuess(*guess);
//    }

}
int initFile()
{

    FILE *fptr = fopen("dictionary.txt", "r");
    if (!fptr)
    {
        printf("Could not open file");
        return 1;
    }
    char buffer[128];
    fscanf(fptr, "%127s", buffer);
    FILE *gameptr = (FILE*)malloc(letters * sizeof(FILE));
    char new_filename[128];
    sprintf(new_filename, "game_%d.txt", letters);
    gameptr = fopen(new_filename, "w");

    rewind(fptr);
    while (fscanf(fptr, "%127s", buffer) == 1)
    {
        int len = strlen(buffer);
        if (len == 6)
            for (int i; i < len; i++)
                if (buffer[i] )
            fprintf(gameptr, "%s\n", buffer);
    }
    fclose(gameptr);
    return true;


}

//void wordprint()
//{
//    for (int i = 0; i < letters; i++)
//        printf("_");
//}

//void load_file(char *fname)
//{
//    FILE *in = fopen(fname, "r");
//    char word[MAX_WORD];
//    while (fscanf(in, "%s", word) == 1)
//        word_count++;
//
//    fclose(in);
//    words = (char **)malloc(word_count * sizeof(char *));
//    in = fopen(fname, "r");
//    int index = 0;
//    while (fscanf(in, "%s", word) == 1)
//    {words[index] = (char *)malloc(strlen(word) * sizeof(char) + 1);
//        strcpy(words[index++], word);
//    }
//    fclose(in);
//}

void checkGuess(char v)
{
    printf("WIP");
    return;
}

//void makeGuess(char v)
//{
//    int val = v - 65; // a = 0
//    if (usedLetters[val] == 1)
//    {
//        printf("%c already used\n", v);
//        return;
//    }
//    usedLetters[val] = true;
//    strcat(letters_used, &v);
//    used_guesses++;
//    printf("Guess %d / %d, Words Left %d, Letter used = %s\n", used_guesses, total_guesses, 0, letters_used);
//}