#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

int** field;
int** fieldCopy;
int side = 0;
int max = 2;

void getFieldSize() {	//Get user input for the side sizes
	printf("Enter side size (more than 2)\n");
	while (side <= 2) {
		scanf_s("%d", &side);
		if (side <= 2) printf("Invalid value\n");
	}
}

void generateRandom(int needRand) {		//Generates 2 or 4 in a random empty spot on the field
	int value = 2;
	if (needRand) {
		int twoOrFour = rand() % 100;
		value = twoOrFour > 80 ? 4 : 2;
	}
	int foundEmpty = 0;
	while (!foundEmpty) {
		int row = rand() % side;
		int col = rand() % side;
		if (!field[row][col]) {
			field[row][col] = value;
			foundEmpty = 1;
		}
	}
}

void generateField() {		//Creates the initial field
	field = (int**)malloc(side * sizeof(int*));
	fieldCopy = (int**)malloc(side * sizeof(int*));;
	for (int i = 0; i < side; ++i) {
		field[i] = (int*)malloc(side * sizeof(int));
		fieldCopy[i] = (int*)malloc(side * sizeof(int));
		for (int j = 0; j < side; ++j) {
			field[i][j] = 0;
			//if (i == 0 && j == 3) field[i][j] = 4;
			//if (i == 1 && j == 3) field[i][j] = 2;
			//if (i == 2 && j == 3) field[i][j] = 4;
			//if (i == 3 && j == 3) field[i][j] = 2;
		}
	}
	generateRandom(0);
}

int countDigits(int value) {
	int digits = 0;
	while (value > 0) {
		value /= 10;
		++digits;
	}
	return digits;
}

void printField() {		//Output the field
	system("cls");
	for (int i = 0; i < side; ++i) {
		for (int j = 0; j < side; ++j) {
			int digits = countDigits(field[i][j]);
			switch (digits) {
				case 0:
				case 1: 
					printf("  %d |", field[i][j]);
					break;
				case 2: 
					printf(" %d |", field[i][j]);
					break;
				case 3: 
					printf(" %d|", field[i][j]);
					break;
				case 4:
					printf("%d|", field[i][j]);
					break;
				}
			//printf("%d ", field[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void rotateField() {		//Rotate field left 90 degrees (so we only slide to one side)
	for (int i = 0; i < side / 2; ++i) {
		for (int j = i; j < side - i - 1; ++j) {
			int t = field[i][j];
			field[i][j] = field[j][side - 1 - i];
			field[j][side - 1 - i] = field[side - 1 - i][side - 1 - j];
			field[side - 1 - i][side - 1 - j] = field[side - 1 - j][i];
			field[side - 1 - j][i] = t;
		}
	}
}

int slideField(char direction) {		//Slide field in a given direction
	int rotation = 0;
	switch (direction) {
		case 'u': 
			rotation = 1;
			break;
		case 'r':
			rotation = 2;
			break;
		case 'd':
			rotation = 3;
			break;
	}
	for (int i = 0; i < rotation; ++i) {
		rotateField();
	}
	int hasChange = 0;		//Check if the field changes with this rotation or no;
	for (int i = 0; i < side; ++i) {
		for (int j = 1; j < side; ++j) {
			if (field[i][j] && checkSpot(i, j)) {
				field[i][j] = 0;
				hasChange = 1;
			}
		}
	}
	if (rotation > 0) {		//If we rotated the field - return to its normal position
		for (int i = 0; i < 4 - rotation; ++i) {
			rotateField();
		}
	}
	return hasChange;
}

int checkSpot(int row, int col) {		//Check where a tile goes to (true if moved, false if stayed)
	int value = field[row][col];
	int cellToGo = col;
	for (int i = col - 1; i >= 0; --i) {
		if (field[row][i] != 0) {
			if (field[row][i] == value) {
				field[row][i] *= 2;
				if (field[row][i] > max) max = field[row][i];
				return 1;
			}
			else if (cellToGo != col) {
				field[row][cellToGo] = value;
				return 1;
			}
			else return 0;
		}
		else cellToGo = i;
	}
	if (!cellToGo) {		//when cellToGo is 0;
		field[row][0] = value;
		return 1;
	}
	return 0;
}

int countEmpty() {
	int cnt = 0;
	for (int i = 0; i < side; ++i) {
		for (int j = 0; j < side; ++j) {
			if (field[i][j] == 0) ++cnt;
		}
	}
	return cnt;
}

char* checkEndGame() {
	if (max == 2048) return "YOU WON!";
	int emptySpaces = countEmpty();
	if (emptySpaces == 0) {
		int** fieldCopy = field;
		if (!slideField('l') && !slideField('r') && !slideField('u') && !slideField('d')) return "YOU LOST!";
		field = fieldCopy;
	}
	return "Continue";
}

char getSlideDirection() {
	char inp = _getch();
	if (inp == 'w') return 'u';
	else if (inp == 's') return 'd';
	else if (inp == 'd') return 'r';
	else if (inp == 'a') return 'l';
	printf("Invalid Input\n");
	return 'e';		//Error - incorrect input
}

void copyArray(int toCopy) {
	for (int i = 0; i < side; ++i) {
		for (int j = 0; j < side; ++j) {
			if (toCopy) fieldCopy[i][j] = field[i][j];
			else field[i][j] = fieldCopy[i][j];
		}
	}
}

int main() {
	time_t t;
	srand((unsigned)time(&t));
	getFieldSize();
	generateField();
	char* end;
	while (1) {		//break inside the loop
		printField();
		char direction = 'e';
		while (direction == 'e') {
			direction = getSlideDirection();
			if (direction != 'e') {
				copyArray(1);
				if (!slideField(direction)) direction = 'e';
				copyArray(0);
			}
		}
		slideField(direction);
		end = checkEndGame();
		if (end != "Continue") break;
		generateRandom(1);
	}
	printField();
	printf(end);
	return 0;
}
