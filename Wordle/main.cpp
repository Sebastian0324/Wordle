#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string.h>
#include <random>
#include <iostream>//used for debugging
#include "TextBox.hpp"

std::string FILE_OF_WORDS = "words.txt";

static std::string getRandomLine() {

    int target = rand() % 14855;
    std::ifstream file(FILE_OF_WORDS);
    std::string line;
    for (int i = 0; i <= target; ++i) {
        std::getline(file, line);
    }

    return line;
}

void addYellow(char c, int index, std::vector<Yellow>& yellow)
{
    char temp = (31 - (1 << index));
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
    for (int i = 0; i < 5; i++)
    {
        if (green[i] != ' ' && word[i] != green[i])
            return "";
        if (wrong.find( word[i] ) != std::string::npos)
            return "";
        
    }

    for (auto &&x : yellow)
    {
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

    for (int i = 0; i < 5; i++)
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
    for (int i = 0; i < 5; i++)
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


int main()
{
    //window information
    const int windowWidth = sf::VideoMode::getDesktopMode().width / 2;
    const int windowHeigth = sf::VideoMode::getDesktopMode().height / 2;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeigth), "SFML works!");
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
    restart.box[0]->setWidth(110.f);
    restart.push_back("ReStart");

    // Show a keyboard
    Keyboard keyboard(&font);

    // print a few possable words

    std::vector<std::string> posibleWords;
    Row guess(&font, 5);
    std::string wrong = "";
    std::vector<Yellow> yellow;
    char green[] = "     ";
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
                    wrong = "";
                    strcpy(green, "     ");
                    yellow.clear();

                    guess.clear();
                    keyboard.reset();
                    posibleWords.clear();

                    hidden = getRandomLine();
                }
            }

            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && guess.size() == 5)
            {
                std::cout << hidden << " | " << guess.print() << "\n";

                evaluateGuess(wrong, yellow, green, guess, hidden);
                keyboard.updateEvaluation(wrong, yellow, green);
                guess.newRow();

                if (posibleWords.size() == 0)
                    FilterWords(wrong, yellow, green, posibleWords, FILE_OF_WORDS);
                else
                    FilterWords(wrong, yellow, green, posibleWords);

                for (int i = 0; i < 5 && i < posibleWords.size(); i++)
                {
                    std::cout << posibleWords[i] << "\n";
                }
                std::cout << posibleWords.size() << "\n";
                std::cout << green << "\n";
                std::cout << wrong << "\n";
                
            }
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && guess.size() > 0)
            {
                // std::cout << " Remove letter.\n";
                guess.pop_back();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code < 26 && event.key.code >= 0 && guess.size() < 5)
            {
                // std::cout << " -- Pressed key: " << char( event.key.code + 65) << "\n";
                guess.push_back(char( event.key.code + 65));
            }
        }

        window.clear(sf::Color(0x121213));

        window.draw( text );
        guess.draw( window );
        keyboard.draw( window );
        restart.draw( window );

        window.display();
    }

    return 0;
}
