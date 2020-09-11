#include "Board.h"
#include "Window.h"
#include "Box.h"
#include "boxStack.h"

#include "SDL.h"
#include <iostream>

#define MAXROWS 200
#define MAXCOLS 200
int Board::boxHeight = 0;
int Board::boxWidth = 0;
int Board::maxActiveLevel = 25;
int Board::color = 'b';
void Board::render() {
	renderBoxes();
}

void Board::renderBoxes() {


	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			boxes[i][j].render();
		}
	}
	renderGrid();
}

void Board::renderGrid() {
	SDL_Rect backgroundRect;
	backgroundRect.w = Board::boxWidth;
	backgroundRect.h = Board::boxHeight;
	SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 255);	//sets the color to black
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			backgroundRect.x = j * boxWidth;
			backgroundRect.y = i * boxHeight;
			SDL_RenderDrawRect(Window::renderer, &backgroundRect);
		}
	}
}


Board::Board() {

}

Board::~Board() {
	for (int i = 0; i < rows; i++) {
		delete[] boxes[i];
	}
	delete[] boxes;
}


void Board::init(int newRows, int newCols) {
	rows = newRows;
	cols = newCols;
	boxWidth = Window::screenWidth / cols;
	boxHeight = Window::screenHeight / rows;
	boxes = new Box*[rows];
	for (int i = 0; i < rows; i++) {
		boxes[i] = new Box[cols];
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			boxes[i][j].init(i, j);
		}
	}
	backgroundColor.r = backgroundColor.g = backgroundColor.b = highlightColor.r = highlightColor.g = highlightColor.b= 0;
	backgroundColor.a = highlightColor.a = 255;
	switch (color) {
	case 'r':
		backgroundColor.r = 255;
		
		break;
	case 'g':
		backgroundColor.g = 255;
		break;
	case 'b':
		backgroundColor.b = 255;
		break;
	default:
		std::cout << "help us all" << std::endl;
	}
	
	mouseDragged = false;
	mouseRow = -1;
	mouseCol = -1;

	change = 5;
	timeOfLastRandom = 0;
	maxTimeBetweenRandom = 50;			//100 milliseconds between a random square getting highlighted.
}

void Board::getMousePos(int& clickRow, int& clickCol) {
	int x;
	int y;
	SDL_GetMouseState(&x, &y);
	 clickCol = x / boxWidth;
	 clickRow = y / boxHeight;
}

void Board::handleMouseDown(SDL_MouseButtonEvent button) {


	switch (button.button) {
	case SDL_BUTTON_LEFT:
		highlightColor.r = highlightColor.g = 0;
		 highlightColor.b = 0;
		highlightColor.a = 255;
		mouseType = 'w';
		break;
	case SDL_BUTTON_RIGHT:
		highlightColor.r = backgroundColor.r;
		highlightColor.g = backgroundColor.g;
		highlightColor.b = backgroundColor.b;
		highlightColor.a = backgroundColor.a;
		mouseType = 'e';
		break;
	}
	mouseDragged = true;

}






void Board::handleMouseUp(SDL_MouseButtonEvent button) {
	mouseDragged = false;
	mouseRow = -1;
	mouseCol = -1;

}
bool Board::mouseDragging() {
	return mouseDragged;
}


void Board::update() {
	if(mouseDragged) {
		int currentRow, currentCol;
		getMousePos(currentRow, currentCol);
		if (mouseRow != currentRow || mouseCol != currentCol) {
			if (currentRow < rows && currentCol < cols) {
				mouseRow = currentRow;
				mouseCol = currentCol;
			}
			if (highlightColor.r == backgroundColor.r && highlightColor.g == backgroundColor.g && highlightColor.b == backgroundColor.b) {
				boxes[mouseRow][mouseCol].toggleColor(backgroundColor, mouseType, 0);
			}
			else {
				boxes[mouseRow][mouseCol].toggleColor(highlightColor, mouseType, maxActiveLevel);
				
			}
		}

	}
	updateBoxes();
	/*if (SDL_GetTicks() - timeOfLastRandom >= maxTimeBetweenRandom) {	//comment this out to make random squares stop
		timeOfLastRandom = SDL_GetTicks();
		activateRandomSquare();
	}*/
}

void Board::activateRandomSquare() {
	int row = rand() % rows;
	int col = rand() % cols;
	int newLevel = (rand() % (maxActiveLevel - 5)) + 5;
	SDL_Color newColor;
	newColor.r = newColor.g = newColor.b = 0;
	newColor.a = 255;
	switch (color) {
	case 'r':
		newColor.r = 255 - map(newLevel, 0, maxActiveLevel, 0, 255);
		break;
	case 'g':
		newColor.g = 255 - map(newLevel, 0, maxActiveLevel, 0, 255);
		break;
	case 'b':
		newColor.b = 255 - map(newLevel, 0, maxActiveLevel, 0, 255);
		break;

	default:
		std::cout << "what happened" << std::endl;
		break;
	}
	boxes[row][col].toggleColor(newColor, mouseType,newLevel );

}


void Board::updateBoxes() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (boxes[i][j].isActive()) {
				if (boxes[i][j].readyToSpread()) {
					updateBox(i, j);
				}
			}
			boxes[i][j].update();
		}
	}
}

void Board::updateBox(int row, int col) {
	int boxActiveLevel = boxes[row][col].getActiveLevel();
	int newLevel = boxActiveLevel - 1;
	SDL_Color newColor;
	newColor.r = newColor.g = newColor.b = 0;
	newColor.a = 255;
	switch (color) {
	case 'r':
		newColor.r = 255 - map(newLevel, 0, maxActiveLevel, 0, 255);
		break;
	case 'g':
		newColor.g = 255 - map(newLevel, 0, maxActiveLevel, 0, 255);
		break;
	case 'b':
		newColor.b = 255 - map(newLevel, 0, maxActiveLevel, 0, 255);
		break;

	default:
		std::cout << "what happened" << std::endl;
		break;
	}
	

	boxes[row][col].toggleColor(newColor, 'w', newLevel);
	attemptUpdateBox(row - 1, col, newLevel);
	attemptUpdateBox(row + 1, col, newLevel);
	attemptUpdateBox(row, col - 1, newLevel);
	attemptUpdateBox(row, col + 1, newLevel);
}

void Board::attemptUpdateBox(int row, int col, int activeLevel) {
	if (row < 0 || col < 0 || row >= rows || col >= cols) {
		return;
	}
	else {
		SDL_Color newColor;
		newColor.r = newColor.g = newColor.b = 0;
		newColor.a = 255;
		switch (color) {
		case 'r':
			newColor.r = 255 - map(activeLevel, 0, maxActiveLevel, 0, 255);
			break;
		case 'g':
			newColor.g = 255 - map(activeLevel, 0, maxActiveLevel, 0, 255);
			break;
		case 'b':
			newColor.b = 255 - map(activeLevel, 0, maxActiveLevel, 0, 255);
			break;

		default:
			std::cout << "what happened" << std::endl;
			break;
		}
		int currentLevel = boxes[row][col].getActiveLevel();
		if (currentLevel < activeLevel) {
		
			boxes[row][col].toggleColor(newColor, 'w', activeLevel );
		
		}
		//}
	}
}



//s for start, f for finish
void Board::keyPress(char key) {

	switch (key) {
	
	case 'c':
		clearAll();
		break;
	default:
		break;
	}
}


int Board::calculatePathLength(Box* startingBox) {
	int length = -2;	//minus 2 for the start and end.
	while (startingBox != NULL) {
		length++;
	
	}
	return length;
}





void Board::resetSearch() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			boxes[i][j].setVisited(false);
			if (boxes[i][j].getType() != 'w') {
				boxes[i][j].clear();
			}
		}
	}
}


void Board::clearAll() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
				boxes[i][j].clear();
		}
	}
}

void Board::deleteBoard() {
	for (int i = 0; i < rows; i++) {
		delete[] boxes[i];
	}
	delete[] boxes;

}


void Board::addRows() {
	if (rows + change <= MAXROWS) {
		deleteBoard();
		rows += change;
		init(rows, cols);
	}
	else {
		deleteBoard();
		rows = MAXROWS;
		init(rows, cols);
	}

}

void Board::addCols() {
	if (cols + change <= MAXCOLS) {
		deleteBoard();
		cols += change;
		init(rows, cols);
	}
	else {
		deleteBoard();
		cols = MAXCOLS;
		init(rows, cols);
	}
}

void Board::removeRows() {
	if (rows - change >= 2) {
		deleteBoard();
		rows -= change;
		init(rows, cols);
	}
	else {
		deleteBoard();
		rows = 2;
		init(rows, cols);
	}
}

void Board::removeCols() {
	if (cols - change >= 2) {
		deleteBoard();
		cols -= change;
		init(rows, cols);
	}
	else {
		deleteBoard();
		cols = 2;
		init(rows, cols);
	}
}