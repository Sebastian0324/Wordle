#ifndef TEXTBOX
#define TEXTBOX

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

struct Yellow
{
    char character;
    char poss = 0b11111;
};

class Box
{
private:
    const sf::Color color[4] = {sf::Color(0x6a6a6c), sf::Color(153, 153, 153), sf::Color(181, 159, 59), sf::Color(83, 141, 78)};

    int evaluation = 0;
public:
    sf::Text character;
    sf::RectangleShape box;

    Box(const sf::Font* font = nullptr);
    ~Box();
    
    void setEvaluation(int eval);
    int getEvaluation() const;
    void setWidth(float w);

    void draw(sf::RenderWindow &window) const;
};


class Row
{
private:
    int chars = 0;
    int length;

public:
    Box** box;
    std::vector<Box*> oldRow;

    Row(sf::Font* font = nullptr, int _length = 5);
    ~Row();

    void pop_back();
    void push_back(std::string in);
    void push_back(char in);
    std::string print();

    int size();
    void newRow();
    void setLength(int l);

    void clear();
    void setPositionScale(sf::Vector2f p, int s);

    void draw(sf::RenderWindow& window) const;
};


class Keyboard
{
private:
    std::string layout[3] = {"QWERTYUIOP","ASDFGHJKL","ZXCVBNM"};
    Row* rows[3];
public:
    Keyboard(sf::Font* font = nullptr);
    ~Keyboard();

    void reset();
    void updateEvaluation(std::string& w, std::vector<Yellow>& y, std::string g);
    void draw(sf::RenderWindow& window) const;
};

#endif
