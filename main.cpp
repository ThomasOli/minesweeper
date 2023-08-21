#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "tile.h"
#include <cmath>
using namespace sf;
using namespace std;

void setText(sf::Text& text, float x, float y) {
	 FloatRect textRect = text.getLocalBounds();
	 text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	 text.setPosition(Vector2f(x, y));
}

bool hasElapsedOneSecond(sf::Clock& clock)
{
	return clock.getElapsedTime().asSeconds() >= 1.0f;
}

void gameover(Texture& face, bool& debug, bool& paused) {
	face.loadFromFile("images\\/face_lose.png");
	debug = true;
	paused = true;
}
void gamewon(Texture& face, bool& paused) {
	face.loadFromFile("images\\/face_win.png");
	
	paused = true;
}

void resetboard(Tile**& tileGrid, int columns, int rows, int minecount, int& second, int& minute, int& minuteone, int& minuteten, int& flagsplaced, bool& paused, bool& debug, int& tilesrevealed, bool& added) {
	int minesonboard = 0;
	int tilesplaced = 0;
	int minesplaced = 0;
	second = -1;
	minute = 0;
	minuteone = 0;
	minuteten = 0;
	flagsplaced = 0;
	debug = false;
	paused = false;
	tilesrevealed = 0;
	added = false;
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			Tile boardtiles = Tile(x, y, 0, 0, 0);
			minesonboard = rand() % (static_cast<int>(columns * rows) - tilesplaced);
			if (minesonboard < minecount - minesplaced) {
				boardtiles.sethasmine();

				tilesplaced++;
				minesplaced++;
				boardtiles.setNeighbors(tileGrid, rows, columns, x, y);
				tileGrid[y][x] = boardtiles;
			}
			else {

				tilesplaced++;
				boardtiles.setNeighbors(tileGrid, rows, columns, x, y);
				tileGrid[y][x] = boardtiles;
			}
		}
	}

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			int minesonboard = 0;

			for (int z = 0; z < tileGrid[y][x].getsizeofneighbors(); z++) {

				if (tileGrid[y][x].getVector().at(z)->gethasmine()) {
					minesonboard++;
				}
			}
			tileGrid[y][x].setsurrondingmines(minesonboard);

		}
	}
}

void revealtiles(Tile& x, bool reveal, int& tilesrevealed) {

	if (x.getrevealed() || x.gethasmine()) {
		return;
	}
	if (x.getsurrondingmines() == 0) {
		if (!x.gethasflag()) {
			x.setrevealed();
			tilesrevealed++;
			
		}
		std::vector<Tile*> neighbors = x.getVector();

		for (Tile* neighbor : neighbors) {
			if (!neighbor->gethasmine()) {
				revealtiles(*neighbor,true, tilesrevealed);
			}
		}
	}
	else if (reveal) {
		x.setrevealed();
		tilesrevealed++;
		
		return;
	}
	
}


int main()
{
	srand(time(0));
	float height, width, columns, rows;
	int minecount;
	ifstream config("board_config.cfg", ifstream::in);
	if (config.is_open()) {
		config >> columns;
		config >> rows;
		config >> minecount;
	}

	Font font;
	font.loadFromFile("font.ttf");
	Text text;
	Text inputprompt;
	Text usertype;
	usertype.setString("|");
	string username;

	height = (rows * 32) + 100;
	width = columns * 32;
	int rows1 = rows;
	int columns1 = columns;
	Tile** tileGrid = new Tile * [rows1];
	for (int i = 0; i < rows; ++i) {
		tileGrid[i] = new Tile[columns1];
	}

	inputprompt.setFont(font);
	text.setFont(font);
	usertype.setFont(font);

	RenderWindow welcomewindow(sf::VideoMode(width, height), "Welcome Window", sf::Style::Default);
	RenderWindow gamewindow;
	RenderWindow leaderboardwindow;


	text.setString("WELCOME TO MINESWEEPER!");
	text.setCharacterSize(24);
	text.setFillColor(Color::White);
	text.setStyle(Text::Bold | Text::Underlined);
	setText(text, width / 2.0f, height / 2.0f - 150);

	inputprompt.setString("Enter your name:");
	inputprompt.setCharacterSize(20);
	inputprompt.setFillColor(Color::White);
	inputprompt.setStyle(Text::Bold);
	setText(inputprompt, width / 2.0f, height / 2.0f - 75);

	usertype.setCharacterSize(18);
	usertype.setFillColor(Color::Yellow);
	usertype.setStyle(Text::Bold);
	setText(usertype, width / 2.0f, height / 2.0f - 45);

	while (welcomewindow.isOpen())
	{
		Event event;

		welcomewindow.clear(Color(0, 0, 255));
		welcomewindow.draw(text);
		welcomewindow.draw(inputprompt);


		while (welcomewindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				welcomewindow.close();

			if (event.type == sf::Event::TextEntered)
			{
				if (isalpha(static_cast<char>(event.text.unicode)) && username.length() < 10) {
					if (username.length() == 0) {
						username.push_back(toupper(static_cast<char>(event.text.unicode)));
						usertype.setString(username + "|");
						setText(usertype, width / 2.0f, height / 2.0f - 45);
						continue;
					}
					username.push_back(tolower(static_cast<char>(event.text.unicode)));
					usertype.setString(username + "|");
					setText(usertype, width / 2.0f, height / 2.0f - 45);
				}
			}
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::BackSpace)
				{
					if (username.length() > 0) {
						username.pop_back();
						usertype.setString(username + "|");
						setText(usertype, width / 2.0f, height / 2.0f - 45);
					}
				}
				if (event.key.code == Keyboard::Enter && username.length() > 0)
				{

					gamewindow.create(sf::VideoMode(width, height), "Minesweeper", sf::Style::Default);
					welcomewindow.close();


				}
			}
		}
		welcomewindow.draw(usertype);
		welcomewindow.display();
	}

	Clock clock;
	Texture tilehidden, face, debug, pause, leaderboard, timer, flag, mines, tilerevealed, tilenumber;

	face.loadFromFile("images\\/face_happy.png");
	debug.loadFromFile("images\\/debug.png");
	tilehidden.loadFromFile("images\\/tile_hidden.png");
	pause.loadFromFile("images\\/pause.png");
	leaderboard.loadFromFile("images\\/leaderboard.png");
	timer.loadFromFile("images\\/digits.png");
	flag.loadFromFile("images\\/flag.png");
	mines.loadFromFile("images\\/mine.png");
	tilerevealed.loadFromFile("images\\/tile_revealed.png");
	tilenumber.loadFromFile("images\\/number_1.png");

	Sprite spritem(mines);
	Sprite spritefl(flag);
	Sprite spritet(tilehidden);
	Sprite sprited(debug);
	Sprite spritep(pause);
	Sprite spritel(leaderboard);
	Sprite spritef(face);
	Sprite spritetimerm(timer);
	Sprite spritetimers(timer);
	Sprite spritetimerm1(timer);
	Sprite spritetimers1(timer);
	Sprite spritenumber(tilenumber);
	Sprite spriterevealedtile(tilerevealed);

	Sprite counter1(timer);
	Sprite counter2(timer);
	Sprite counter3(timer);
	Sprite counter4(timer);

	bool paused = false;
	bool debugmode = false;
	bool gamestate = true;
	bool leaderboardopen = false;
	int second = 0;
	int secondone, secondten;

	int minute = 0;
	int minuteone = 0;
	int minuteten = 0;
	int flagsplaced = 0;
	int minesonboard = 0;
	int minesplaced = 0;
	int tilesplaced = 0;
	int tilesrevealed = 0;
	float mouseposx, mouseposy;

	counter1.setPosition(33, 32 * ((rows)+0.5f) + 16);
	counter2.setPosition(54, 32 * ((rows)+0.5f) + 16);
	counter3.setPosition(75, 32 * ((rows)+0.5f) + 16);
	counter4.setPosition(12, 32 * ((rows)+0.5f) + 16);

	spritetimerm.setPosition(((columns) * 32) - 97, 32 * ((rows)+0.5f) + 16);
	spritetimers.setPosition(((columns) * 32) - 54, 32 * ((rows)+0.5f) + 16);
	spritetimerm1.setPosition(((columns) * 32) - 76, 32 * ((rows)+0.5f) + 16);
	spritetimers1.setPosition(((columns) * 32) - 33, 32 * ((rows)+0.5f) + 16);

	sprited.setPosition((columns * 32) - 304, 32 * ((rows)+0.5f));
	spritef.setPosition((((columns) / 2.0) * 32) - 32, 32 * ((rows)+0.5f));
	spritep.setPosition(((columns) * 32) - 240, 32 * ((rows)+0.5f));
	spritel.setPosition(((columns) * 32) - 176, 32 * ((rows)+0.5f));

	spritetimerm.setTextureRect(sf::IntRect(0, 0, 21, 32));
	spritetimers.setTextureRect(sf::IntRect(0, 0, 21, 32));
	spritetimerm1.setTextureRect(sf::IntRect(0, 0, 21, 32));
	spritetimers1.setTextureRect(sf::IntRect(0, 0, 21, 32));

	counter1.setTextureRect(sf::IntRect(21 * (minecount / 100), 0, 21, 32));
	counter2.setTextureRect(sf::IntRect(21 * ((minecount / 10) % 10), 0, 21, 32));
	counter3.setTextureRect(sf::IntRect(21 * (minecount % 10), 0, 21, 32));
	counter4.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));


	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			Tile boardtiles = Tile(x, y, 0, 0, 0);
			minesonboard = rand() % (static_cast<int>(columns * rows) - tilesplaced);
			if (minesonboard < minecount - minesplaced) {
				boardtiles.sethasmine();

				tilesplaced++;
				minesplaced++;
				boardtiles.setNeighbors(tileGrid, rows, columns, x, y);
				tileGrid[y][x] = boardtiles;
			}
			else {

				tilesplaced++;
				boardtiles.setNeighbors(tileGrid, rows, columns, x, y);
				tileGrid[y][x] = boardtiles;
			}
		}
	}

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			int minesonboard = 0;

			for (int z = 0; z < tileGrid[y][x].getsizeofneighbors(); z++) {

				if (tileGrid[y][x].getVector().at(z)->gethasmine()) {
					minesonboard++;
				}
			}
			tileGrid[y][x].setsurrondingmines(minesonboard);

		}
	}
	minesonboard = minesplaced;

	bool added = false;

	while (gamewindow.isOpen())
	{

		Event event;

		gamewindow.clear(Color(255, 255, 255));

		counter1.setTextureRect(sf::IntRect(21 * (abs(minecount - flagsplaced) / 100), 0, 21, 32));
		counter2.setTextureRect(sf::IntRect(21 * ((abs(minecount - flagsplaced) / 10) % 10), 0, 21, 32));
		counter3.setTextureRect(sf::IntRect(21 * (abs(minecount - flagsplaced) % 10), 0, 21, 32));

		if (minecount - flagsplaced < 0) {
			gamewindow.draw(counter4);
		}


		for (int y = 0; y < rows; y++)
		{
			for (int x = 0; x < columns; x++)
			{
				if (tileGrid[y][x].getrevealed()) {
					if (tileGrid[y][x].getsurrondingmines() > 0) {
						tilenumber.loadFromFile("images\\/number_" + to_string(tileGrid[y][x].getsurrondingmines()) + ".png");
						spriterevealedtile.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
						spritenumber.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
						gamewindow.draw(spriterevealedtile);
						gamewindow.draw(spritenumber);

						continue;
					}
					else {
						tilenumber.loadFromFile("images\\/tile_revealed.png");
						spritenumber.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
						spriterevealedtile.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
						gamewindow.draw(spriterevealedtile);
						gamewindow.draw(spritenumber);

						continue;
					}
				}
				if (tileGrid[y][x].gethasflag()) {

					spritet.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
					spritefl.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
					gamewindow.draw(spritet);
					gamewindow.draw(spritefl);
				}
				else {
					spritet.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
					gamewindow.draw(spritet);
				}
				if (tileGrid[y][x].gethasmine() && (debugmode || !gamestate)) {
					spritem.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
					gamewindow.draw(spritem);

				}

			}
		}


		if (tilesrevealed == (columns * rows) - minecount) {
			gamewon(face, paused);
			gamestate = false;
			string info, inputstr, newleaderboards;
			int values;
			
			if (!added) {
				ifstream score("leaderboard.txt", std::ios::in);
				if (score.is_open()) {

					for (int i = 0; i < 5; i++) {
						score >> info;
						inputstr = info;
						inputstr.erase(std::remove_if(inputstr.begin(), inputstr.end(), [](char c) { return !std::isdigit(c); }), inputstr.end());

						values = stoi(inputstr);
						if (values > minute * 10 + second && !added) {
							newleaderboards += to_string(minuteten) + to_string(minuteone) + ":" + to_string(secondten) + to_string(secondone) + "," + username.substr(0, username.length()) + "*" + "\n";
							i++;
							added = true;
						}
						if (info.find("*") != std::string::npos) {
							newleaderboards += info.substr(0, info.length() - 1) + "\n";
						}
						else {
							newleaderboards += info + "\n";
						}
					}
					score.close();
					ofstream leader("leaderboard.txt", std::ios::in);
					leader << newleaderboards;
					leader.close();
				}
			}
			
			

			flagsplaced = minecount;
			for (int y = 0; y < rows; y++)
			{
				for (int x = 0; x < columns; x++)
				{
					if (tileGrid[y][x].gethasmine()) {
						spritet.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
						spritefl.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
						gamewindow.draw(spritet);
						gamewindow.draw(spritefl);

					}
				}
			}
		}
		while (gamewindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				gamewindow.close();
			if (event.type == Event::MouseButtonPressed) {
				Vector2i mousePos = Mouse::getPosition(gamewindow);
				mouseposx = static_cast<float>(mousePos.x);
				mouseposy = static_cast<float>(mousePos.y);
				if ((32 * ((rows)+0.5f)) < mouseposy && mouseposy < (32 * ((rows)+0.5f) + 64))
				{
					if (mouseposx > (((columns) * 32) - 240) && mouseposx < ((columns) * 32) - 176 && gamestate) {
						if (!paused) {
							paused = true;
							pause.loadFromFile("images\\/play.png");

						}
						else {
							paused = false;
							pause.loadFromFile("images\\/pause.png");
						}
					}


					else if (mouseposx > (((columns) * 32) - 176) && mouseposx < ((columns) * 32) - 112) {
						if (!leaderboardopen && paused) {
							paused = true;
							leaderboardwindow.create(sf::VideoMode(width / 2.0f, height / 2.0f), "Minesweeper", sf::Style::Default);
							string leaderboards = "";
							string info;

							inputprompt.setCharacterSize(18);
							inputprompt.setFillColor(Color::White);
							inputprompt.setStyle(Text::Bold);
							text.setString("LEADERBOARD");
							text.setCharacterSize(20);
							text.setFillColor(Color::White);
							text.setStyle(Text::Bold | Text::Underlined);
							setText(text, (width / 2.0f) / 2.0f, (height / 2.0f) / 2.0f - 120);

							IntRect revealedboard(0, 0, width, height - 100);
							Sprite spriter(tilerevealed, revealedboard);
							tilerevealed.setRepeated(true);
							gamewindow.draw(spriter);
							gamewindow.draw(spritetimerm);
							gamewindow.draw(spritetimers);
							gamewindow.draw(spritetimerm1);
							gamewindow.draw(spritetimers1);
							gamewindow.draw(counter1);
							gamewindow.draw(counter2);
							gamewindow.draw(counter3);
							gamewindow.draw(spritef);
							gamewindow.draw(sprited);
							gamewindow.draw(spritep);
							gamewindow.draw(spritel);
							gamewindow.display();

							ifstream leader("leaderboard.txt", ifstream::in);
							while (leaderboardwindow.isOpen())
							{
								Event event;

								if (leader.is_open()) {
									for (int i = 0; i < 5; i++) {
										leader >> info;
										leaderboards += to_string(i + 1) + ".\t" + info.substr(0, 5) + "\t" + info.substr(6) + "\n\n";
									}
									leader.close();
								}

								while (leaderboardwindow.pollEvent(event))
								{
									if (event.type == sf::Event::Closed)

										leaderboardwindow.close();

								}
								inputprompt.setString(leaderboards);
								setText(inputprompt, (width / 2.0f) / 2.0f, (height / 2.0f) / 2.0f + 20);
								leaderboardwindow.clear(Color(0, 0, 255));
								leaderboardwindow.draw(text);
								leaderboardwindow.draw(inputprompt);
								leaderboardwindow.display();
							}
						}
						else if (!leaderboardopen) {
							paused = true;
							leaderboardwindow.create(sf::VideoMode(width / 2.0f, height / 2.0f), "Minesweeper", sf::Style::Default);
							string leaderboards = "";
							string info;

							inputprompt.setCharacterSize(18);
							inputprompt.setFillColor(Color::White);
							inputprompt.setStyle(Text::Bold);
							text.setString("LEADERBOARD");
							text.setCharacterSize(20);
							text.setFillColor(Color::White);
							text.setStyle(Text::Bold | Text::Underlined);
							setText(text, (width / 2.0f) / 2.0f, (height / 2.0f) / 2.0f - 120);

							IntRect revealedboard(0, 0, width, height - 100);
							tilerevealed.setRepeated(true);
							Sprite spriter(tilerevealed, revealedboard);
							gamewindow.draw(spriter);
							gamewindow.draw(spritetimerm);
							gamewindow.draw(spritetimers);
							gamewindow.draw(spritetimerm1);
							gamewindow.draw(spritetimers1);
							gamewindow.draw(counter1);
							gamewindow.draw(counter2);
							gamewindow.draw(counter3);
							gamewindow.draw(spritef);
							gamewindow.draw(sprited);
							gamewindow.draw(spritep);
							gamewindow.draw(spritel);
							gamewindow.display();

							ifstream leader("leaderboard.txt", ifstream::in);
							while (leaderboardwindow.isOpen())
							{
								Event event;

								if (leader.is_open()) {
									for (int i = 0; i < 5; i++) {
										leader >> info;
										leaderboards += to_string(i + 1) + ".\t" + info.substr(0, 5) + "\t" + info.substr(6) + "\n\n";
									}
									leader.close();
								}

								while (leaderboardwindow.pollEvent(event))
								{
									if (event.type == sf::Event::Closed)

										leaderboardwindow.close();

								}
								inputprompt.setString(leaderboards);
								setText(inputprompt, (width / 2.0f) / 2.0f, (height / 2.0f) / 2.0f + 20);
								leaderboardwindow.clear(Color(0, 0, 255));
								leaderboardwindow.draw(text);
								leaderboardwindow.draw(inputprompt);
								leaderboardwindow.display();
							}
							paused = false;
						}
					}



					else if (mouseposx > ((columns) * 32) - 304 && mouseposx < ((columns) * 32) - 240 && gamestate) {
						if (!debugmode) {
							debugmode = true;
						}
						else {
							debugmode = false;
						}
					}

					else if (mouseposx > (((columns) / 2.0) * 32) - 32 && mouseposx < (((columns) / 2.0) * 32) + 32) {
						resetboard(tileGrid, columns, rows, minecount, second, minute, minuteone, minuteten, flagsplaced, paused, debugmode, tilesrevealed, added);
						face.loadFromFile("images\\/face_happy.png");
						gamestate = true;

					}
				}

				else if (mouseposy > 0 && mouseposy < height - 100) {
					if (!paused && !leaderboardopen) {
						if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
							if (tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].gethasflag() && tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].gethasmine()) {
								tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].sethasflag(false);
								flagsplaced--;
								minesonboard++;
							}
							else if (!tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].gethasflag() && tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].gethasmine()) {

								tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].sethasflag(true);
								flagsplaced++;
								minesonboard--;
							}
							else if (tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].gethasflag() && !tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].gethasmine()) {
								tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].sethasflag(false);
								flagsplaced--;
							}
							else {
								tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].sethasflag(true);
								flagsplaced++;
							}
						}
						else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
							if (!tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].gethasmine() && !tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].gethasflag() && !tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].getrevealed()) {
								revealtiles(tileGrid[(int)mouseposy / 32][(int)mouseposx / 32], true, tilesrevealed);

							}
							else if (tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].gethasmine() && !tileGrid[(int)mouseposy / 32][(int)mouseposx / 32].gethasflag()) {
								gameover(face, debugmode, paused);
								gamestate = false;

							}
						}
					}
				}
			}
		}


		if (!paused) {
			if (hasElapsedOneSecond(clock)) {
				if (second == 59) {
					minute++;
					minuteone++;
					second = -1;
					if (minuteone == 10) {
						minuteone = 0;
						minuteten++;
					}
				}
				second = second++;
				secondten = (second % 60) / 10;
				secondone = (second % 10);
				spritetimerm.setTextureRect(sf::IntRect(0 + 21 * minuteten, 0, 21, 32));
				spritetimers.setTextureRect(sf::IntRect(0 + 21 * secondten, 0, 21, 32));
				spritetimerm1.setTextureRect(sf::IntRect(0 + 21 * minuteone, 0, 21, 32));
				spritetimers1.setTextureRect(sf::IntRect(0 + 21 * secondone, 0, 21, 32));
				clock.restart();
			}
			gamewindow.draw(spritetimerm);
			gamewindow.draw(spritetimers);
			gamewindow.draw(spritetimerm1);
			gamewindow.draw(spritetimers1);
			gamewindow.draw(counter1);
			gamewindow.draw(counter2);
			gamewindow.draw(counter3);
			gamewindow.draw(spritef);
			gamewindow.draw(sprited);
			gamewindow.draw(spritep);
			gamewindow.draw(spritel);
			gamewindow.display();
		}

		else if (gamestate == false) {
			gamewindow.draw(spritetimerm);
			gamewindow.draw(spritetimers);
			gamewindow.draw(spritetimerm1);
			gamewindow.draw(spritetimers1);
			gamewindow.draw(counter1);
			gamewindow.draw(counter2);
			gamewindow.draw(counter3);
			gamewindow.draw(spritef);
			gamewindow.draw(sprited);
			gamewindow.draw(spritep);
			gamewindow.draw(spritel);
			gamewindow.display();
		}

		else {
			IntRect revealedboard(0, 0, width, height - 100);
			Sprite spriter(tilerevealed, revealedboard);
			tilerevealed.setRepeated(true);
			gamewindow.draw(spriter);
			gamewindow.draw(spritetimerm);
			gamewindow.draw(spritetimers);
			gamewindow.draw(spritetimerm1);
			gamewindow.draw(spritetimers1);
			gamewindow.draw(counter1);
			gamewindow.draw(counter2);
			gamewindow.draw(counter3);
			gamewindow.draw(spritef);
			gamewindow.draw(sprited);
			gamewindow.draw(spritep);
			gamewindow.draw(spritel);
			gamewindow.display();
		}
	}

	for (int i = 0; i < rows; ++i) {
		delete[] tileGrid[i];
	}
	delete[] tileGrid;

	return 0;
}