#pragma once
#include "Tile.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
using namespace std;


class Board {
	int width;
	int height;
	Tile** tiles;

public:
	Board();
	Board(int width, int height, int numMines, map<string, sf::Texture>& imgs);
	void Draw(sf::RenderWindow& window);
	void GenerateBoard(int mines);
	int LoadBoard(string path); // returns new mine count for main() to update
	void GenerateTileNumbers();

	void ToggleDebugMode();

	void Click(sf::Vector2i pos);
	string Flag(sf::Vector2i pos);
	void Reset();

	~Board();
	
};
