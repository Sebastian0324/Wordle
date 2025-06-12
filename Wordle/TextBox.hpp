#ifndef TEXTBOX
#define TEXTBOX

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>


class Box
{
private:
    const sf::Color color[4] = {sf::Color(0x6a6a6c), sf::Color(153, 153, 153), sf::Color(181, 159, 59), sf::Color(83, 141, 78)};

    int evaluation = 0;
public:
    sf::Text character;
    sf::RectangleShape box;

    Box(const sf::Font* font = nullptr, int fs = 50);
    ~Box();
    
    void setEvaluation(int eval);
    int getEvaluation() const;

    void draw(sf::RenderWindow &window) const;
};


class Row
{
private:
    int chars = 0;

public:
    Box* box[5];
    std::vector<Box*> oldRow;

    Row(sf::Font* font = nullptr);
    ~Row();

    void pop_back();
    void push_back(char in);
    std::string print();
    int size();
    void newRow();

    void draw(sf::RenderWindow& window) const;
};


class Keyboard
{
private:
    /* data */
public:
    Keyboard(/* args */);
    ~Keyboard();
};

#endif
