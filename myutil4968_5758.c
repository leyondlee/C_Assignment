#include "myutil4968_5758.h"

/*
	Description: Create dictionary
	Parameters: nil
	Return: dict *
*/
dict * createDict() {
	dict * d = (dict *) malloc(sizeof(dict));

	int i;
	for (i = 0; i < LETTERSCOUNT; i++) {
		(*d)[i] = (wordNode *)NULL;
	}

	return d;
}

/*
	Description: Create new node and set word
	Parameters: <string (char *) as word>
	Return: wordNode *
*/
wordNode * createNewNode(char * w) {
	wordNode * newNode = (wordNode *)malloc(sizeof(wordNode));
	strcpy(newNode->word,w);
	newNode->nextNode = NULL;

	return newNode;
}

/*
	Description: Get node list size
	Parameters: <wordNode * as head node>
	Return: int
*/
int getNodeListSize(wordNode * h) {
	int count = 0;

	//Loop until end of node
	if (h != NULL) {
		do {
			count++;
			h = h->nextNode;
		} while (h != NULL);
	}

	return count;
}

/*
	Description: Add node to list
	Parameters: <wordNode * as head node>, <wordNode * as new node>
	Return: wordNode * (new head node)
*/
wordNode * addNodeToList(wordNode * h, wordNode * newNode) {
	if (h != NULL) {
		wordNode * curNode = h;
		wordNode * prevNode = NULL;

		while (1) {
			//Check if new word is smaller
			if (strcmp(newNode->word,curNode->word) < 0) {
				if (prevNode != NULL) {
					prevNode->nextNode = newNode;
				} else {
					//No prev node = first node
					//Reassign first node
					h = newNode;
				}
				newNode->nextNode = curNode;
				break;
			}

			//Reached last node
			if (curNode->nextNode == NULL) {
				//Add newNode to the back of the current last node
				curNode->nextNode = newNode;
				break;
			}

			prevNode = curNode;
			curNode = curNode->nextNode;
		}
	}

	return h;
}

/*
	Description: Get index of character
	Parameters: <char c>
	Return: int
*/
int getCharIndex(char c) {
	//ASCII
	int index;
	index = c - 'a';

	return index;
}

/*
	Description: Add node to dict
	Parameters: <dict * d>, <wordNode * as new node>
	Return: nil
*/
void addNodeToDict(dict * d, wordNode * node) {
	char word[MAXWORD_LEN];
	strcpy(word,node->word);
	int index = getCharIndex(word[0]);

	//First node of character
	wordNode * firstNodeOfIndex = (*d)[index];
	if (firstNodeOfIndex != NULL) {
		//Make sure to reassign new first node if any
		(*d)[index] = addNodeToList((*d)[index],node);
	} else {
		//if no first node, assign as first
		(*d)[index] = node;
	}
}

/*
	Description: Load words from file to dictionary
	Parameters: <dict * d>, <string (char *) as filename>
	Return: int (1 = success, 0 = fail)
*/
int initializeDict(dict * d, char * filename) {
	//Load words into dict
	int success = 0;

	FILE * fp;
	fp = fopen(filename,"r");

	if (fp != NULL) {
		success = 1;

		char word[MAXWORD_LEN];
		wordNode * newNode;
		while (fscanf(fp,"%s",word) != EOF) {
			newNode = createNewNode(word);
			addNodeToDict(d,newNode);
		}

		fclose(fp);
	} else {
		printf(COLOR_RED "Error: " COLOR_RESET "\"%s\" not found\n",filename);
	}

	return success;
}

/*
	Description: Free nodes in dictionary
	Parameters: <dict * d>
	Return: nil
*/
void freeDict(dict * d) {
	int i;
	for (i = 0; i < LETTERSCOUNT; i++) {
		//Free individual first character nodes
		freeNode((*d)[i]);
	}
}

/*
	Description: Free nodes in list
	Parameters: <wordNode * as head node>
	Return: nil
*/
void freeNode(wordNode * n) {
	if (n != NULL) {
		//Chain reaction
		freeNode(n->nextNode);
	}

	free(n);
}

/*
	Description: Get random number
	Parameters: nil
	Return: int
*/
int getRandomNum() {
	srand(time(NULL)); //Reset seed
	int rnd = rand();

	return rnd;
}

/*
	Description: Get random word from dictionary
	Parameters: <dict * d>, <string (char *) as variable to store word>
	Return: int (1 = success, 0 = fail)
*/
int getRandomWord(dict * d, char * word) {
	int success = 1;

	int rnd, size;

	//Get random number between 0 and LETTERSCOUNT
	rnd = getRandomNum() % LETTERSCOUNT;

	//First node
	wordNode * node = (*d)[rnd];

	if (node != NULL) {
		//Get random number between 0 and size
		size = getNodeListSize(node);
		rnd = getRandomNum() % size;

		//Navigate to random word
		int i;
		for (i = 0; i < rnd; i++) {
			node = node->nextNode;
		}

		//Set word
		strcpy(word,node->word);
	} else {
		success = 0;
		strcpy(word,"");
	}

	return success;
}

/*
	Description: Search list for word
	Parameters: <wordNode * as head node>, <string (char *) as word to search>
	Return: wordNode *
*/
wordNode * searchList(wordNode * node, char * w) {
	int size;

	if (node != NULL) {
		size = getNodeListSize(node);

		int i;
		for (i = 0; i < size; i++) {
			if (strcasecmp(node->word,w) == 0) {
				break;
			}

			node = node->nextNode;
		}
	}

	return node;
}

/*
	Description: Search dictionary for word
	Parameters: <dict * d>, <string (char *) as word to search>
	Return: wordNode *
*/
wordNode * searchDict(dict * d, char * w) {
	wordNode * node;
	int index;

	index = getCharIndex(w[0]);
	node = (*d)[index];
	
	node = searchList(node,w);

	return node;
}

/*
	Description: Print all words in dictionary (in alphabetical order)
	Parameters: <dict * d>
	Return: nil
*/
void printDict(dict * d) {
	wordNode * node;
	char c;

	printf("\n------------------------------\n");
	printf("Words used:\n");

	int i, first;
	for (i = 0; i < LETTERSCOUNT; i++) {
		first = 1;

		c = 'a' + i;
		printf("%c: ",c);

		node = (*d)[i];
		if (node != NULL) {
			do {
				if (first == 1) { //Check if first
					first = 0;
					printf("%s",node->word);
				} else {
					printf(", %s",node->word);
				}

				node = node->nextNode;
			} while (node != NULL);
		}

		printf("\n");
	}

	printf("------------------------------\n");
	printf("Press <Enter Key> to exit");

	//To capture when user clicks ENTER
	getchar();

	printf("\n");
}

/*
	Description: Print all words of a given character
	Parameters: <dict * d>, <char c>
	Return: nil
*/
void printList(dict * d, char c) {
	int index, first = 1;
	wordNode * node;

	index = getCharIndex(c);
	node = (*d)[index];

	printf("\n------------------------------\n");
	printf("Words used:\n");

	printf("%c: ",c);

	if (node != NULL) {
		do {
			if (first == 1) { //Check if first
				first = 0;
				printf("%s",node->word);
			} else {
				printf(", %s",node->word);
			}

			node = node->nextNode;
		} while (node != NULL);
	}

	printf("\n");

	printf("------------------------------\n");
	printf("Press <Enter Key> to exit");

	//To capture when user clicks ENTER
	getchar();

	printf("\n");
}