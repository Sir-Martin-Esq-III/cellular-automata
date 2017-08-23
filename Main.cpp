#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>


void checkMouseIntersect(sf::Vector2f mousePos) {



}

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
	int type;
	int health = 10;
	sf::RectangleShape shape;//Predator or prey
	cell(int xoffset, int yoffset);
	void  update_type(sf::Color, int ntype);
	~cell();

private:

};

cell::cell(int xoffset,int yoffset)
{
	shape.setSize(sf::Vector2f(3, 3));
	shape.setOutlineThickness(1);
	shape.setPosition(4 * xoffset, 4 * yoffset);
	shape.setOutlineColor(sf::Color::Black);

}
void cell::update_type(sf::Color color, int ntype) {
	//shape.setFillColor(color);
	type = ntype;
}

cell::~cell()
{
}
std::vector <std::vector<cell> > createGrid(int width, int height) {
	std::vector<std::vector<cell>> grid;
	for (int xOff = 0; xOff < floor(width / 4); xOff++) {
		std::vector<cell> row;
		for (int yOff = 0; yOff < floor(height / 4); yOff++) {		
			int number = xorshf96();
			cell Cell(xOff,yOff);
			row.push_back(Cell);
		}
		grid.push_back(row);
	}
	return grid;
}
int main()
{
	int height =900;
	int width= 1200;
	std::vector <std::vector<cell> > Grid;
	sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
	Grid = createGrid(width, height);

	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed) {
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
				for (int xOff = 0; xOff < floor(width / 4); xOff++) {
					for (int yOff = 0; yOff < floor(height / 4); yOff++) {
						if (Grid[xOff][yOff].shape.getGlobalBounds().contains(mousePos)){
							if (event.mouseButton.button == sf::Mouse::Button::Left) {//Add prey
								Grid[xOff][yOff].shape.setFillColor(sf::Color::Green);
								break;
							}
							else { //add predator
								Grid[xOff][yOff].shape.setFillColor(sf::Color::Red);
								break;
							}
						}
					}
				}
				
			}
		}

		window.clear();
		
		for (int xOff = 0; xOff < floor(width / 4); xOff++) {
			for (int yOff = 0; yOff < floor(height / 4); yOff++) {
				//Move in a direction
				window.draw(Grid[xOff][yOff].shape);
			}
		}
		
		window.display();
	}

	return 0;
}
