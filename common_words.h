#ifndef COMMON_WORDS_H
#define COMMON_WORDS_H

// Array of common words to ignore
const char *common_words[] = {
    "the", "and", "is", "in", "at", "of", "on", "for", "with", "a", "an", "to", NULL
};

// Function to check if a word is in the common words list
int is_common_word(const char *word) {
    for (int i = 0; common_words[i] != NULL; i++) {
        if (strcmp(word, common_words[i]) == 0) {
            return 1;  // Word is common
        }
    }
    return 0;  // Word is not common
}

#endif
