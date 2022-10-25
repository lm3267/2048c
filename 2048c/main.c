#include <stdio.h>

int** field;
int side = 4;

void getFieldSize() {	//Get user input for the side sizes
	printf("Enter side size (more than 2)\n");
	scanf_s("%d", &side);
}

void generateField() {		//Creates the initial field
	field = (int**)malloc(side * sizeof(int*));
	for (int i = 0; i < side; ++i) {
		field[i] = (int*)malloc(side * sizeof(int));
		for (int j = 0; j < side; ++j) {

			field[i][j] = 0;
			if (i == 1 && j == 1) field[i][j] = 1;
			if (i == 3 && j == 1) field[i][j] = 2;
			if (i == 3 && j == 3) field[i][j] = 1;

		}
	}
}

void printField() {		//Output the field
	for (int i = 0; i < side; ++i) {
		for (int j = 0; j < side; ++j) {
			printf("%d ", field[i][j]);
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

void slideField(char direction) {		//Slide field in a given direction
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
	for (int i = 0; i < side; ++i) {
		for (int j = 1; j < side; ++j) {
			if (field[i][j] && checkSpot(i, j)) {
				field[i][j] = 0;
				//printField();
			}
		}
	}
	if (rotation > 0) {		//If we rotated the field - return to its normal position
		for (int i = 0; i < 4 - rotation; ++i) {
			rotateField();
		}
	}
}

int checkSpot(int row, int col) {		//Check where a tile goes to (true if moved, false if stayed)
	int value = field[row][col];
	for (int i = 0; i < col; ++i) {
		if (field[row][i] == 0) {
			field[row][i] = value;
			//printf("1 %d\n", i);
			return 1;
		}
		else if (field[row][i] == value) {
			field[row][i] *= 2;
			//printf("2\n");
			return 1;
		}
	}
	return 0;
}

int main() {
	//getFieldSize();
	generateField();
	printField();
	//rotateField();
	//printField();
	//rotateField();
	slideField('r');
	printField();
	slideField('u');
	printField();
	slideField('r');
	printField();
	return 0;
}
