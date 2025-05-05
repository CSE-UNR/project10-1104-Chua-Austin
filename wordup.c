//Author: Austin Chua
//Date: April 21st, 2025
//Purpose: Project #10

#include <stdio.h>
#include <stdbool.h>
#define GUESSES 6
#define WRD_LENGTH 8
#define rFILE "mystery.txt"

void readTargetWrd(FILE* fout, int size, char wrd[]);
void displayPrompt(int total, int row, int col, char guess[][col]);
int wrdCheck(int total, int row, int col, char guess[][col], bool* vGuess);
void errorPrompt(int t, int n);
int winChk(int total, int rows, int cols, char guess[][cols], int sizeW, char wrd[], int sizeCr, int sizeCc, char correct[][sizeCc]);
void displayWrdResult(int total, int rows, int cols, char guess[][cols], int sizeCr, int sizeCc, char correct[][sizeCc]);
void write2DArr(int t, int r, int c, char arr[][c]);
bool winCondition(int ct);
void endResult(int total, bool notWon, int sizeW, char wrd[]);

int main(){
	int aTotal = 0, count = 0;
	_Bool noWin = true, validGuess = false;
	char targetWrd[WRD_LENGTH], userWrd[GUESSES][WRD_LENGTH], correctArr[GUESSES][WRD_LENGTH];
	
	//targetWrd: word to guess, userWrd: entered guess, correctArr: hints '^'
	
	FILE* fp1;
	
	fp1 = fopen(rFILE, "r");
	
	if (fp1 == NULL){ //ensures file is able to open properly
		printf("Unable to open %s!\n", rFILE);
		return 0;
	} else {
		readTargetWrd(fp1, WRD_LENGTH, targetWrd);
		fclose(fp1);
	}
	
	while (aTotal < 6 && noWin) { //runs game until either max attempts reached or win condition is met
		displayPrompt(aTotal, GUESSES, WRD_LENGTH, userWrd);
		aTotal = wrdCheck(aTotal, GUESSES, WRD_LENGTH, userWrd, &validGuess); //increments "main index" (attempt) for program
		count = winChk(aTotal, GUESSES, WRD_LENGTH, userWrd, WRD_LENGTH, targetWrd, GUESSES, WRD_LENGTH, correctArr);
		
		if (validGuess){
			displayWrdResult(aTotal, GUESSES, WRD_LENGTH, userWrd, GUESSES, WRD_LENGTH, correctArr);
		}
		
		noWin = winCondition(count);
	}
	
	endResult(aTotal, noWin, WRD_LENGTH, targetWrd);
	
	return 0;
}

void readTargetWrd(FILE* fout, int size, char wrd[]){

	fgets(wrd, size, fout);

}

void displayPrompt(int total, int row, int col, char guess[][col]){
	
	if (total < 5){
		printf("\nGUESS %d! Enter your guess! ", total + 1);
		scanf("%s", guess[total]);
	} else {
		printf("\nFINAL GUESS : ");
		scanf("%s", guess[total]);
	}

}

int wrdCheck(int total, int row, int col, char guess[][col], bool* vGuess){
	int countT = 0, countN = 0;
	
	for (int i = 0; guess[total][i] != '\0'; i++){
		countT++;
		
		if (guess[total][i] >= 'A' && guess[total][i] <= 'Z' ){ //converts all uppercase to lowercase
			guess[total][i] += 32;
		}
		
		if (guess[total][i] >= 'a' && guess[total][i] <= 'z'){ //if char is a lowercase letter, add to countN
			countN++;
		}
		
	}

	errorPrompt(countT, countN);

	while (countT != 5 || countN != 5){ //if total # of lowercase chars isn't 5, restart.
	
		countT = countN = 0;
		
		scanf("%s", guess[total]);
	
		for (int i = 0; guess[total][i] != '\0'; i++){
			countT++;
			
			if (guess[total][i] >= 'A' && guess[total][i] <= 'Z' ){ //converts all uppercase to lowercase
				guess[total][i] += 32;
			}
		
			if (guess[total][i] >= 'a' && guess[total][i] <= 'z'){ //if char is a lowercase letter, add to countN
				countN++;
			}
			
		}
		
		errorPrompt(countT, countN);

	}
	
	*vGuess = true; //if everything runs correctly, registers attempt as a valid guess
	
	return total + 1;
}

void errorPrompt(int t, int n){
	
	if (t != 5 || n != 5){ //Prompts if input fails to meet either condition
	
		if (t != 5){
			printf("Your guess must be 5 letters long. ");
		}
		
		if (n != 5){
			printf("Your guess must only contain letters.");
		}
		
		printf("\nPlease Try again: ");
	}
	
}

int winChk(int total, int rows, int cols, char guess[][cols], int sizeW, char wrd[], int sizeCr, int sizeCc, char correct[][sizeCc]){
	int counter = 0;
   
	total--; //scope and duration: this function (momentarily changes total var for these actions)

	bool usedInWrd[5] = {false}; // new array, tracks letters used in the target word
	bool usedInGuess[5] = {false}; // new array, tracks letters in guess that are already correct, affects later logic checks

	for (int i = 0; i < 5; i++) { //clears all unnecessary hints from previous attempts
	
		correct[total][i] = ' ';
		
	}

	for (int i = 0; i < 5; i++) { 	//checks for correct letter in the correct spot
		if (guess[total][i] == wrd[i]) {
			guess[total][i] -= 32;
			usedInWrd[i] = true; 
			usedInGuess[i] = true; //sets these two boolean arrays to 'true' to "mark" them
			counter++; //if all 5 indices are correct, game is won
		}
	}

	for (int i = 0; i < 5; i++) { //checks for correct letter in any spot
		if (usedInGuess[i]){
			continue; //Proceeds if value is true
		}
        

		for (int j = 0; j < 5; j++) {
			if (!usedInWrd[j] && guess[total][i] == wrd[j]) {
                		correct[total][i] = '^'; //correct letter in wrong spot = hint.
                		usedInWrd[j] = true;
                		break;
            		}
		}
	
	}

	return counter;
}

void displayWrdResult(int total, int rows, int cols, char guess[][cols], int sizeCr, int sizeCc, char correct[][sizeCc]){
	int start;
		
	for (int i = 0; i < 8; i++){
		printf("====");
	}
	
	if (total == 0){ 
		total--;
		printf("\n");
		write2DArr(total, rows, cols, guess);
		printf("\n");
		write2DArr(total, sizeCr, sizeCc, correct);
	}
	
	for (int i = 0; i < total; i++){ //loops to display all previous guesses and associated hints.
		printf("\n");
		write2DArr(i, rows, cols, guess);
		printf("\n");
		write2DArr(i, sizeCr, sizeCc, correct);
	}
	
}

void write2DArr(int t, int r, int c, char arr[][c]){
	
	for (int rowI = 0; rowI < 1; rowI++){ //displays a specified row (attempt/hint) of a specified 2d array
		printf("%s", arr[t]);
	}
	
}

bool winCondition(int ct){

	switch (ct){ //checks result of winChk
		case 5:
			return false; //noWin = false, thus Win.
			break;
		default: 
			return true;
			break;
	}
	
}

void endResult(int total, bool notWon, int sizeW, char wrd[]){ 
	
	if (!notWon){
	
		switch (total){
			case 1: 
			
				printf("\n");
			
				for (int i = 0; i < 8; i++){
					printf("====");
				}
				
				printf("\n\t\t");
				
				for (int i = 0; i < 5; i++){
					wrd[i] -= 32;
				}
				
				for (int i = 0; wrd[i] != '\0'; i++){
					printf("%c", wrd[i]);
				}
				
				printf("\tYou won in 1 Attempt!\n");
				printf("\t       AMAZING!\n\n");
				
				break;
			default:
			
				printf("\n");
				
				for (int i = 0; i < 8; i++){
					printf("====");
				}
				
				printf("\n\t\t");
				
				for (int i = 0; i < 5; i++){
					wrd[i] -= 32;
				}
				
				for (int i = 0; wrd[i] != '\0'; i++){
					printf("%c", wrd[i]);
				}
				
				printf("\tYou won in %d Attempts!\n", total);
				printf("\t   CONGRATULATIONS!\n\n");
				break;
		}
		
	} else {
	
		printf("\nYou lost, better luck next time!\n");
		
	}
	
}
