#include "Tile.h"

bool Tile::isDebugMode = false;
bool Tile::lost = false;
bool Tile::won = false;

Tile::Tile() {
	pos.x = 0;
	pos.y = 0;

	isRevealed = false;
	hasFlag = false;
	hasMine = false;
	number = 0;
}

Tile::Tile(int x, int y, map<string, sf::Texture>& imgs) {
	pos.x = x;
	pos.y = y;
	this->imgs = imgs;

	isRevealed = false;
	hasFlag = false;
}

void Tile::Draw(sf::RenderWindow& window) {
	sf::Texture texture;
	sf::Sprite sprite;

	if (isRevealed) {
		texture = imgs["tile_revealed"];
	}
	else {
		texture = imgs["tile_hidden"];
	}

	sprite.setTexture(texture);
	sprite.setPosition(pos.x * 32.0f, pos.y * 32.0f);

	window.draw(sprite);


	if (hasFlag) {
		texture = imgs["flag"];
		sprite.setTexture(texture);
		window.draw(sprite);
	}

	// WIP ------------- change to account for debug mode / win loss condition
	if (hasMine && isRevealed) {
		lost = true;
		isDebugMode = false;
	}

	if (lost) {
		if (hasMine) {
			isRevealed = true;
		}
	}

	if (isDebugMode || lost) {
		if (hasMine) {
			texture = imgs["mine"];
			sprite.setTexture(texture);
			window.draw(sprite);
		}
	}

	if (won) {
		if (hasMine) {
			hasFlag = true;
		}
	}

	if (isRevealed && number != 0) {
		if (number == 1) {
			texture = imgs["number_1"];
		}
		else if (number == 2) {
			texture = imgs["number_2"];
		}
		else if (number == 3) {
			texture = imgs["number_3"];
		}
		else if (number == 4) {
			texture = imgs["number_4"];
		}
		else if (number == 5) {
			texture = imgs["number_5"];
		}
		else if (number == 6) {
			texture = imgs["number_6"];
		}
		else if (number == 7) {
			texture = imgs["number_7"];
		}
		else if (number == 8) {
			texture = imgs["number_8"];
		}

		sprite.setTexture(texture);
		window.draw(sprite);
	}

}

void Tile::AddAdjTile(Tile* tile) {
	adjTiles.push_back(tile);
}

void Tile::AddMine() {
	hasMine = true;
}

void Tile::AddNumber() {
	int count = 0;

	if (!hasMine) {
		// loops through adj tiles to count how many mines
		for (int i = 0; i < adjTiles.size(); i++) {
			if (adjTiles[i]->hasMine) {
				count++;
			}
		}
	}
	number = count;
}

void Tile::Reset() {
	hasMine = false;
	hasFlag = false;
	number = 0;
	isRevealed = false;
}

void Tile::Reveal() {
	if (!lost && !won) {
		if (!hasFlag) {
			isRevealed = true;
			
			if (number == 0 && !hasMine) {  // if tile completely empty
				for (int i = 0; i < adjTiles.size(); i++) {  
					if (!adjTiles[i]->isRevealed) {
						adjTiles[i]->Reveal();
					}
				}
			}
			
		}
	}
}

string Tile::Flag() {
	if (!lost && !won) {
		if (!isRevealed) {
			if (!hasFlag) {
				hasFlag = true;
				return "flagged";
			}
			else {
				hasFlag = false;
				return "unflagged";
			}
		}
	}
	return "void";
}

int Tile::GetX() {
	return pos.x;
}

int Tile::GetY() {
	return pos.y;
}

bool Tile::GetMine() {
	return hasMine;
}

bool Tile::GetRevealed() {
	return isRevealed;
}

void Tile::PrintAdjTiles() {
	for (int i = 0; i < adjTiles.size(); i++) {
		cout << "(" << adjTiles[i]->pos.y << ", " << adjTiles[i]->pos.x << ")" << endl;
	}
}

Tile::~Tile() {

}

