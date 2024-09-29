#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <ctype.h>



enum TokenType {
	IDENTIFIER,			//0
	KEYWORD,			//1
	OPERATOR,			//2
	NUMERIC_LITERAL,		//3
	STRING_LITERAL,			//4
	BOOLEAN_LITERAL,		//5
	DELIMITER,			//6
	BRACE,				//7
	COMMENT,			//8
	PREPROCESSOR_DIRECTIVE,		//9
	EOF_TOKEN			//10
};


struct Token {
	enum TokenType type;
	char *text;
	int startPos;

};

int tokenCapacity = 10;
int tokenCount = 0;
struct Token** tokens; 

bool isOperator(char ch) {
	return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>';
}

bool isBrace(char ch) {
	return ch == '(' || ch == ')' || ch == '[' || ch == '{' || ch == '}';
}

bool isDelimiter(char ch) {
	//return ch == ' ' || ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ':';
	return ch == ' ' || ch == ',' || ch == ';' || ch == ':';
}

bool isReserved(char word[]) {
	return strcmp(word, "integer") == 0 || strcmp(word, "double") == 0 || strcmp(word, "output") == 0 || strcmp(word, "if") == 0;
}

bool isNuemric(char word[]) {
	return true; 
}

enum TokenType getTokenType(char word[]) {
	if (isOperator(word[0])) {
		return OPERATOR;
	}
	else if (isDelimiter(word[0])) {
		return DELIMITER;
	}
	else if (isBrace(word[0])) {
		return BRACE;
	}
	else if (isReserved(word)) {
		return KEYWORD;
	}
	else if (isdigit(word[0])) {
		return NUMERIC_LITERAL; 
	}
	return IDENTIFIER;
}

//struct Operator {
//	char symbol;
//	OperatorFunc function;
//};





struct Token* createToken(enum TokenType type, const char* text, int startPos) {
	struct Token* token = (struct Token*)malloc(sizeof(struct Token));
	if (token != NULL) {
		token->type = type;
		token->text = strdup(text);
		token->startPos = startPos;
	}
	return token;
};

void freeToken(struct Token* token) {
	if (token) {
		free(token->text);
		free(token);
	}
}




//lexical analyser
void lexus (char word[], int startPos) {
	if (strlen(word) == 0) {
		return;  // No token to process
	}

	// Determine token type based on the first character
	enum TokenType tokenType = getTokenType(word);

	// Create a token and add it to the global token array
	struct Token* token = createToken(tokenType, word, startPos);

	// Store token in the global array
	tokens[tokenCount] = token;
	tokenCount++;

	// Resize the array if needed
	if (tokenCount >= tokenCapacity) {
		tokens = (struct Token**)realloc(tokens, tokenCapacity * 2 * sizeof(struct Token*));
		tokenCapacity *= 2;
	}
	
}

void addLetter (char word[], char letter) {
	word[strlen(word)] = letter;
	word[strlen(word) + 1] = '\0';
}

void printTokens() {
	
	printf("Tokens:\n");
	//iterate through the Tokens Array
	for (int i = 0; i < tokenCount; i++) {
		//printf("Token %d: Type: %d, Text: %s, Start Position: %d\n", 
		//	i + 1, tokens[i]->type, tokens[i]->text, tokens[i]->startPos);
	}
}




void checkTokens() {

	int errors = 0;
	printf("TokenValidity\n");
	for (int i = 0; i < tokenCount; i++) {
		int next = i + 1;
		if (next >= 0 && next < tokenCount) {
			if (tokens[i]->type == IDENTIFIER) {
				//printf("found identifier \n");
				if(tokens[next]->type != DELIMITER &&
				   tokens[next]->type != OPERATOR &&
				   tokens[next]->type != BRACE) { 
					errors++;	
					printf("Error: next should be a Delimiter, Operator or Brace\n");
					printf("The text is %s on %d\n", tokens[i]->text, tokens[i]->startPos);
				}
			}	
			else if (tokens[i]->type == DELIMITER) {
				if(tokens[next]->type != NUMERIC_LITERAL &&
				   tokens[next]->type != STRING_LITERAL &&
				   tokens[next]->type != IDENTIFIER &&
				   tokens[next]->type != KEYWORD
				) {
					errors++;	
					printf("Error: next should be a Number, String, or Identifier\n");
					printf("The text is %s on %d\n", tokens[i]->text, tokens[i]->startPos);

				}
				
			}
			else if (tokens[i]->type == NUMERIC_LITERAL) {
				if(tokens[next]->type != DELIMITER &&
				   tokens[next]->type != BRACE &&
				   tokens[next]->type != OPERATOR
				) {
					errors++;	
					printf("Error: next should be a Delimiter, Braces, or Identifier\n");
					printf("The text is %s on %d\n", tokens[i]->text, tokens[i]->startPos);

				}	
			}
			else if (tokens[i]->type == KEYWORD) {
				if(tokens[next]->type != DELIMITER &&
				   tokens[next]->type != BRACE &&
				   tokens[next]->type != OPERATOR
				) {
					errors++;	
					printf("Error: next should be a Delimiter, Braces, or Identifier\n");
					printf("The text is %s on %d\n", tokens[i]->text, tokens[i]->startPos);

				}	
			}


		}
		else {
			printf("Reached End of File: %d errors found\n", errors);
		}
	}
}




int main(int argc, char *argv[]) {
	
	if (argc != 2) {
		printf("Usage:\n       %s <filename.HL> \n", argv[0]);
		return 1;
	}

	FILE *file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("File error");
		return 1;
	}


	//initialize memory
	//struct Token *words

	tokens = (struct Token**)malloc(tokenCapacity * sizeof(struct Token*));


	///read file. Lexer starts here
	char ch;
	int linecount = 1, charcount = 0;
	char strbuff[50] = "\0";
	while ((ch = fgetc(file)) != EOF) {
		if (isspace(ch)) {
			printf("%s \n", strbuff);
			lexus(strbuff, charcount);
			memset(strbuff, '\0', sizeof(strbuff));
		//	this is initially the idea, but became useless after implementing the is	operator, isDelimiter blahblah
		}
		else if (isOperator(ch)) {
			lexus(strbuff, charcount);
			printf("%s \n", strbuff);
			memset(strbuff, '\0', sizeof(strbuff));
			char nextch = fgetc(file);
			

			if (isOperator(nextch)) {
				addLetter(strbuff, ch);
				addLetter(strbuff, nextch);
				printf("%s \n", strbuff);
				lexus(strbuff, charcount);
				memset(strbuff, '\0', sizeof(strbuff));
			}
			else {
				addLetter(strbuff, ch);
				printf("%s \n", strbuff);
				lexus (strbuff, charcount);
				memset(strbuff, '\0', sizeof(strbuff));
				ungetc(nextch, file);
			}
		}
		else if (isDelimiter(ch) || isBrace(ch)) {
			//push the word into the lexer
			lexus(strbuff, charcount);
			printf("%s \n", strbuff);
			memset(strbuff, '\0', sizeof(strbuff));
			
			char nextch = fgetc(file);
			if(isOperator(nextch)) {
				addLetter(strbuff, ch);
				addLetter(strbuff, nextch);
				printf("%s \n", strbuff);
				lexus(strbuff, charcount);
				memset(strbuff, '\0', sizeof(strbuff));
			}
			else {
				addLetter(strbuff, ch);
				printf("%s \n", strbuff);
				lexus(strbuff, charcount);
				memset(strbuff, '\0', sizeof(strbuff));
				ungetc(nextch, file);
			}
		}
		else if (isdigit(ch)) {
			char nextch = fgetc(file);

			if (nextch == '.') {
				addLetter(strbuff, ch);
				addLetter(strbuff, nextch);
				printf("%s \n", strbuff);
			}
			else if(!isdigit(nextch)) {
				addLetter(strbuff, ch);
				printf("%s \n", strbuff);
				lexus(strbuff, charcount);
				memset(strbuff, '\0', sizeof(strbuff));
			
				ungetc(nextch, file);
			}
			else {
				addLetter(strbuff, ch);
				ungetc(nextch, file);
			}

		}
		
		
		
		//add the letter to the end of the string
		else addLetter(strbuff, ch);
		
		charcount++;
		if (ch == '\n') linecount++;

	}
	printf("-------- %s -------- \n %d lines \n %d characters\n", argv[1], linecount, charcount);
	printTokens();
	checkTokens();
	
	fclose(file);
	return 0;
				

}


