#include "myutil4968_5758.h"

//Global variables
dict * dictptr;
dict * wordsUsedptr;

/*
	Description: Print help menu
	Parameters: nil
	Return: nil
*/
void printHelp() {
	printf("\n\n");
	printf("+-----------------------+\n");
	printf("|          Help         |\n");
	printf("+-----------------------+\n");
	printf("This is a two players game.\n\n");
	printf("  The game begins by showing the players a randomly picked word\n");
	printf("  The two players will take turns to enter a new word to replace the current word according to the game rules.\n");
	printf("  Repeat the above process to earn scores from the game.\n");
	printf("  The game will be terminated when one of the players has chosen to quit or entered an unacceptable word\n");
	
	printf("\nAcceptance Rules for a new word:\n\n");
	printf("  .The minimum length of a word is 6 letters\n");
	printf("  .It is listed in the internal dictionary.\n");
	printf("  .It has not been entered at the same play session.\n");
	printf("  .The new word must begin with the last letter of the current word\n");
	printf("  .It cannot be ended with -ing-.\n");

	printf("\nScoring Rules:\n\n");

	printf("  Each letter in the new word earns points based on the following point values:\n");
	printf("\n"); 
	printf("  (1 point)-a, e, i, o, u, l, n, s, t, r.\n");
	printf("  (2 points)-d, g.\n");
	printf("  (3 points)-b, c, m, p.\n");
	printf("  (4 points)-f, h, v, w, y.\n");
	printf("  (5 points)-k.\n");
	printf("  (8 points)- j, x.\n");
	printf("  (10 points)-q, z.\n");
	printf("\nPress <Enter Key> to exit help");

	//To capture when user clicks ENTER
	getchar();

	printf("+-----------------------+\n");
	printf("|      End of Help      |\n");
	printf("+-----------------------+\n");
}

/*
	Description: Remove '\n' from fgets input
	Parameters: <char * s (string)>
	Return: nil
*/
void fixInput(char * s) {
	int length = strlen(s);

	int i;
	for (i = 0; i < length; i++) {
		if (s[i] == '\n') {
			s[i] = (char) 0;
		}
	}
}

/*
	Description: Only allows lowercase 'a' to 'z'
	Parameters: <char * word (string)>
	Return: int (1 = pass, 0 = fail)
*/
int restrictionCheck(char * word) {
	int check = 1;
	char c;

	int i;
	for (i = 0; i < strlen(word); i++) {
		c = word[i];
		if (!(c >= 'a' && c <= 'z')) {
			check = 0;
			break;
		}
	}

	return check;
}

/*
	Description: Get user input and stores in string passed
	Parameters: <string (char *) to store user input>, <int length>, <int restriction>
	Return: nil
*/
void getUserinput(char * var, int length, int restriction) {
	fgets(var,length,stdin);
	fixInput(var);

	if (restriction == 1 && restrictionCheck(var) == 0) {
		//Restriction enabled
		//Set to empty string if check fails
		strcpy(var,"");
	}
}

/*
	Description: Get name of user
	Parameters: <string (char *) to display>, <string (char *) to store user input>
	Return: nil
*/
void getName(char * msg, char * var) {
	do {
		printf("%s",msg);
		getUserinput(var,MAXNAME_LEN,0);

		if (strcmp(var,"") == 0) {
			printf(COLOR_RED "Error: " COLOR_RESET "Name cannot be empty.\n\n");
		}
	} while (strcmp(var,"") == 0);
}

/*
	Description: Check for given ending pattern
	Parameters: <string (char *) to check>, <string (char *) as pattern>
	Return: int (1 = contains pattern, 0 = doesn't)
*/
int containsPatternEnd(char * word, char * pattern) {
	int contains = 1;

	int wlen, plen;
	wlen = strlen(word);
	plen = strlen(pattern);

	if (wlen >= plen) { //Make sure word length is larger than pattern
		int i;
		for (i = 0; i < plen; i++) {
			//Compare nth character of pattern with ((length of word) - n + i)th character of word
			if (pattern[i] != word[wlen - plen + i]) {
				contains = 0;
				break;
			}
		}
	}

	return contains;
}

/*
	Description: Check if word is valid
	Parameters: <string (char *) as current word>, <string (char *) as input word>
	Return: int (0 = fail, 1 = pass, 2 = wrong word, 3 = ending with -ing-, 4 = repeated word, 5 = less than 6 chars)
*/
int checkWord(char * curWord, char * inWord) {
	int check = 0, index;
	char lastChar;
	wordNode * node;

	//Word length requirement check
	if (strlen(inWord) >= MINWORD_LEN) {
		if (searchDict(wordsUsedptr,inWord) == NULL) { //Check if word is used before
			if (containsPatternEnd(inWord,"ing") == 0) { //Check if word has pattern
				//Get last character of current word
				lastChar = curWord[strlen(curWord) - 1];

				//First node of character
				index = getCharIndex(lastChar);
				node = (*dictptr)[index];

				if (node != NULL) {
					node = searchList(node,inWord); //Check if word exists in list
					if (node != NULL) {
						check = 1;

						//Update word
						strcpy(curWord,node->word);
					} else {
						check = 2;
						printf("\n" COLOR_RED "Error: " COLOR_RESET "Wrong word. (Penalty: -5 points)\nSkipping your turn.\n");
					}
				} else {
					//Cannot find starting node for character
					printf("\n" COLOR_RED "Error: " COLOR_RESET "No words found for character '%c'.\n",lastChar);
				}
			} else {
				//Word ends with "ing"
				check = 3;
				printf("\n" COLOR_RED "Error: " COLOR_RESET "Word cannot end with -ing-. (Penalty: -10 points)\nSkipping your turn.\n");
			}
		} else {
			//Word already used
			check = 4;
			printf("\n" COLOR_RED "Error: " COLOR_RESET "\"%s\" has already been used. (Penalty: -10 points)\nSkipping your turn.\n",inWord);
		}
	} else {
		//Word does not meet length requirement
		check = 5;
		printf("\n" COLOR_RED "Error: " COLOR_RESET "Your word must have at least %d characters and can only contain lowercase letters.\n",MINWORD_LEN);
	}
	
	return check;
}

/*
	Description: Player Round
	Parameters: <string (char *) as player name>, <string (char *) as current word>
	Return: int (0 = pass, 1 = quit, 2 = view used words, 3 = wrong word, 4 = end with -ing- or repeated word, 5 = less than 6 chars)
*/
int playerRound(char * name, char * word) {
	int r = 0;
	int check;
	char input[MAXWORD_LEN];

	printf("\nCurrent word is " COLOR_YELLOW "\"%s\"" COLOR_RESET "\n",word);
	printf("Enter 'v' to view used words (Penalty: -5 points)\n");
	printf(COLOR_CYAN "%s" COLOR_RESET ", please enter the next word (or 'h' for help, 'q' to quit) => ",name);
	getUserinput(input,MAXWORD_LEN,1);

	if (strcmp(input,"h") == 0) {
		//help
		printHelp();
		r = playerRound(name,word); //retry to type new word
	} else if (strcmp(input,"q") == 0) {
		//quit
		r = 1;
	} else if (strcmp(input,"v") == 0) {
		//show used words
		printList(wordsUsedptr,word[strlen(word) - 1]);
		r = 2;
	} else {
		//Valid Input
		check = checkWord(word, input);
		switch (check) {
			case 0: {
				//Check fail
				strcpy(word,"");
				break;
			}

			case 1: {
				//Check pass
				wordNode * newNode = createNewNode(word);
				addNodeToDict(wordsUsedptr, newNode);
				break;
			}

			case 2: {
				r = 3;
				break;
			}

			case 3: //Word ends with "ing"
			case 4: //Word already used
				r = 4;
				break;
			case 5: { //Word length does not meet requirement
				printf("Please try again.\n");
				r = playerRound(name,word);
				break;
			}

			default: {
				break;
			}
		}
	}

	return r;
}

/*
	Description: Get points for character
	Parameters: <char c>
	Return: int
*/
int getCharacterPoints(char c) {
	int points = 0;

	switch (c) {
		//a, e, i ,o , u, l, n, s, t, r
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		case 'l':
		case 'n':
		case 's':
		case 't':
		case 'r': {
			points = 1;
			break;
		}

		//d, g
		case 'd':
		case 'g': {
			points = 2;
			break;
		}

		//b, c, m, p
		case 'b':
		case 'c':
		case 'm':
		case 'p': {
			points = 3;
			break;
		}

		//f, h, v, w, y
		case 'f':
		case 'h':
		case 'v':
		case 'w':
		case 'y': {
			points = 4;
			break;
		}

		//k
		case 'k': {
			points = 5;
			break;
		}

		//j, x
		case 'j':
		case 'x': {
			points = 8;
			break;
		}

		//q, z
		case 'q':
		case 'z': {
			points = 10;
			break;
		}
	}

	return points;
}

/*
	Description: Calculate points for word
	Parameters: <string (char *) as word>
	Return: int
*/
int calculateWordPoints(char * word) {
	int points = 0;
	char c;

	int i, p;
	for (i = 0; i < strlen(word); i++) {
		c = word[i];
		p = getCharacterPoints(c); //Individual char points

		if (i == 0) { //Check if first char
			printf(COLOR_YELLOW "%c(%02d)" COLOR_RESET,c,p);
		} else {
			printf(COLOR_YELLOW "+ %c(%02d)" COLOR_RESET,c,p);
		}

		points += p; //Add to points
	}

	if (points > 0) {
		printf(COLOR_YELLOW "= %d\n" COLOR_RESET,points);
	}

	return points;
}

/*
	Description: Confirm quit
	Parameters: <string (char *) as next player name>
	Return: int (1 = quit, 0 = no quit)
*/
int quitConfirm(char * name) {
	int quit = 0;
	char input[MAXWORD_LEN];

	printf("\n");
	printf("+---------------------------------+\n");
	printf("| Next player please confirm quit |\n");
	printf("+---------------------------------+\n");
	printf(COLOR_CYAN "%s" COLOR_RESET ", please enter \"quit\" to confirm (or enter any key to continue) => ",name);

	getUserinput(input,MAXWORD_LEN,0);
	if (strcmp(input,"quit") == 0) {
		quit = 1;
	}

	return quit;
}

/*
	Description: Get no. of rounds from players
	Parameters: nil
	Return: int
*/
int getMaxRounds() {
	int rounds;
	char input[MAXWORD_LEN];

	do {
		printf("Please enter the no. of rounds to play => ");
		getUserinput(input,MAXWORD_LEN,0);
		rounds = atoi(input);

		if (!(rounds > 0)) {
			printf(COLOR_RED "Error: " COLOR_RESET "No. of rounds must be more than 0.\n\n");
		} else {
			if (rounds % 2 != 0) {
				rounds = 0;
				printf(COLOR_RED "Error: " COLOR_RESET "No. of rounds must be even.\n\n");
			}
		}
	} while (!(rounds > 0));

	return rounds;
}

/*
	Description: Gameplay
	Parameters: nil
	Return: nil
*/
void playGame() {
	int round = 1, maxRounds;
	int scores[] = {0,0};
	char names[2][MAXNAME_LEN];
	char word[MAXWORD_LEN];

	printf("+-----------------------------+\n");
	printf("|Welcome to ST2512 playground!|\n");
	printf("+-----------------------------+\n");

	getName("Player1 Name Please => ",names[0]);
	getName("Player2 Name Please => ",names[1]);

	maxRounds = getMaxRounds();

	printf("\n");

	//Random word for first round
	int hasRandomWord;
	hasRandomWord = getRandomWord(dictptr,word);

	if (hasRandomWord == 1) {
		int playerIndex, pRnd, points;
		int quit = 0;
		while (1) {
			if (quit == 1) {
				quit = quitConfirm(names[round % 2]);
				if (quit == 1) {
					printf("+--------------+\n");
					printf("| Quit Success |\n");
					printf("+--------------+\n");
					printf("\n");
					break;
				} else {
					printf("+----------------+\n");
					printf("| Game Continues |\n");
					printf("+----------------+\n");
					printf("\n");
					continue;
				}
			}

			quit = 0;

			if (round == maxRounds + 1) {
				break;
			}

			//Display score and word
			//printf("Round %d:\n%s scores: %d   %s scores: %d\nCurrent word is %s\n\n",round,names[0],scores[0],names[1],scores[1],word);
			printf("+-------------+\n");
			printf("| Round %-5d |\n",round);
			printf("+-------------+\n");
			printf(COLOR_CYAN "%s: " COLOR_RESET "%d\n",names[0],scores[0]);
			printf(COLOR_CYAN "%s: " COLOR_RESET "%d\n",names[1],scores[1]);

			//Get current player's index
			playerIndex = (round + 1) % 2;
			pRnd = playerRound(names[playerIndex],word);

			switch (pRnd) {
				case 1: {
					//Quit
					quit = 1;
					continue;
					break;
				}

				case 2: {
					scores[playerIndex] -= 5;
					continue;
					break;
				}

				case 3: {
					//Wrong word
					scores[playerIndex] -= 5;
					break;
				}

				case 4: {
					//Repeated words or word end with -ing-
					scores[playerIndex] -= 10;
					break;
				}

				default: {
					//Calculate and add points
					printf("\n");
					points = calculateWordPoints(word);
					scores[playerIndex] += points;
					break;
				}
			}

			printf("+--------------+\n");
			printf("| End of Round |\n");
			printf("+--------------+\n\n\n");

			round++;

			if (scores[playerIndex] <= MINPOINTS) {
				break;
			}
		}

		//End
		printf("\n\n");
		printf("+-----------------------+\n");
		printf("|        Results        |\n");
		printf("+-----------------------+\n");
		printf("No of rounds played: %d\n",(round - 1));
		printf(COLOR_CYAN "%s: " COLOR_RESET "%d\n",names[0],scores[0]);
		printf(COLOR_CYAN "%s: " COLOR_RESET "%d\n\n",names[1],scores[1]);

		char winner[MAXNAME_LEN];
		if (scores[0] == scores[1]) {
			strcpy(winner,"");
		} else if (scores[0] > scores[1]) {
			strcpy(winner,names[0]);
		} else {
			strcpy(winner,names[1]);
		}

		if (strcmp(winner,"") == 0) {
			//Tied
			printf("Congratulations to both players, game is tied.\n");
		} else {
			printf("Congratulations to %s, you are the champion!\n",winner);
		}

		printf("Thank you for playing. See you again soon.\n\n");
	} else {
		printf(COLOR_RED "Error: " COLOR_RESET "Cannot generate random word.\n");
	}
}

int main() {
	//Create and load dictionary
	dictptr = createDict();
	int run = initializeDict(dictptr,"wordlist.txt");

	wordsUsedptr = createDict();

	//Start game
	if (run == 1) {
		playGame();
	}

	//End of program, free all memory
	freeDict(dictptr);
	freeDict(wordsUsedptr);

	return 0;
}