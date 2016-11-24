#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define LETTERSCOUNT 26 //'a' to 'z' 26 char
#define MAXNAME_LEN 35
#define MAXWORD_LEN 35
#define MINWORD_LEN 6
#define MINPOINTS -10

//Colors
//http://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"

typedef struct node {
	char word[MAXWORD_LEN];
	struct node * nextNode;
} wordNode;
typedef wordNode * dict[LETTERSCOUNT];

//Function prototypes
dict * createDict();
wordNode * createNewNode(char * w);
int getNodeListSize(wordNode * h);
wordNode * addNodeToList(wordNode * h, wordNode * newNode);
int getCharIndex(char c);
void addNodeToDict(dict * d, wordNode * node);
int initializeDict(dict * d, char * filename);
void freeDict(dict * d);
void freeNode(wordNode * n);
int getRandomNum();
int getRandomWord(dict * d, char * word);
wordNode * searchList(wordNode * node, char * w);
wordNode * searchDict(dict * d, char * w);
void printDict(dict * d);
void printList(dict * d, char c);