#pragma once
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;


class Tile {
	bool isRevealed;
	bool hasMine;
	bool hasFlag;
	int number;

	map<string, sf::Texture> imgs;
	sf::Vector2i pos;
	vector<Tile*> adjTiles;

public:
	static bool isDebugMode;
	static bool lost;
	static bool won;

	Tile();
	Tile(int x, int y, map<string, sf::Texture>& imgs);
	void Draw(sf::RenderWindow& window);
	void AddAdjTile(Tile* tile);
	void AddMine();
	void AddNumber();
	void Reset();

	void Reveal();
	string Flag();

	int GetX();
	int GetY();
	bool GetMine();
	bool GetRevealed();

	void PrintAdjTiles(); // test to see if vector<Tile*> is created correctly

	~Tile();
};
