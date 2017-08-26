#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>


class cell
{
public:
	int type=0;
	int health = 10;
	sf::RectangleShape shape;//Predator or prey
	void changeType(int newType);
	sf::Color returnColor();
	cell(int xoffset, int yoffset);
	~cell();

private:

};

cell::cell(int xoffset,int yoffset)
{
	shape.setSize(sf::Vector2f(19, 19));
	shape.setOutlineThickness(1);
	shape.setPosition(20 * xoffset, 20 * yoffset);
	shape.setOutlineColor(sf::Color::Black);

}

sf::Color cell::returnColor() {
	return this->shape.getFillColor();
}

void cell::changeType(int newType) {
	switch (type)
	{
	case 1:
		shape.setFillColor(sf::Color::Green);
		break;
	case 2: 
		shape.setFillColor(sf::Color::Red);
		break;
	default:
		shape.setFillColor(sf::Color::White);
		break;
	}
	//type = newType;
/*Move in a random direction
Check if the movement in that direction is valid

PREY VALID MOVES
Need to increase health until it reaches a theshold at which a new prey is created
->Pred
-->Prey changes to Prey and Prey goes back to full health

->Empty
-->Empty cell changes to prey amd prev prey goes to empty

Pred VALID MOVES
Need to lower the health each generation
->Prey
-->Prey cell turns to Pred and current Pred full health

->Empty
-->Empty cell changes to Pred and prev pred goes to empty

*/
}
cell::~cell()
{
}
std::vector <std::vector<cell> > createGrid(int width, int height) {
	std::vector<std::vector<cell>> grid;
	for (int xOff = 0; xOff < floor(width / 20); xOff++) {
		std::vector<cell> row;
		for (int yOff = 0; yOff < floor(height / 20); yOff++) {		
			cell Cell(xOff,yOff);
			row.push_back(Cell);
		}
		grid.push_back(row);
	}
	return grid;
}


std::vector<std::vector<cell>> checkMouseIntersect(sf::Vector2f mousePos,int width,int height,sf::Event event,std::vector<std::vector<cell>> Grid) {
	for (int xOff = 0; xOff < floor(width / 20); xOff++) {
		for (int yOff = 0; yOff < floor(height / 20); yOff++) {
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

void swapCells(cell &cell1, cell &cell2) {
	cell dupe = cell1;
	cell1.type = cell2.type;
	cell1.health = cell2.health;
	cell1.shape.setFillColor(cell2.returnColor());

	cell2.type = dupe.type;
	cell2.health = dupe.health;
	cell2.shape.setFillColor(dupe.shape.getFillColor());
}

void createEmptyCell(cell &cell1) {
	cell1.health = 1;
	cell1.shape.setFillColor(sf::Color::White);
	cell1.type = 0;
}


int main()
{
	int height =900;
	int width= 1200;
	std::vector <std::vector<cell> > Grid;
	sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
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
			cell* lastEmptyCell;
			for (int xOff = 0; xOff < floor(width / 20); xOff++) {
				for (int yOff = 0; yOff < floor(height / 20); yOff++) {
					cell* currentCell = &Grid[xOff][yOff];
					switch (Grid[xOff][yOff].type){
					case 0://Empty cell
						lastEmptyCell = currentCell;
						break;
					case 1://Prey	
						if (yOff - 1 < 0) {
							createEmptyCell(*currentCell);
							break;
						}
						swapCells(*currentCell, Grid[xOff][yOff - 1]);
						break;
					case 2://Predator
						break;
					default:
						std::cout << "Error, check if there is a valid cell type" << '\n';
						break;
					}
				}
			}
		}
		//Rendering loop
		window.clear();
		for (int xOff = 0; xOff < floor(width / 20); xOff++) {
			for (int yOff = 0; yOff < floor(height / 20); yOff++) {
				Grid[xOff][yOff].changeType(2);
				window.draw(Grid[xOff][yOff].shape);
			}
			
		}	
		window.display();
	}

	return 0;
}
