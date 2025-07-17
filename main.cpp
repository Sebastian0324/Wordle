#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string.h>
#include <random>
#include <iostream>//used for debugging
#include "TextBox.hpp"

std::string FILE_OF_WORDS = "words_5.txt";
int wordLength = 5;

static std::string getRandomLine() {

    int target;
    if (FILE_OF_WORDS == "words_7.txt")
    {
        target = rand() % 34342;
    }
    else if (FILE_OF_WORDS == "words_6.txt")
    {
        target = rand() % 23033;
    }
    else
    {
        target = rand() % 14855;
    }
    

    std::ifstream file(FILE_OF_WORDS);
    std::string line;
    for (int i = 0; i <= target; ++i) {
        std::getline(file, line);
    }

    return line;
}

void clear(std::string& w, char* g, std::vector<Yellow>& y, Row& ge, Keyboard& k, std::vector<std::string>& p, Row& po, std::string& h)
{
    w = "";
    strcpy(g, "       ");
    y.clear();

    ge.clear();
    k.reset();
    p.clear();
    po.clear();
    po.setPositionScale({-480, -250}, 35);

    h = getRandomLine();
}

void showPosWords(Row* w, std::vector<std::string>& words)
{
    w->clear();
    w->setPositionScale({480, 250}, 35);
    
    w->box[0]->setWidth(22.f * wordLength);
    w->push_back(words[0]);
    w->box[0]->setEvaluation(1);

    for (int i = 1; i < 6 && i < words.size(); i++)
    {
        w->newRow();
        w->box[0]->setWidth(22.f * wordLength);
        w->push_back(words[i]);
        w->box[0]->setEvaluation(1);
    }

}

void addYellow(char c, int index, std::vector<Yellow>& yellow)
{
    char temp = (127 - (1 << index));
    for (auto &&i : yellow)
    {
        if (i.character == c)
        {
            i.poss &= temp;
            return;
        }
    }
    
    yellow.push_back({c, temp});
}
    
static std::string checks(std::string& wrong, std::vector<Yellow>& yellow, char* green, std::string word)
{
    for (int i = 0; i < wordLength; i++)
    {
        if (green[i] != ' ' && word[i] != green[i])
            return "";
        if (wrong.find( word[i] ) != std::string::npos)
            return "";
        
    }

    for (auto &&x : yellow)
    {
        // replace find whit find all
        if (word.find( x.character ) == std::string::npos || (((1 << word.find( x.character )) & x.poss) == 0))
            return "";
    }

    return word;
}

void FilterWords(std::string& wrong, std::vector<Yellow>& yellow, char* green, std::vector<std::string>& result)
{
    std::vector<std::string> filtered;

    for (const auto& word : result) {
        if (checks(wrong, yellow, green, word) != "") {
            filtered.push_back(word);
        }
}

result = filtered;
}
void FilterWords(std::string& wrong, std::vector<Yellow>& yellow, char* green, std::vector<std::string>& result, std::string& filePath) 
{
    std::ifstream file(filePath);
    std::string word;
    std::string temp;
    if (!file) {
        std::cerr << "Could not open file!\n";
        return;
    }
    
    while (file >> word) 
    {
        temp = checks(wrong, yellow, green, word);
        if (temp == "")
            continue;
        result.push_back( temp );
    }
}

void evaluateGuess(std::string& wrong, std::vector<Yellow>& yellow, char* green, Row& guess, std::string& hidden)
{
    std::string g = guess.print();
    std::string temp = hidden;

    for_each(g.begin(), g.end(), [](char& c) {
        c = tolower(c);
    });

    for (int i = 0; i < wordLength; i++)
    {
        if (wrong.find( g[i] ) != std::string::npos)
        {
            guess.box[i]->setEvaluation(1);
            continue;
        }

        if (hidden.find( g[i] ) == std::string::npos)
        {
            guess.box[i]->setEvaluation(1);
            wrong += g[i];
            continue;
        } 
        else if (g[i] == hidden[i])
        {
            green[i] = hidden[i];
            temp[i] = '*';
            guess.box[i]->setEvaluation(3);
            continue;
        }
        addYellow(g[i], i, yellow);
    }
    for (int i = 0; i < wordLength; i++)
    {
        if (guess.box[i]->getEvaluation() != 0)
            continue;
        if (temp.find( g[i] ) != std::string::npos)
        {
            guess.box[i]->setEvaluation(2);
            temp[temp.find(g[i])] = '*';
        } else
            guess.box[i]->setEvaluation(1);
    }
    
}

bool isValidWord(std::string guess)
{
    std::ifstream file(FILE_OF_WORDS);
    std::string word;
    if (!file) {
        std::cerr << "Could not open file!\n";
        return false;
    }
    for_each(guess.begin(), guess.end(), [](char& c) {
        c = tolower(c);
    });
    while (file >> word)
    {
        if (word == guess)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    //window information
    sf::RenderWindow window(sf::VideoMode(950, 520), "SFML works!");
    window.setFramerateLimit(60);

    //Font used for all text
    sf::Font font;
    if (!font.loadFromFile("./arial.ttf"))
    {
        std::cout << " --- Faild to load font ---\n";
        return EXIT_FAILURE;
    }
    
    sf::Text text("[|87 Wordle", font, 50);
    text.setPosition(20, 20);

    // add a restart button
    Row restart(&font, 1);
    restart.setPositionScale({480, 170}, 30);
    restart.box[0]->setWidth(115.f);
    // restart.box[0]->setEvaluation(1);
    restart.push_back("ReStart");

    // add buttons to change the length off the words
    Row changeLength(&font, 2);
    changeLength.setPositionScale({300, 30}, 50);
    changeLength.push_back("-");
    changeLength.push_back("+");
    changeLength.box[0]->setEvaluation(1);
    changeLength.box[1]->setEvaluation(3);
    
    // Show a keyboard
    Keyboard keyboard(&font);

    // print a few possable words
    Row posWords(&font, 1);
    posWords.setPositionScale({-480, -250}, 35);
    posWords.box[0]->setWidth(22.f * wordLength);

    std::vector<std::string> posibleWords;
    Row guess(&font, wordLength);
    std::string wrong = "";
    std::vector<Yellow> yellow;
    char green[] = "       ";
    std::string hidden = getRandomLine();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (restart.box[0]->box.getGlobalBounds().contains({event.mouseButton.x/1.f, event.mouseButton.y/1.f}))
                {
                    clear(wrong, green, yellow, guess, keyboard, posibleWords, posWords, hidden);
                }
                else if (changeLength.box[0]->box.getGlobalBounds().contains({event.mouseButton.x/1.f, event.mouseButton.y/1.f}) && wordLength > 5)
                {
                    guess.setLength(--wordLength);
                    FILE_OF_WORDS[FILE_OF_WORDS.length() - 5] = char(wordLength + 48);
                    clear(wrong, green, yellow, guess, keyboard, posibleWords, posWords, hidden);

                    if (wordLength == 5)
                        changeLength.box[0]->setEvaluation(1);
                    else 
                        changeLength.box[0]->setEvaluation(3);
                    changeLength.box[1]->setEvaluation(3);
                }
                else if (changeLength.box[1]->box.getGlobalBounds().contains({event.mouseButton.x/1.f, event.mouseButton.y/1.f}) && wordLength < 7)
                {
                    guess.setLength(++wordLength);
                    FILE_OF_WORDS[FILE_OF_WORDS.length() - 5] = char(wordLength + 48);
                    clear(wrong, green, yellow, guess, keyboard, posibleWords, posWords, hidden);

                    if (wordLength == 7)
                        changeLength.box[1]->setEvaluation(1);
                    else
                        changeLength.box[1]->setEvaluation(3);
                    changeLength.box[0]->setEvaluation(3);
                }
                else if (keyboard.contains(event.mouseButton))
                {
                    if (keyboard.click(event.mouseButton, wrong, yellow) == '#')
                    {
                        posibleWords.clear();
                        FilterWords(wrong, yellow, green, posibleWords, FILE_OF_WORDS);
                        showPosWords(&posWords, posibleWords);
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && guess.size() == wordLength)
            {
                if (posibleWords.size() == 0)
                    FilterWords(wrong, yellow, green, posibleWords, FILE_OF_WORDS);
                else
                    FilterWords(wrong, yellow, green, posibleWords);
                
                if (isValidWord(guess.print()))
                {
                    evaluateGuess(wrong, yellow, green, guess, hidden);
                    keyboard.updateEvaluation(wrong, yellow, green);
                    
                    guess.newRow();

                    if (posibleWords.size() == 0)
                        FilterWords(wrong, yellow, green, posibleWords, FILE_OF_WORDS);
                    else
                        FilterWords(wrong, yellow, green, posibleWords);

                    showPosWords(&posWords, posibleWords);
                }
            }
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && guess.size() > 0)
            {
                // std::cout << " Remove letter.\n";
                guess.pop_back();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code < 26 && event.key.code >= 0 && guess.size() < wordLength)
            {
                // std::cout << " -- Pressed key: " << char( event.key.code + 65) << "\n";
                guess.push_back(char( event.key.code + 65));
            }
        }

        window.clear(sf::Color(0x121213));

        window.draw( text );
        guess.draw( window );
        keyboard.draw( window );
        changeLength.draw( window );
        restart.draw( window );
        posWords.draw( window );

        window.display();
    }

    return 0;
}
