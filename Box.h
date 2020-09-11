#pragma once
#include "SDL.h"
#include <iostream>
int map(int originalNumber, int origLow, int origHigh, int newLow, int newHigh); 
class Box
{
public:
	void render();
	Box();
	~Box();
	void toggleColor(SDL_Color, char,int);
	void init(int, int);
	void clear();
	char getType();
	
	void setVisited(bool newVisited) {
		visited = newVisited;
	}
	bool getVisited() {
		return visited;
	}
	int getCol() {
		return col;
	}
	int getRow() {
		return row;
	}
	void update();
	int getActiveLevel() {
		return activeLevel;
	}
	void updateActiveLevel(int newLevel) {
		activeLevel = newLevel;
	}
	bool readyToSpread();
	bool isActive() {
		return boxActive;
	}
private:
	SDL_Color boxColor;
	SDL_Rect drawRect;
	int row;
	int col;
	char boxType;

	bool isWall;
	
	bool isEmpty;
	bool visited;
	int aliveTime;
	bool boxActive;
	int activeLevel;
	Uint32 timeClicked;
};

