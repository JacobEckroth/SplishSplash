#pragma once
#include "Box.h"
#include "SDL.h"
#include "boxStack.h"
class Board
{

public:
	void render();
	void renderBoxes();
	void renderGrid();
	Board();
	~Board();
	void init(int rows, int cols);
	void handleMouseDown(SDL_MouseButtonEvent);
	void handleMouseUp(SDL_MouseButtonEvent);
	static int boxWidth;
	static int boxHeight;
	bool mouseDragging();
	void update();
	void keyPress(char key);
	
	void getMousePos(int&, int&);

	void resetSearch();

	void clearAll();
	void updateBox(int, int);
	void updateBoxes();
	void addRows();
	void addCols();
	void removeRows();
	void removeCols();
	void deleteBoard();
	int calculatePathLength(Box*);
	static int maxActiveLevel;
	static int color;
	void attemptUpdateBox(int, int, int);
	void activateRandomSquare();

private:
	int rows;
	int cols;
	Box** boxes;
	bool mouseDragged;
	int mouseRow;
	int mouseCol;
	char mouseType;
	SDL_Color highlightColor;

	SDL_Color backgroundColor;
	int change;
	Uint32 timeOfLastRandom;
	int maxTimeBetweenRandom;

	
};

