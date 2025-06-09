#include "TextBox.hpp"
#include <string>

Row::Row()
{
}

Row::~Row()
{
}

void Row::pop_back()
{
    guess.pop_back();
}
void Row::push_back(char in)
{
    guess.push_back(in);
}

std::string Row::print()
{
    std::string temp;
    for (auto &&i : guess)
    {
        temp += i;
    }
    
    return temp + "\n";
}
int Row::size()
{
    return guess.size();
}
