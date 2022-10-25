#include <stdio.h>

typedef struct {
	int row;
	int col;
	int value;
} tile;

tile** field;
int side = 4;

void getFieldSize() {	//Get user input for the side sizes
	printf("Enter side size (more than 2)\n");
	scanf_s("%d", &side);
}

void generateField() {		//Creates the initial field
	tile t;
	t.value = 0;
	field = (tile**)malloc(side * sizeof(tile*));
	for (int i = 0; i < side; ++i) {
		field[i] = (tile*)malloc(side * sizeof(tile));
		for (int j = 0; j < side; ++j) {
			t.row = i;
			t.col = j;
			t.value = 0;
			if (i == 1 && j == 1) t.value = 1;
			if (i == 3 && j == 1) t.value = 2;
			if (i == 3 && j == 3) t.value = 1;

			field[i][j] = t;
		}
	}
}

void printField() {		//Output the field
	for (int i = 0; i < side; ++i) {
		for (int j = 0; j < side; ++j) {
			printf("%d ", field[i][j].value);
		}
		printf("\n");
	}
	printf("\n");
}

void rotateField() {		//Rotate field left 90 degrees (so we only slide to one side)
	for (int i = 0; i < side / 2; ++i) {
		for (int j = i; j < side - i - 1; ++j) {
			tile t = field[i][j];
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
	printf("rotated\n");
	printField();
	for (int i = 0; i < side; ++i) {
		for (int j = 1; j < side; ++j) {
			if (field[i][j].value)
				if (checkSpot(field[i][j])) field[i][j].value = 0;
		}
	}
	if (rotation > 0) {		//If we rotated the field - return to its normal position
		for (int i = 0; i < 4 - rotation; ++i) {
			rotateField();
		}
	}
}

int checkSpot(tile t) {		//Check where a tile goes to (true if moved, false if stayed)
	int row = t.row;
	for (int i = 0; i < t.col; ++i) {
		if (field[i][row].value == 0) {
			field[i][row].value = t.value;
			printf("1 %d\n", i);
			printField();
			return 1;
		}
		else if (field[row][i].value == t.value) {
			field[row][i].value *= 2;
			printf("2\n");
			printField();
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
	//printField();
	return 0;
}
