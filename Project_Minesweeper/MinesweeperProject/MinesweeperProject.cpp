#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Board.h"
using namespace std;

int main(){

    //read config file
    string line;
    vector<string> lines;
    ifstream file("boards/config.cfg");

    if (file.is_open()) {
        while (getline(file, line)){
            lines.push_back(line);
        }
    }
    int WIDTH = stoi(lines[0]);
    int HEIGHT = stoi(lines[1]);

    int mineCountCFG = stoi(lines[2]); // stores og mineCount from config file in case it needs to be reverted
    int mineCount = mineCountCFG;
    int tileCount = WIDTH * HEIGHT;

    file.close();

    // save img data
    map<string, sf::Texture> textures;
    sf::Texture texture;
    sf::Sprite sprite;

    sf::Sprite counter_sprite;

    texture.loadFromFile("images/tile_hidden.png");
    textures["tile_hidden"] = texture;
    texture.loadFromFile("images/tile_revealed.png");
    textures["tile_revealed"] = texture;

    texture.loadFromFile("images/face_happy.png");
    textures["face_happy"] = texture;
    texture.loadFromFile("images/face_lose.png");
    textures["face_lose"] = texture;
    texture.loadFromFile("images/face_win.png");
    textures["face_win"] = texture;

    texture.loadFromFile("images/debug.png");
    textures["debug"] = texture;
    texture.loadFromFile("images/test_1.png");
    textures["test_1"] = texture;
    texture.loadFromFile("images/test_2.png");
    textures["test_2"] = texture;
    texture.loadFromFile("images/test_3.png");
    textures["test_3"] = texture;

    texture.loadFromFile("images/number_1.png");
    textures["number_1"] = texture;
    texture.loadFromFile("images/number_2.png");
    textures["number_2"] = texture;
    texture.loadFromFile("images/number_3.png");
    textures["number_3"] = texture;
    texture.loadFromFile("images/number_4.png");
    textures["number_4"] = texture;
    texture.loadFromFile("images/number_5.png");
    textures["number_5"] = texture;
    texture.loadFromFile("images/number_6.png");
    textures["number_6"] = texture;
    texture.loadFromFile("images/number_7.png");
    textures["number_7"] = texture;
    texture.loadFromFile("images/number_8.png");
    textures["number_8"] = texture;

    texture.loadFromFile("images/flag.png");
    textures["flag"] = texture;
    texture.loadFromFile("images/mine.png");
    textures["mine"] = texture;

    // mine counter number imgs
    vector<string> counter_nums;
    texture.loadFromFile("images/digits.png", sf::IntRect(0, 0, 21, 32));
    textures["counter_0"] = texture;
    texture.loadFromFile("images/digits.png", sf::IntRect(21, 0, 21, 32));
    textures["counter_1"] = texture;
    texture.loadFromFile("images/digits.png", sf::IntRect(42, 0, 21, 32));
    textures["counter_2"] = texture;
    texture.loadFromFile("images/digits.png", sf::IntRect(63, 0, 21, 32));
    textures["counter_3"] = texture;
    texture.loadFromFile("images/digits.png", sf::IntRect(84, 0, 21, 32));
    textures["counter_4"] = texture;
    texture.loadFromFile("images/digits.png", sf::IntRect(105, 0, 21, 32));
    textures["counter_5"] = texture;
    texture.loadFromFile("images/digits.png", sf::IntRect(126, 0, 21, 32));
    textures["counter_6"] = texture;
    texture.loadFromFile("images/digits.png", sf::IntRect(147, 0, 21, 32));
    textures["counter_7"] = texture;
    texture.loadFromFile("images/digits.png", sf::IntRect(168, 0, 21, 32));
    textures["counter_8"] = texture;
    texture.loadFromFile("images/digits.png", sf::IntRect(189, 0, 21, 32));
    textures["counter_9"] = texture;
    texture.loadFromFile("images/digits.png", sf::IntRect(210, 0, 21, 32));
    textures["counter_-"] = texture;

    counter_nums.push_back("counter_0");
    counter_nums.push_back("counter_1");
    counter_nums.push_back("counter_2");
    counter_nums.push_back("counter_3");
    counter_nums.push_back("counter_4");
    counter_nums.push_back("counter_5");
    counter_nums.push_back("counter_6");
    counter_nums.push_back("counter_7");
    counter_nums.push_back("counter_8");
    counter_nums.push_back("counter_9");

    
    // create board
    Board board = Board(WIDTH, HEIGHT, mineCount, textures);

    sf::Color WHITE = sf::Color(255, 255, 255);

    // create window
    sf::RenderWindow window(sf::VideoMode(WIDTH * 32, HEIGHT * 32 + 100), "Minesweeper");

    bool lock_mouse = false; // mouse variable to prevent holding down

    // while window is open
    while (window.isOpen()){

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonReleased) {
                lock_mouse = false;
            }
        }

        window.clear(WHITE);
        
        // draw       
        // draws all tiles
        board.Draw(window);

        // draws smiley
        texture = textures["face_happy"];
        if (Tile::lost) {
            texture = textures["face_lose"];
        }
        if (Tile::won) {
            texture = textures["face_win"];
            mineCount = 0;
        }
        sprite.setTexture(texture);
        sprite.setPosition((WIDTH * 32.0f) / 2 - 32, HEIGHT * 32.0f);
        window.draw(sprite);


        // draws all test buttons
        texture = textures["debug"];
        sprite.setTexture(texture);
        sprite.setPosition(WIDTH * 32.0f - 256, HEIGHT * 32.0f);
        window.draw(sprite);

        texture = textures["test_1"];
        sprite.setTexture(texture);
        sprite.setPosition(WIDTH * 32.0f - 192, HEIGHT * 32.0f);
        window.draw(sprite);

        texture = textures["test_2"];
        sprite.setTexture(texture);
        sprite.setPosition(WIDTH * 32.0f - 128, HEIGHT * 32.0f);
        window.draw(sprite);

        texture = textures["test_3"];
        sprite.setTexture(texture);
        sprite.setPosition(WIDTH * 32.0f - 64, HEIGHT * 32.0f);
        window.draw(sprite);

        // draws mine counter
        if (mineCount < 0) {  // negative sign
            texture = textures["counter_-"];
            counter_sprite.setTexture(texture);
            counter_sprite.setPosition(0, HEIGHT * 32.0f);
            window.draw(counter_sprite);

            texture = textures[counter_nums[-mineCount / 100]];
            counter_sprite.setTexture(texture);
            counter_sprite.setPosition(21, HEIGHT * 32.0f);
            window.draw(counter_sprite);

            texture = textures[counter_nums[(-mineCount / 10) % 10]];
            counter_sprite.setTexture(texture);
            counter_sprite.setPosition(42, HEIGHT * 32.0f);
            window.draw(counter_sprite);

            texture = textures[counter_nums[-mineCount % 10]];
            counter_sprite.setTexture(texture);
            counter_sprite.setPosition(63, HEIGHT * 32.0f);
            window.draw(counter_sprite);
        }

        else {
            texture = textures[counter_nums[mineCount / 100]];
            counter_sprite.setTexture(texture);
            counter_sprite.setPosition(21, HEIGHT * 32.0f);
            window.draw(counter_sprite);

            texture = textures[counter_nums[(mineCount / 10) % 10]];
            counter_sprite.setTexture(texture);
            counter_sprite.setPosition(42, HEIGHT * 32.0f);
            window.draw(counter_sprite);

            texture = textures[counter_nums[mineCount % 10]];
            counter_sprite.setTexture(texture);
            counter_sprite.setPosition(63, HEIGHT * 32.0f);
            window.draw(counter_sprite);
        }
        
        
    
        window.display();

        // mouse input
        // left click
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !lock_mouse) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // if mouse is on tiles
            if ((mousePos.y > 0 && mousePos.y < HEIGHT * 32) && (mousePos.x > 0 && mousePos.x < WIDTH * 32)) {
                board.Click(mousePos);
            }


            // smiley button
            if ((mousePos.x > (WIDTH * 32) / 2 - 32 && mousePos.x < (WIDTH * 32) / 2 + 32) && (mousePos.y > HEIGHT * 32 && mousePos.y < HEIGHT * 32 + 64)) {
                board.GenerateBoard(mineCountCFG);
                mineCount = mineCountCFG;
            }


            // debug
            if ((mousePos.x > WIDTH * 32 - 256 && mousePos.x < WIDTH * 32 - 192) && (mousePos.y > HEIGHT * 32 && mousePos.y < HEIGHT * 32 + 64)) {
                board.ToggleDebugMode();
            }

            // test 1
            if ((mousePos.x > WIDTH * 32 - 192 && mousePos.x < WIDTH * 32 - 128) && (mousePos.y > HEIGHT * 32 && mousePos.y < HEIGHT * 32 + 64)) {
                // cout << "Clicked test 1 " << endl;
                mineCount = board.LoadBoard("boards/testboard1.brd");
            }

            // test 2
            if ((mousePos.x > WIDTH * 32 - 128 && mousePos.x < WIDTH * 32 - 64) && (mousePos.y > HEIGHT * 32 && mousePos.y < HEIGHT * 32 + 64)) {
                // cout << "Clicked test 2 " << endl;
                mineCount = board.LoadBoard("boards/testboard2.brd");
            }

            // test 3
            if ((mousePos.x > WIDTH * 32 - 64 && mousePos.x < WIDTH * 32) && (mousePos.y > HEIGHT * 32 && mousePos.y < HEIGHT * 32 + 64)) {
                // cout << "Clicked test 3 " << endl;
                mineCount = board.LoadBoard("boards/testboard3.brd");
            }

            lock_mouse = true; // lock mouse from holding down
        }

        // right click
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !lock_mouse) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // if mouse is on tiles
            if ((mousePos.y > 0 && mousePos.y < HEIGHT * 32) && (mousePos.x > 0 && mousePos.x < WIDTH * 32)) {
                string status = board.Flag(mousePos);
                if (status == "flagged") {
                    mineCount--;
                }
                else if (status == "unflagged") {
                    mineCount++;
                }
            }

            lock_mouse = true; // lock mouse from holding down
        }


    }

    return 0;
}
