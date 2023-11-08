#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
/*
Specifications:
Pre-Conditions:
-char* spells [] is the Array of Strings containing a list of words. The list should not be empty.
-int SpellsSize is the number of words in the list (size of the array)
-char*word is any word given by the client.

Expected Output: 
-Returns True if the char*word provided by the user is found in the char*spells [] list.
-Returns false otherwise. (If the given word is not found in the list).

Test Cases:

 
 */
// Function to check if a word is in the list of spells
bool spellProvided(char *spells[], int spellsSize, char word[]) {
    for (int i = 0; i < spellsSize; i++) {
        int comp = strcmp(word, spells[i]);
        if (comp == 0)
            return true;
    }
    return false;
}

/*
Specifications:
Pre-Conditions:
-char* usedWords [] is the Array of Strings containing a list of words used by the players. The Array of Strings should not be empty.
-int usedSize is the number of words in the list (size of the array)
-char*word is any word given by the client.

Expected Output: 
-Returns True if the char*word provided by the user was used another time in the game.
-Returns false otherwise. (If the given word was not used before in the game).

Test Cases:



*/

// Function to check if a word has already been used
bool spellAlreadyUsed(char *spells[], int spellsSize, char usedWords[]) {
    for (int i = 0; i < spellsSize; i++) {
        int comp = strcmp(usedWords, spells[i]);
        if (comp == 0)
            return true;
    }
    return false;
}
/*
Specifications:
Pre-Conditions:
- Given any Two strings (char *previousWord, char *currentWord).
- The order of parameters matters pass the previousWord with the last character being common with the first Character of currentWord.

Expected Output:
-Returns true if the last character of the previous word matches the first character of the current word.
-Returns false otherwise.


Test Cases:

*/

//Function to check if the last character of the previous word matches the first character of the current word
bool charMatching(char previousWord[], char currentWord[]) {
    size_t len = strlen(previousWord);
    char lastChar = previousWord[len - 1];
    char firstChar = currentWord[0];
    return lastChar == firstChar;
}
/*
Specifications:
Pre-Condition:
- Take a string char*currentWord 
-An array of integers that counts the number of words starting with the alphabetical letters in a given list. (int Tally[])

Expected Output:
-Returns true if the list contains available starting letters mataching the lastLetter of the String char*currentWord.
-Returns false otherwise.

Test Cases:

*/
// Function to check if a spell has run out of uses
bool runOut(char currentWord[], int Tally[]) {
    size_t len = strlen(currentWord);
    char lastLetter = currentWord[len - 1];
    int Index = lastLetter - 97;
    if (Tally[Index] <= 0)
        return false;
    else
        return true;
}

int main() {
    // Step 1
    FILE *filePointer = fopen("spells.txt", "r");
    if (filePointer == NULL) {
        printf("file did not open \n");
        return 0;
    }

    char size[100];
    char *spells[78];
    int i = 0;

    // Read spell names into the spells array
    while (i < 78 && fgets(size, sizeof(size), filePointer)) {
        size_t len = strlen(size); // Check for lines that are too long
        if (len > 0 && size[len - 1] == '\n') {
            size[len - 1] = '\0'; // Remove the newline character
        }
        spells[i] = strdup(size);
        i++;
    }

    fclose(filePointer);

    // Using The Tally Method we are creating an array to keep track of the remaining words.
    int Tally [26];
    for(int i=0;i<26;i++){
        Tally[i]=0;
    }
    for (int i = 0; i < 78; i++) {
        char firstLetter = spells[i][0];
        int value = firstLetter - 97;
        Tally[value]=Tally[value]+1;
    }
    // Step 2
    char player1[100];
    printf("Player 1 please enter your first name: ");
    scanf(" %s", player1);
    printf("Welcome %s!\n", player1);
    printf("Player 2 please enter your first name: ");
    char player2[100];
    scanf(" %s", player2);
    printf("Welcome %s!\n", player2);

    // Step 3
    for (int i = 0; i < 78; i++) {
        if (i % 5 != 0) {
            printf("%s\t", spells[i]);
        }
        else{
            printf("\n");
            printf("%s\t", spells[i]);
        }
    }
    printf("\n\n");

    // Step 4
    srand(time(NULL));
    int startingPlayer = rand() % 2; // Use the random number to determine which player starts
    bool player1Turn;

    if (startingPlayer == 0) {
        printf("%s will start the game\n", player1);
        player1Turn = true;
    }
    else {
        printf("%s will start the game\n", player2);
        player1Turn = false;
    }
    printf("\n");
    // Step 5
    int numberOfPlays = 78;
    char *usedWords[78];
    // Allocate memory for each char pointer
    for (int i = 0; i < numberOfPlays; i++) {
        usedWords[i] = (char *)malloc(78 * sizeof(char));
    }

    bool gameInProgress = true;
    int index = 0;
    while (gameInProgress) {
        if (index!=0) {  // After the 1st Input we should always check if there is sufficient number of words to continue the game.
            if(runOut(usedWords[index - 1], Tally)==false)
            break;
        }
        if (player1Turn) {
            printf("%s pick a spell from the following List: ", player1);
            player1Turn = false;
        }
        else {
            printf("%s pick a spell from the following List: ", player2);
            player1Turn = true;  
        }

        scanf("%s", usedWords[index]);
        char firstLetter = usedWords[index][0];
        int value = firstLetter - 97;
        Tally[value]--;
        index++;

        if(index==1) // At the first iteration we have Only 1 word to avoid calling the function charNotMatching
        gameInProgress = spellProvided(spells, 78, usedWords[index - 1]) && !spellAlreadyUsed(usedWords, index-1, usedWords[index - 1]);
        else
        gameInProgress = spellProvided(spells, 78, usedWords[index - 1]) && !spellAlreadyUsed(usedWords, index-1, usedWords[index - 1]) && charMatching(usedWords[index - 2], usedWords[index - 1]);
    }
    // Step 6 announcing the Winner.
    char winner[100];
    char loser[100];
    
    if(player1Turn && (spellAlreadyUsed(usedWords, 78, usedWords[index - 1]) || !spellProvided(spells, index-1, usedWords[index - 1])  || !charMatching(usedWords[index - 2], usedWords[index - 1]))) {
        strcpy(winner,player1);
        strcpy(loser,player2);
    }
    else {
        strcpy(winner,player2);
        strcpy(loser,player1);
    }
    if (!spellProvided(spells, 78, usedWords[index - 1])) {
        printf("%s is the Winner! Congratulations! You %s did not provide a Spell that is found in the list.\n ", winner, loser);
    } 
    else if ( spellAlreadyUsed(usedWords,index-1, usedWords[index - 1] )) {
    printf("%s is the Winner! Congratulations! You won since %s casted a Spell that is already used.\n ", winner, loser);
    }
    else if (index != 1 && !charMatching(usedWords[index - 2], usedWords[index - 1])) {    
      printf("%s is the Winner! Congratulations! You won since the first character of the word provided by %s is not matching the last character of the previous word.\n ", winner, loser);
    }
    else if (runOut(usedWords[index - 1], Tally)==false) {
        printf("%s is the Winner! Congratulations! You won due to %s running out of words.\n ", loser, winner); // It is the only opposite case.
    }

    //free the allocated memory when you're done with it
    for (int i = 0; i < numberOfPlays; i++) {
        free(usedWords[i]);
    }
    for(int i=0;i<numberOfPlays;i++){
        free(spells[i]);
    }
    return 0;
}