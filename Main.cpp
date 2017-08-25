#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

//Marsaglia's xorshf generator
static unsigned long x = 123456789, y = 362436069, z = 521288629;

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
	int health = 10;
	sf::RectangleShape shape;//Predator or prey
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
					return Grid;
				}
				else { //add predator
					Grid[xOff][yOff].shape.setFillColor(sf::Color::Red);
					Grid[xOff][yOff].type = 2;
					return Grid;
				}
			}
		}
	}
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
					simMode = true;  
				}
			}
			if (event.type == sf::Event::MouseButtonPressed &&!simMode) {
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
				Grid=checkMouseIntersect(mousePos,width,height,event,Grid);
			}
		}


		//Simulation loop
		if (simMode) {
			for (int xOff = 0; xOff < floor(width / 20); xOff++) {
				for (int yOff = 0; yOff < floor(height / 20); yOff++) {
					if (!Grid[xOff][yOff].type == 0) {//If not empty
						cell Copy(-20, -20);
					}
				}
			}
		}
		//Rendering loop
		window.clear();
		for (int xOff = 0; xOff < floor(width / 20); xOff++) {
			for (int yOff = 0; yOff < floor(height / 20); yOff++) {
				
				window.draw(Grid[xOff][yOff].shape);
			}
			
		}
		
		window.display();
	}

	return 0;
}
