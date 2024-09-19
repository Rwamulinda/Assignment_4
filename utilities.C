#include <stdio.h>
#include <string.h>
#include "utilities.h"

// Global array to store words and their occurrences
struct Word words[MAX_WORDS];
int word_count = 0;

// Function to add a word to the list (or increment its occurrence if it already exists)
void add_word(const char *word, int line) {
    // Check if the word is already in the list
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            // Word found, increment occurrences and add the line number
            words[i].occurrences++;
            int j = 0;
            while (words[i].lines[j] != 0) j++;  // Find the next empty spot in lines array
            words[i].lines[j] = line;
            return;
        }
    }

    // If the word is not found, add it as a new word
    strcpy(words[word_count].word, word);
    words[word_count].occurrences = 1;
    words[word_count].lines[0] = line;
    words[word_count].lines[1] = 0;  // Null terminate the list of lines
    word_count++;
}

// Function to sort the words array by occurrences or alphabetically if occurrences are equal
void sort_words(struct Word words[], int word_count) {
    struct Word temp;
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (words[i].occurrences < words[j].occurrences ||
                (words[i].occurrences == words[j].occurrences &&
                 strcmp(words[i].word, words[j].word) > 0)) {
                // Swap the words
                temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }
}

// Function to print the words and their occurrences and line numbers
void print_words(const struct Word words[], int word_count) {
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d occurrences, lines: ", words[i].word, words[i].occurrences);
        for (int j = 0; words[i].lines[j] != 0; j++) {
            printf("%d ", words[i].lines[j]);
        }
        printf("\n");
    }
}
