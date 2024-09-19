#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000

// Common words list provided by the professor
const char *common_words[] = {
    "a", "about", "after", "all", "also", "an",
    "and", "any", "are", "as", "at", "back", "be", "because", "but", "by",
    "can", "come", "could", "day", "do", "even", "first", "for", "from", "get",
    "give", "go", "good", "have", "he", "her", "him", "his", "how", "i",
    "if", "in", "into", "is", "it", "its", "it's", "just", "know", "like",
    "look", "make", "me", "most", "my", "new", "no", "not", "now", "of", "on",
    "one", "only", "or", "other", "our", "out", "over", "people", "say", "see",
    "she", "so", "some", "take", "than", "that", "the", "their", "them", "then",
    "there", "these", "they", "think", "this", "time", "to", "two", "up", "us",
    "use", "want", "way", "we", "well", "we'll", "what", "when", "which", "who",
    "will", "with", "work", "would", "year", "you", "your", NULL
};

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
    int lines[MAX_LINE_LENGTH];
    int line_count;
} WordEntry;

WordEntry word_list[MAX_WORDS];
int word_count = 0;

int is_common_word(const char *word) {
    for (int i = 0; common_words[i] != NULL; i++) {
        if (strcmp(word, common_words[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_word(const char *word, int line_number) {
    // Convert word to lowercase
    char lower_word[MAX_WORD_LENGTH];
    for (int i = 0; word[i]; i++) {
        lower_word[i] = tolower(word[i]);
    }
    lower_word[strlen(word)] = '\0';
    
    // Check if the word is already in the list
    for (int i = 0; i < word_count; i++) {
        if (strcmp(word_list[i].word, lower_word) == 0) {
            // Update count and line number
            word_list[i].count++;
            if (word_list[i].line_count < MAX_LINE_LENGTH) {
                word_list[i].lines[word_list[i].line_count++] = line_number;
            }
            return;
        }
    }
    
    // Add new word to the list
    if (word_count < MAX_WORDS) {
        strcpy(word_list[word_count].word, lower_word);
        word_list[word_count].count = 1;
        word_list[word_count].line_count = 0;
        word_list[word_count].lines[word_list[word_count].line_count++] = line_number;
        word_count++;
    }
}

void print_words() {
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d occurrences, lines: ", word_list[i].word, word_list[i].count);
        for (int j = 0; j < word_list[i].line_count; j++) {
            printf("%d ", word_list[i].lines[j]);
        }
        printf("\n");
    }
}

int main() {
    char line[MAX_LINE_LENGTH];
    int line_number = 1;

    while (fgets(line, sizeof(line), stdin)) {
        char *token = strtok(line, " \t\n\r,.!?\"';:");
        while (token != NULL) {
            // Remove punctuation and convert to lowercase
            char cleaned_word[MAX_WORD_LENGTH];
            int k = 0;
            for (int i = 0; token[i]; i++) {
                if (!ispunct(token[i])) {
                    cleaned_word[k++] = tolower(token[i]);
                }
            }
            cleaned_word[k] = '\0';

            if (strlen(cleaned_word) > 0 && !is_common_word(cleaned_word)) {
                add_word(cleaned_word, line_number);
            }

            token = strtok(NULL, " \t\n\r,.!?\"';:");
        }
        line_number++;
    }

    print_words();

    return 0;
}
