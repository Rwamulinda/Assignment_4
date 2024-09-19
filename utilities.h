#ifndef UTILITIES_H
#define UTILITIES_H

#define MAX_WORD_LEN 20
#define MAX_WORDS 1200

// Data structure to store words and their occurrences
struct Word {
    char word[MAX_WORD_LEN + 1];
    int occurrences;
    int lines[20];  // Line numbers where the word appears
};

// External declarations (since words and word_count are global)
extern struct Word words[MAX_WORDS];
extern int word_count;

// Function prototypes
void add_word(const char *word, int line);
void sort_words(struct Word words[], int word_count);
void print_words(const struct Word words[], int word_count);

#endif

