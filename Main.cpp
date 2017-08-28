#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

//TO DO:
// Check quadrents for mouse input
// Fix non random cell placement

int cellSize = 4;
//Marsaglia's xorshf generator
static unsigned long x = 1231056789, y = 3621036069, z = 521288629;

unsigned long xorshf96(void) {          //period 2^96-1
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}


class cell
{
public:
	int type=0;
	sf::RectangleShape shape;//Predator or prey
	void changeType(int newType);
	cell(int xoffset, int yoffset);
	
	~cell();

private:

};

cell::cell(int xoffset,int yoffset)
{
	shape.setSize(sf::Vector2f(cellSize,cellSize));
	shape.setOutlineThickness(1);
	shape.setPosition(cellSize * xoffset, cellSize * yoffset);
}

void cell::changeType(int newType) {
	switch (newType)
	{
	case 0:
		this->shape.setFillColor(sf::Color::White);
		break;
	case 1:
		this->shape.setFillColor(sf::Color::Green);
		break;
	case 2: 
		this->shape.setFillColor(sf::Color::Red);
		break;
	default:
		break;
	}
	type = newType;
}
cell::~cell()
{
}
//Creates the grid
std::vector <std::vector<cell> > createGrid(int width, int height) {
	std::vector<std::vector<cell>> grid;
	for (int xOff = 0; xOff < floor(width / cellSize); xOff++) {
		std::vector<cell> row;
		for (int yOff = 0; yOff < floor(height / cellSize); yOff++) {		
			cell Cell(xOff,yOff);
			int randomNumber = xorshf96();
			if (randomNumber % 3 == 0) {
				Cell.changeType(1);
			}
			else {
				Cell.changeType(2);
			}
			row.push_back(Cell);
		}
		grid.push_back(row);
	}
	return grid;
}

//Checks what cell the mouse intersects for input
std::vector<std::vector<cell>> checkMouseIntersect(sf::Vector2f mousePos,int width,int height,sf::Event event,std::vector<std::vector<cell>> Grid) {
	for (int xOff = 0; xOff < floor(width / cellSize); xOff++) {
		for (int yOff = 0; yOff < floor(height / cellSize); yOff++) {
			if (Grid[xOff][yOff].shape.getGlobalBounds().contains(mousePos)) {
				if (event.mouseButton.button == sf::Mouse::Button::Left) {//Add prey
					Grid[xOff][yOff].shape.setFillColor(sf::Color::Green);
					Grid[xOff][yOff].type = 1;
				}
				else {
					Grid[xOff][yOff].shape.setFillColor(sf::Color::Red);
					Grid[xOff][yOff].type = 2;	
				}
				return Grid;
			}
		}
	}
}

//Swaps cells if they are different types
void swapCells(cell &cell1, cell &cell2) {
	cell *dupe = nullptr;
	if (cell1.type != cell2.type) {//We know that the prey will be eaten
		dupe = &cell1;
	}
	else {
		dupe = &cell2;
	}
	cell1.changeType(cell2.type);

	cell2.changeType(dupe->type);
}


int main()
{
	std::cout << "Please enter the cell size, It will create a grid of (1920*1080)/(cellsize^2) so don't put anything too low: ";
	std::cin >> cellSize;
	srand(time(NULL));
	int height =1080;
	int width= 1920;
	std::vector <std::vector<cell> > Grid;
	sf::RenderWindow window(sf::VideoMode(width, height), "cellular-automata");
	Grid = createGrid(width, height);
	bool simMode = false;
	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Return) {
					if (simMode) {
						simMode = false;
					}
					else {
						simMode = true;
					}
				}
			}
			if (event.type == sf::Event::MouseButtonPressed &&!simMode) {
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
				Grid=checkMouseIntersect(mousePos,width,height,event,Grid);
			}
		}


		//Simulation loop
		if (simMode) {
			cell* lastEmptyCell=nullptr;
			for (int xOff = 0; xOff < floor(width / cellSize); xOff++) {
				for (int yOff = 0; yOff < floor(height / cellSize); yOff++) {
					cell* currentCell = &Grid[xOff][yOff];
					int randomNumber = rand() % 4 + 1;
					int newXcoord = xOff;
					int newYcoord = yOff;
					//Get Position of movement;
					switch (randomNumber){
					case 1://Up
						newYcoord--;
						break;
					case 2://Down
						newYcoord++;
						break;
					case 3://Left
						newXcoord--;
						break;
					case 4://Right
						newXcoord++;
						break;
					default:
						break;
					}
					if (newYcoord <= 0 || newYcoord >= height / cellSize || newXcoord< 0 || newXcoord >= width / cellSize) {
						currentCell->changeType(0);
						break;
					}
					else {
						swapCells(*currentCell, Grid[newXcoord][newYcoord]);
					}
				}
			}
		}

		//Rendering loop
		window.clear();
		for (int xOff = 0; xOff < floor(width / cellSize); xOff++) {
			for (int yOff = 0; yOff < floor(height / cellSize); yOff++) {
				if (!simMode) {
					Grid[xOff][yOff].shape.setOutlineThickness(1);
				}
				else {
					Grid[xOff][yOff].shape.setOutlineThickness(0);
				}

				window.draw(Grid[xOff][yOff].shape);
			}
		}	
		window.display();
	}
	return 0;
}
