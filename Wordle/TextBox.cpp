#include "TextBox.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>//used for debugging

//-----------------    MISC    -----------------

void CenterTextOnBox(sf::Text& t, sf::RectangleShape& b, std::string in)
{
    t.setString(in);
    t.setPosition({(b.getSize().x/2) - (t.getLocalBounds().width / 2.f) + b.getPosition().x, b.getPosition().y - 5});
}

//---------------------------------------------------
//-----------------    Box    -----------------

Box::Box(const sf::Font* font)
{
    box.setFillColor(sf::Color(0x6a6a6c));

    if (font != nullptr)
        character.setFont(*font);
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

void Box::setWidth(float w)
{
    sf::Vector2f temp = {w, box.getSize().y};
    box.setSize(temp);
}

void Box::draw(sf::RenderWindow &window) const
{
    window.draw( box );
    window.draw( character );
}

//---------------------------------------------------
//-----------------    MISC    -----------------

void CreateRow(const sf::Font* f, Box* box[], int numberOfBoxes)
{
    for (int i = 0; i < numberOfBoxes; i++)
    {
        box[i] = new Box(f);
        // set position
    }
}

void MoveDown(Box* b)
{
    int offset = b->character.getCharacterSize() * 1.3;
    b->character.setPosition(b->character.getPosition().x,b->character.getPosition().y + offset);
    b->box.setPosition(b->box.getPosition().x,b->box.getPosition().y + offset);
}

//---------------------------------------------------
//-----------------    Row    -----------------

Row::Row(sf::Font* font, int _length)
{
    length = _length;
    box = new Box*[length]{nullptr};

    CreateRow(font, box, length);
    setPositionScale({32, 100}, 50);
}

Row::~Row()
{
    for (int i = 0; i < length; i++)
    {
        delete box[i];
    }
    delete[] box;

    for (auto &&i : oldRow)
    {
        delete i;
    }
}

void Row::pop_back()
{
    box[--chars]->character.setString("");
}
void Row::push_back(std::string in)
{
    CenterTextOnBox(box[chars]->character, box[chars]->box, in);
    ++chars;
}

void Row::push_back(char in)
{
    std::string temp = "";
    temp += in;
    push_back(temp);
}

std::string Row::print()
{
    std::string temp;
    for (int i = 0; i < length; i++)
    {
        temp += box[i]->character.getString();
    }
    
    return temp;
}
int Row::size()
{
    return chars;
}

void Row::newRow()
{
    sf::Vector2f p = box[0]->box.getPosition();
    int fs = box[0]->character.getCharacterSize();
    for (auto &&b : oldRow)
    {
        MoveDown(b);
    }
    
    for (int i = 0; i < length; i++)
    {
        MoveDown(box[i]);
        oldRow.push_back(box[i]);
    }
    CreateRow(oldRow[0]->character.getFont(), box, length);
    setPositionScale(p, fs);
    chars = 0;
}

void Row::setLength(int l)
{
    auto f = box[0]->character.getFont();
    
    for (int i = 0; i < length; i++)
    {
        delete box[i];
    }
    delete[] box;
    
    length = l;
    box = new Box*[l];
    CreateRow(f, box, length);
    setPositionScale({32, 100}, 50);

}

void Row::clear()
{
    
    for (auto &&i : oldRow)
    {
        delete i;
    }
    oldRow.clear();

    while (chars > 0)
    {
        pop_back();
    }
}

void Row::setPositionScale(sf::Vector2f p, int s)
{
    for (int i = 0; i < length; i++)
    {
        box[i]->box.setPosition(p.x + ((s * 1.2) * i), p.y);
        box[i]->box.setSize({s/1.f, s/1.f});
        box[i]->character.setCharacterSize(s);
    }
    
}

void Row::draw(sf::RenderWindow &window) const
{
    for (int i = 0; i < length; i++)
    {
        box[i]->draw( window );
    }

    for (auto &&i : oldRow)
    {
        i->draw(window);
    }
    
}

//---------------------------------------------------
//-----------------    Keyboard    -----------------

Keyboard::Keyboard(sf::Font* font)
{
    std::string temp = "";

    for (int i = 0; i < 3; i++)
    {
        rows[i] = new Row(font, layout[i].length());
        rows[i]->setPositionScale({450.f + (35 * i), 20.f + (48 * i)}, 35);

        for (int j = 0; j < layout[i].length(); j++)
        {
            rows[i]->push_back( layout[i][j] );
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for_each(layout[i].begin(), layout[i].end(), [](char& c) {
            c = tolower(c);
        });
    }

    border.setFillColor(sf::Color(0,0,0,0));
    border.setOutlineColor(sf::Color::Black);
    border.setOutlineThickness(3);
    border.setPosition(444, 14);
    border.setSize({ 426, 145});
    
    enter = new Box(font);
    enter->character.setCharacterSize(35);
    enter->box.setSize({35.f, 35.f});
    enter->box.setPosition({818, 115});
    CenterTextOnBox(enter->character, enter->box, "h");
    temp = "¤";
    enter->character.setString( sf::String::fromUtf8(temp.begin(), temp.end()) );
}

Keyboard::~Keyboard()
{
    delete rows[0];
    delete rows[1];
    delete rows[2];
    delete enter;
}

void Keyboard::reset()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < layout[i].length(); j++)
            if (rows[i]->box[j]->getEvaluation() != 0)
                rows[i]->box[j]->setEvaluation(0);
    
}

bool Keyboard::contains(sf::Event::MouseButtonEvent pos) const
{
    return border.getGlobalBounds().contains({pos.x/1.f, pos.y/1.f});
}

char Keyboard::click(sf::Event::MouseButtonEvent pos, std::string& w, std::vector<Yellow>& y)
{
    char temp = ' ';
    sf::Vector2f xy = {pos.x/1.f, pos.y/1.f};
    if (enter->box.getGlobalBounds().contains(xy))
        return '#';
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < layout[i].length(); j++)
        {
            if (rows[i]->box[j]->box.getGlobalBounds().contains(xy))
            {
                temp = char(std::tolower(rows[i]->box[j]->character.getString()[0]));

                if (rows[i]->box[j]->getEvaluation() == 0)
                {
                    w += temp;
                } 
                else if (rows[i]->box[j]->getEvaluation() == 1)
                {
                    w = w.substr(0, w.find(temp)) + w.substr(w.find(temp) + 1);
                    y.push_back({temp, 127});
                } 
                else if (rows[i]->box[j]->getEvaluation() == 2)
                {
                    for (int k = 0; k < y.size(); k++)
                    {
                        if (y[k].character == temp)
                        {
                            y.erase(y.begin()+k);
                        }
                        
                    }
                    
                }
                
                
                
                rows[i]->box[j]->setEvaluation( (rows[i]->box[j]->getEvaluation() + 1) % 3 );
                return temp; // can only click on one thing at a time
            }
            
        }
    }
    return ' ';
}

void Keyboard::updateEvaluation(std::string &w, std::vector<Yellow> &y, std::string g)
{
    for (int i = 0; i < 3; i++)
    {
        for (auto &&x : y)
        {
            if (layout[i].find( x.character ) != std::string::npos)
            {
                rows[i]->box[layout[i].find( x.character )]->setEvaluation(2);
            }
        }

        for (int j = 0; j < layout[i].length(); j++)
        {
            if (w.find( layout[i][j] ) != std::string::npos)
            {
                rows[i]->box[j]->setEvaluation(1);
            }
            else if (g.find( layout[i][j] ) != std::string::npos)
            {
                rows[i]->box[j]->setEvaluation(3);
            }
        }
    }
}

void Keyboard::draw(sf::RenderWindow &window) const
{
    window.draw( border );
    enter->draw( window );
    for (int i = 0; i < 3; i++)
    {
        rows[i]->draw( window );
    }
}
