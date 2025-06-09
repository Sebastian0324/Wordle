#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>//used for debugging
#include "TextBox.hpp"

// curl -s ... | grep -v [dukfiht] | grep .r... | grep n | grep -v ...n. | grep g | grep -v ..g..
static std::vector<std::string> runGrepFilter(std::string& wrong, std::vector<char[5]>& yellow, std::vector<char[5]>& green) {
    std::vector<std::string> result;
    std::string cmd = "cat words.txt | ";

    cmd += "grep -v [" + wrong + "] | ";

    for (std::string y : yellow)
    {
        cmd += "grep -v " + y + " | ";
    }
    for (std::string g : green)
    {
        cmd += "grep " + g + " | ";
    }

    FILE* pipe = popen(cmd.substr(0, cmd.length() - 2).c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        std::string line(buffer);
        // Remove trailing newline
        if (!line.empty() && line.back() == '\n')
            line.pop_back();
        result.push_back(line);
    }

    pclose(pipe);
    return result;
}

int main()
{
    //window information
    const int windowWidth = sf::VideoMode::getDesktopMode().width / 2;
    const int windowHeigth = sf::VideoMode::getDesktopMode().height / 2;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeigth), "SFML works!");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("./arial.ttf"))
    {
        std::cout << " --- Faild to load font ---\n";
        return EXIT_FAILURE;
    }
    
    sf::Text text("Hello SFML", font, 50);
    text.setPosition(20, 20);
    
    sf::CircleShape test = sf::CircleShape( 50.f );
    test.setFillColor( sf::Color::Red );

    // std::vector<char>* guess;
    Row guess;
    std::string wrong = "";
    std::vector<char[5]> yellow;
    std::vector<char[5]> green;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (event.type ==  sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                std::cout << guess.print();
            }
            if (event.type ==  sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && guess.size() > 0)
            {
                std::cout << " Remove letter.\n";
                guess.pop_back();
            }
            if (event.type ==  sf::Event::KeyPressed && event.key.code < 26 && event.key.code >= 0 && guess.size() < 5)
            {
                std::cout << " -- Pressed key: " << char( event.key.code + 65) << "\n";
                guess.push_back(char( event.key.code + 65));
            }
        }

        window.clear(sf::Color(0x121213));
        window.draw( test );
        window.draw( text );
        window.display();
    }

    return 0;
}
