@REM g++ -c main.cpp -I"C:\cpp\SFML-2.5.1\include"
@REM g++ -c TextBox.cpp -I"C:\cpp\SFML-2.5.1\include"
@REM g++ main.o TextBox.o -o Main-app -L "C:\cpp\SFML-2.5.1\lib" -l sfml-graphics -l sfml-window -l sfml-system
g++ main.cpp TextBox.cpp -o Main-app -I"C:\cpp\SFML-2.5.1\include" -L"C:\cpp\SFML-2.5.1\lib" -lsfml-graphics -lsfml-window -lsfml-system
.\Main-app