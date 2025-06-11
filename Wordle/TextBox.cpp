#include "TextBox.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>//used for debugging

//-----------------    MISC    -----------------

void CenterTextOnBox(sf::Text& t, sf::RectangleShape& b, char in)
{
    t.setString(in);
    t.setPosition({25 - (t.getLocalBounds().width / 2.f) + b.getPosition().x, b.getPosition().y - 5});
}

//---------------------------------------------------
//-----------------    Box    -----------------

Box::Box(sf::Font* font, int fs)
{
    box.setFillColor(sf::Color(0x6a6a6c));
    character.setFont(*font);
    character.setCharacterSize(fs);
}

Box::~Box()
{
}

void Box::setEvaluation(int eval)
{
    evaluation = eval;
    box.setFillColor(color[evaluation]);
}

int Box::getEvaluation() const
{
    return evaluation;
}

void Box::draw(sf::RenderWindow &window) const
{
    window.draw( box );
    window.draw( character );
}

//---------------------------------------------------

Row::Row(sf::Font* font)
{
    for (int i = 0; i < 5; i++)
    {
        box[i].box.setPosition(32 + (62 * i), 130);
        box[i].box.setSize({50, 50});
        box[i].character.setFont(*font);
    }
}

Row::~Row()
{
}

void Row::pop_back()
{
    box[--chars].character.setString("");
}
void Row::push_back(char in)
{
    CenterTextOnBox(box[chars].character, box[chars].box, in);
    ++chars;
}

std::string Row::print()
{
    std::string temp;
    for (auto &&i : box)
    {
        temp += i.character.getString();
    }
    
    return temp;
}
int Row::size()
{
    return chars;
}

void Row::draw(sf::RenderWindow &window) const
{
    for (int i = 0; i < 5; i++)
    {
        box[i].draw( window );
    }
}
