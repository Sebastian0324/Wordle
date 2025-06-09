#ifndef TEXTBOX
#define TEXTBOX

#include <vector>
#include <string>

class Row
{
private:
    std::vector<char> guess;
public:
    Row(/* args */);
    ~Row();

    void pop_back();
    void push_back(char in);
    std::string print();
    int size();
};

#endif
