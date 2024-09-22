#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common_words.h"

#define MAX_LINE_LENGTH 254
#define MAX_WORDS 1200
#define MAX_LINES 20
#define MAX_WORD_LENGTH 50



//  Author: Pauline Uwase
// Date: 22/09/2024
// Indexer Program 


// defining structure  to store each word's details

typedef struct {
    char word[MAX_WORD_LENGTH];
    int frequency;
    int lines[MAX_LINES];
    int line_count;
} WordEntry;


// Function: is_common_word that will check if the  the word is not in common_word list we have(common_words.h has the list of common words)
// ignoring it if it meets with it Returns 1, or  return 0 otherwise.
int is_common_word(const char *word) {
    for (int i = 0; common_words[i] != NULL; i++) {
        if (strcasecmp(word, common_words[i]) == 0) {
            return 1; 
        }
    }
    return 0; 
}

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function: is_valid_character
 //Determines if a character is valid for a word (alphabetic, hyphen, or apostrophe), Restricts the use of hyphens and apostrophes to valid positions.

int is_valid_character(char c, int position, int hyphen_count) {
    if (isalpha(c)) return 1; 
    if (c == '-') {
        return position > 0 && position < MAX_WORD_LENGTH - 1 && hyphen_count == 0; // Allow one hyphen in the middle
    }
    if (c == '\'') {
        return position > 0 && position < MAX_WORD_LENGTH - 1; 
    }
    return 0; 
}
//Function: add_line_number Adds a line number to the list of lines where a word appears, 
 // ensuring no duplicate entries

void add_line_number(int *lines,int *line_count, int new_line) {
    for (int i = 0; i < *line_count; i++) {
        if (lines[i] == new_line) {
            return; // Line number already exist, do nothing
        }
    }
    lines[*line_count] = new_line;
    (*line_count)++; 
}


// Function: parse_words which tokenizes a line of input and updates the word entries array with word frequency
 //and line numbers. Also checks for errors such as too many distinct words.

void parse_words(const char *line, WordEntry *words, int *word_count, int line_number) {
    char token[MAX_WORD_LENGTH];
    int token_index = 0;
    int hyphen_count = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        char c = line[i];
        if (is_valid_character(c, token_index, hyphen_count)) {
            token[token_index++] = c;
            if (c == '-') hyphen_count++;
        } else {
            if (token_index > 0) {
                token[token_index] = '\0'; 
                if (token[token_index - 1] == '-') {
                    token[token_index - 1] = '\0'; // Remove trailing hyphen
                }

                // Remove trailing apostrophe if present
                if (token[token_index - 1] == '\'') {
                    token[token_index - 1] = '\0';
                }

                to_lowercase(token);

                if (!is_common_word(token)) {
                    int found = 0;
                    for (int j = 0; j < *word_count; j++) {
                        if (strcmp(words[j].word, token) == 0) {
                            found = 1;
                            words[j].frequency++;
                            add_line_number(words[j].lines, &words[j].line_count, line_number);
                            break;
                        }
                    }
                    if (!found) {
                        if (*word_count < MAX_WORDS) {
                            strcpy(words[*word_count].word, token);
                            words[*word_count].frequency = 1;
                            words[*word_count].lines[0] = line_number;
                            words[*word_count].line_count = 1;
                            (*word_count)++;
                        } else {
                            fprintf(stderr, "Error: Too many distinct words in the input.\n");
                            exit(1); // Exit the program with an error
                        }
                    }
                }
                token_index = 0; 
                hyphen_count = 0; 
            }
        }
    }

    // Check for last token
    if (token_index > 0) {
        token[token_index] = '\0';
        if (token[token_index - 1] == '-') {
            token[token_index - 1] = '\0'; // Remove the hypen behing the word
        }

        // Remove trailing apostrophe if present
        if (token[token_index - 1] == '\'') {
            token[token_index - 1] = '\0';
        }

        to_lowercase(token);
        if (!is_common_word(token)) {
            int found = 0;
            for (int j = 0; j < *word_count; j++) {
                if (strcmp(words[j].word, token) == 0) {
                    found = 1;
                    words[j].frequency++;
                    add_line_number(words[j].lines, &words[j].line_count, line_number);
                    break;
                }
            }
            if (!found) {
                if (*word_count < MAX_WORDS) {
                    strcpy(words[*word_count].word, token);
                    words[*word_count].frequency = 1;
                    words[*word_count].lines[0] = line_number;
                    words[*word_count].line_count = 1;
                    (*word_count)++;
                } else {
                    fprintf(stderr, "Error: Too many distinct words in the input.\n");
                    exit(1); // Exit the program with an error
                }
            }
        }
    }
}

int compare(const void *a, const void *b) {
    WordEntry *word_a = (WordEntry *)a;
    WordEntry *word_b = (WordEntry *)b;

    if (word_b->frequency != word_a->frequency) {
        return word_b->frequency - word_a->frequency; 
    }
    return strcmp(word_a->word, word_b->word); 
}


// main part to execute the functions
int main() {
    WordEntry words[MAX_WORDS] = {0};
    int word_count = 0;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    while (fgets(line, sizeof(line), stdin)) {
        line_number++;
        parse_words(line, words, &word_count, line_number);
    }

    if (word_count == 0) {
        fprintf(stderr, "No words found\n");
        return 1;
    }

    qsort(words, word_count, sizeof(WordEntry), compare);

    for (int i = 0; i < word_count; i++) {
        printf("%-15s [%d]:", words[i].word, words[i].frequency);
        for (int j = 0; j < words[i].line_count; j++) {
            if (j > 0) printf(", ");
            printf(" %d", words[i].lines[j]);
        }
        printf("\n");
    }

    return 0;
}
