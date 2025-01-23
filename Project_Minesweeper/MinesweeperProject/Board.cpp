#include "Board.h"

Board::Board() {
	width = 0;
	height = 0;

	tiles = new Tile*[height];
	for (int i = 0; i < height; i++) {
		tiles[i] = new Tile[width];
	}
}

Board::Board(int width, int height, int numMines, map<string, sf::Texture>& imgs) {
	this->width = width;
	this->height = height;
	
	// creating 2D array for all tiles
	tiles = new Tile*[height];
	for (int i = 0; i < height; i++) {
		tiles[i] = new Tile[width];
	}


	// creates tiles for board
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Tile tile = Tile(j, i, imgs);
			tiles[i][j] = tile;

			// cout << "(" << i << ", " << j << ") ";
		}
		// cout << endl;
	}

	// giving each Tile object vector of its adj Tiles
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			// checks for if tile is on the edge of the board
			if (j - 1 >= 0) {
				tiles[i][j].AddAdjTile(&tiles[i][j - 1]); // l
			}
			if (j + 1 < width) {
				tiles[i][j].AddAdjTile(&tiles[i][j + 1]); // r
			}

			if (i - 1 >= 0) {
				tiles[i][j].AddAdjTile(&tiles[i - 1][j]); // tm
			}
			if (i + 1 < height) {
				tiles[i][j].AddAdjTile(&tiles[i + 1][j]); // bm
			}

			if (i - 1 >= 0 && j - 1 >= 0) {
				tiles[i][j].AddAdjTile(&tiles[i - 1][j - 1]); // tl
			}
			if (i - 1 >= 0 && j + 1 < width) {
				tiles[i][j].AddAdjTile(&tiles[i - 1][j + 1]); // tr
			}

			if (i + 1 < height && j - 1 >= 0) {
				tiles[i][j].AddAdjTile(&tiles[i + 1][j - 1]); // bl
			}
			if (i + 1 < height && j + 1 < width) {
				tiles[i][j].AddAdjTile(&tiles[i + 1][j + 1]); // br
			}	
		}
	}

	GenerateBoard(numMines);
	
}

void Board::Draw(sf::RenderWindow& window) {
	
	// converts all tile objects to sprites to be drawn
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tiles[i][j].Draw(window);
		}
	}
	
}

void Board::GenerateBoard(int mines) {
	Tile::lost = false;
	Tile::won = false;

	vector<vector<int>> spaces;
	vector<sf::Vector2i> mineCoords;

	// initialize vector 
	for (int i = 0; i < height; i++) {
		vector<int> temp;
		spaces.push_back(temp);
		for (int j = 0; j < width; j++) {
			spaces[i].push_back(0);
		}
	}

	bool valid = true;

	for (int i = 0; i < mines; i++) {
		valid = true;
		int x = rand() % height;
		int y = rand() % width;
		sf::Vector2i coord = sf::Vector2i(x, y);

		// check if coord is already generated
		for (int j = 0; j < mineCoords.size(); j++) {
			if (mineCoords[j].x == x && mineCoords[j].y == y) {
				valid = false;
				i--;
			}
		}
		if (valid) {
			mineCoords.push_back(coord);
		}
	}

	// turn coordinates into 2D std::vector of 1s and 0s
	for (int i = 0; i < mineCoords.size(); i++) {
		spaces[mineCoords[i].x][mineCoords[i].y] = 1;
	}

	Reset();

	// sets all tiles with 1s from vector to have mines
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (spaces[i][j] == 1) {
				tiles[i][j].AddMine();
			}
		}
	}

	GenerateTileNumbers();

}

int Board::LoadBoard(string path) {
	Tile::lost = false;
	Tile::won = false;

	// update mine count
	int mines = 0;
	string line;
	vector<vector<int>> spaces;

	ifstream file(path);

	// converts .brd file to 2d std::vector of 1s and 0s
	if (file.is_open()) {
		while (getline(file, line)) {
			vector<int> temp;
			
			for (int i = 0; i < line.size(); i++) {
				temp.push_back(stoi(line.substr(i, 1)));
			}
			spaces.push_back(temp);
		}
	}
	
	Reset();

	// sets all tiles with 1s from .brd file to have mines
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (spaces[i][j] == 1) {
				tiles[i][j].AddMine();
				mines++;
			}
		}
	}

	GenerateTileNumbers();

	return mines;
}

void Board::GenerateTileNumbers() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tiles[i][j].AddNumber();
		}
	}
}

void Board::Click(sf::Vector2i pos) {
	int mouse_x = (int)(pos.x / 32);
	int mouse_y = (int)(pos.y / 32);

	// checks if mouse is on a tile then reveals tile with same position
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (mouse_x == tiles[i][j].GetX() && mouse_y == tiles[i][j].GetY()) {
				tiles[i][j].Reveal();
			}
		}
	}

	// win condition
	bool win = true;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!(tiles[i][j].GetRevealed()) && !(tiles[i][j].GetMine())) {  // if any tile hasnt been revealed that doesnt have a min you didnt win
				win = false;
			}
		}
	}
	
	if (win) {
		Tile::won = true;
		Tile::isDebugMode = false;
	}
}

string Board::Flag(sf::Vector2i pos) {
	int mouse_x = (int)(pos.x / 32);
	int mouse_y = (int)(pos.y / 32);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (mouse_x == tiles[i][j].GetX() && mouse_y == tiles[i][j].GetY()) {
				string status = tiles[i][j].Flag();
				if (status == "flagged") {
					return "flagged";
				}
				else if (status == "unflagged") {
					return "unflagged";
				}
			}
		}
	}
	return "void";
}

void Board::ToggleDebugMode() {
	if (Tile::isDebugMode) {
		Tile::isDebugMode = false;
	}
	else {
		Tile::isDebugMode = true;
	}

	if (Tile::won || Tile::lost) {  // disables debug button when won or lost
		Tile::isDebugMode = false;
	}
	
}

void Board::Reset() {
	// wipe the board
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tiles[i][j].Reset();
		}
	}
}

Board::~Board() {
	for (int i = 0; i < height; i++) {
		delete[] tiles[i];
	}
	delete[] tiles;
}
