#include "Box.h"
#include "Board.h"
#include "SDL.h"
#include "Window.h"
#include <cstdlib>
void Box::render() {
	SDL_SetRenderDrawColor(Window::renderer, boxColor.r, boxColor.g, boxColor.b, boxColor.a);
	SDL_RenderFillRect(Window::renderer, &drawRect);
}


Box::Box() {

}

Box::~Box() {

}
char Box::getType() {
	return boxType;
}

void Box::init(int newRow, int newCol) {
	boxActive = false;
	activeLevel = 0;
	timeClicked = 0;
	row = newRow;
	col = newCol;
	boxColor.r = boxColor.g = boxColor.b = 0;
	boxColor.a = 255;
	switch (Board::color) {
	case 'r':
		boxColor.r = 255 - map(activeLevel, 0, Board::maxActiveLevel, 0, 255);
		break;
	case 'g':
		boxColor.g = 255 - map(activeLevel, 0, Board::maxActiveLevel, 0, 255);
		break;
	case 'b':
		boxColor.b = 255 - map(activeLevel, 0, Board::maxActiveLevel, 0, 255);
		break;

	default:
		std::cout << "what happened" << std::endl;
		break;
	}

	drawRect.x = col * Board::boxWidth;
	drawRect.y = row * Board::boxHeight;
	drawRect.w = Board::boxWidth;
	drawRect.h = Board::boxHeight;
	isWall =  isEmpty = false;
	boxType = 'e';
	visited = false;

}

void Box::toggleColor(SDL_Color highlightColor, char newType, int activeLevel) {
	this->activeLevel = activeLevel;
	if (activeLevel > 0) {
		boxActive = true;
	}
	
	timeClicked = SDL_GetTicks();
	boxColor.r = highlightColor.r;
	boxColor.g = highlightColor.g;
	boxColor.b = highlightColor.b;
	boxColor.a = highlightColor.a;
	isWall =  isEmpty = false;
	boxType = newType;
	switch (newType) {
	case 'w':
		isWall = true;
		break;
	
	case 'e':
		isEmpty = true;
		break;
	case 'p':
		isEmpty = true;
		break;

	default:
		break;
	}
}
void Box::clear() {

	boxColor.r = boxColor.g = boxColor.b =  0;
	boxColor.a = 255;
	switch (Board::color) {
	case 'r':
		boxColor.r = 255 - map(activeLevel, 0, Board::maxActiveLevel, 0, 255);
		break;
	case 'g':
		boxColor.g = 255 - map(activeLevel, 0, Board::maxActiveLevel, 0, 255);
		break;
	case 'b':
		boxColor.b = 255 - map(activeLevel, 0, Board::maxActiveLevel, 0, 255);
		break;

	default:
		std::cout << "what happened" << std::endl;
		break;
	}
	isEmpty = true;
	isWall = false;
	boxType = 'e';
}
bool Box::readyToSpread() {
	if (SDL_GetTicks() - timeClicked >= 100) {
		timeClicked = SDL_GetTicks();
		return true;
	}
	return false;
}



void Box::update() {
	if (activeLevel <= 0) {
		boxActive = false;
		activeLevel = 0;
		clear();
	}
	/*if (boxActive) {
		if (SDL_GetTicks() - timeClicked >= 1000) {
			timeClicked = SDL_GetTicks();
			clear();
		}
	}*/
	
	/*if (boxActive) {					REALLY COOL COLOR STUFF!!!!!!
		int choice = rand() % 3;
		switch (choice) {
		case 0:
			if (boxColor.g < 255) {
				boxColor.g++;
			}
			break;
		case 1:
			if (boxColor.r < 255) {
				boxColor.r++;
			}
			break;
		case 2:
			if (boxColor.b < 255) {
				boxColor.b++;
			}
			break;

		}
	
		if (boxColor.r == boxColor.g == boxColor.b == 255) {
			boxActive = false;
			clear();
		}
		
		/*if (SDL_GetTicks() - timeClicked >= 3000) {
			std::cout << "3 seconds passed" << std::endl;
			boxActive = false;
			clear();
		}*/
	//}
}
int map(int originalNumber, int origLow, int origHigh, int newLow, int newHigh) {

	float newNumber = float((originalNumber - origLow)) / (origHigh - origLow) * (newHigh - newLow) + newLow;

	return newNumber;
}
