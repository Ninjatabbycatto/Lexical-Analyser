#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum TokenType {
    IDENTIFIER,
    KEYWORD,
    OPERATOR,
    NUMERIC_LITERAL,
    STRING_LITERAL,
    BOOLEAN_LITERAL,
    DELIMITER,
    BRACE,
    COMMENT,
    PREPROCESSOR_DIRECTIVE,
    EOF_TOKEN
};

struct Token {
    enum TokenType type;
    char *text;
    int startPos;
};

struct Token* createToken(enum TokenType type, const char* text, int startPos) {
    struct Token* token = (struct Token*)malloc(sizeof(struct Token));
    if (token != NULL) {
        token->type = type;
        token->text = strdup(text);
        token->startPos = startPos;
    }
    return token;
}

void freeToken(struct Token* token) {
    if (token) {
        free(token->text);
        free(token);
    }
}

bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>';
}

bool isDelimiter(char ch) {
    return ch == ' ' || ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']';
}

// Lexical analyzer function
void lexus (char word[], int startPos) {
    // Check if there's an operator or delimiter
    int counter = startPos;
    for (int i = 0; word[i] != '\0'; i++) {
        char ch = word[i];
        if (isOperator(ch)) {
            printf("Operator found: %c\n", ch);
        } else if (isDelimiter(ch)) {
            printf("Delimiter found: %c\n", ch);
        } else {
            printf("Identifier: %s\n", word);
        }
        counter++;
    }
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("File error\n");
        return 1;
    }

    // Initialize variables
    char ch;
    int linecount = 0, charcount = 0;
    char strbuff[100] = "\0"; // Increased buffer size for longer strings

    // Read file and start lexical analysis
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch); // Optional: Print each character to the console

        if (isspace(ch)) {
            // If we encounter whitespace, process the word
            if (strlen(strbuff) > 0) {
                printf("Processing word: %s\n", strbuff);
                lexus(strbuff, charcount);
                memset(strbuff, '\0', sizeof(strbuff)); // Clear buffer
            }
        } else {
            size_t len = strlen(strbuff);
            strbuff[len] = ch;         // Add character to buffer
            strbuff[len + 1] = '\0';   // Null-terminate the string
        }

        charcount++;
        if (ch == '\n') linecount++;
    }

    // Process any remaining buffer content after EOF
    if (strlen(strbuff) > 0) {
        printf("Processing word: %s\n", strbuff);
        lexus(strbuff, charcount);
    }

    printf("-------- %s -------- \n%d lines \n%d characters\n", argv[1], linecount, charcount);

    fclose(file);
    return 0;
}

