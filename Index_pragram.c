#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_WORD_LENGTH 256
#define MAX_WORDS 1000
#define MAX_LINES 1000
#define COMMON_WORDS_COUNT 100

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

// Function to check if a word is a common word
int is_common_word(const char *word) {
    for (int i = 0; common_words[i] != NULL; i++) {
        if (strcmp(word, common_words[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to convert a string to lowercase
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function to remove punctuation from a string, except for apostrophes
void strip_punctuation(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (!ispunct((unsigned char)*src) || *src == '\'') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
    int lines[MAX_LINES];
} WordEntry;

int compare(const void *a, const void *b) {
    return strcmp(((WordEntry *)a)->word, ((WordEntry *)b)->word);
}

int main() {
    char line[MAX_LINE_LENGTH];
    char word[MAX_WORD_LENGTH];
    int line_number = 0;

    WordEntry words[MAX_WORDS];
    int words_index = 0;

    while (fgets(line, sizeof(line), stdin)) {
        line_number++;
        char *token = strtok(line, " \t\n");
        while (token != NULL) {
            // Remove punctuation and convert to lowercase
            strip_punctuation(token);
            to_lowercase(token);

            // Check if the token is a common word
            if (!is_common_word(token) && strlen(token) > 0) {
                int found = 0;
                for (int i = 0; i < words_index; i++) {
                    if (strcmp(words[i].word, token) == 0) {
                        if (words[i].count == 0 || words[i].lines[words[i].count - 1] != line_number) {
                            words[i].lines[words[i].count] = line_number;
                            words[i].count++;
                        }
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    strcpy(words[words_index].word, token);
                    words[words_index].count = 1;
                    words[words_index].lines[0] = line_number;
                    words_index++;
                }
            }
            token = strtok(NULL, " \t\n");
        }
    }

    // Sort words alphabetically
    qsort(words, words_index, sizeof(WordEntry), compare);

    // Print results
    for (int i = 0; i < words_index; i++) {
        printf("%s [%d]:", words[i].word, words[i].count);
        for (int j = 0; j < words[i].count; j++) {
            if (j > 0) printf(", ");
            printf(" %d", words[i].lines[j]);
        }
        printf("\n");
    }

    return 0;
}

