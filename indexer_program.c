#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "common_words.h"
#include "utilities.h"

#define MAX_LINE_LEN 254

int main() {
    char line[MAX_LINE_LEN];
    int line_num = 1;

    // Read input line by line
    while (fgets(line, MAX_LINE_LEN, stdin) != NULL) {
        // Split line into words
        char *word = strtok(line, " \t\n\r.,;?!\"\'()");

        while (word != NULL) {
            // Convert to lowercase for case-insensitive comparison
            for (int i = 0; word[i]; i++) {
                word[i] = tolower(word[i]);
            }

            // Check if the word is common or empty
            if (!is_common_word(word) && strlen(word) > 0) {
                // Add word to index
                add_word(word, line_num);
            }

            // Get the next word
            word = strtok(NULL, " \t\n\r.,;?!\"\'()");
        }

        line_num++;
    }

    // Sort words by occurrences
    sort_words(words, word_count);

    // Print the words and their occurrences
    print_words(words, word_count);

    return 0;
}
