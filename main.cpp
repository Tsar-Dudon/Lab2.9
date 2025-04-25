#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <vector>

bool isLeftButtonPressed = false;
float stop = 1; //проверка на столкновение с объектом
float speed = 2; //для птицы
float dictance = 800;//для колонн
float colSpeed = 2;

struct Column{
    sf::RectangleShape downPart;
    sf::RectangleShape upperPart;
    float weight = 100;
    float m_X;
    float freeSpace = 200;
    float m_height;

    Column(float height, float X){
        downPart.setSize({weight, height});
        m_height = height;
        m_X = X;
        downPart.setFillColor(sf::Color::Green);
        downPart.setPosition({m_X, 800 - m_height});

        upperPart.setSize({weight, 800 - height - freeSpace});
        upperPart.setFillColor(sf::Color::Green);
        upperPart.setPosition({m_X, 0});
    }

    void move(){
        m_X -= colSpeed*stop;
        downPart.setPosition({m_X, 800 - m_height});
        upperPart.setPosition({m_X, 0});
    }
};

bool HitBox(Column column, sf::CircleShape circle){
    float circleX = circle.getPosition().x;
    float circleY = circle.getPosition().y;
    float circleRadius = circle.getRadius();

    float columnX = column.downPart.getPosition().x;

    float columnYDown = column.downPart.getPosition().y;
    float columnHeightDown = column.downPart.getSize().y;
    float columnYUpper = column.upperPart.getPosition().y;
    float columnHeightUpper = column.upperPart.getSize().y;

    if (circleX + circleRadius > columnX && circleX - circleRadius < columnX + column.weight &&
        circleY + circleRadius > columnYDown) {
        return true; //нижняя колонна
    }

    if (circleX + circleRadius > columnX && circleX - circleRadius < columnX + column.weight &&
        circleY - circleRadius < columnHeightUpper) {
        return true; //верхняя колонна
    }

    return false;
};

int main()
{    
    auto window = sf::RenderWindow(sf::VideoMode({1500, 800}), "Flappy Bird not original");
    window.setFramerateLimit(120);

    sf::CircleShape circle(24.f);
    circle.setOrigin(sf::Vector2f(24.f, 24.f));
    circle.setFillColor(sf::Color::Yellow);
    float y0 = 300;//птица

    //колоннЫ
    std::srand(std::time(0));
    std::vector<Column> columns;
    for(char i = 0; i < 5; i++){
        float height = rand() % (625 - 50 + 1) + 50;
        columns.emplace_back(height, 1500 + i*dictance);
    }



    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        y0 += speed*stop;
        if(HitBox(columns.front(), circle))
            stop = 0;
        circle.setPosition({150, y0});

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if(!isLeftButtonPressed) {
                isLeftButtonPressed = true;
                y0 -= 75;
            }
        }
        else {
            isLeftButtonPressed = false;
        }

        for (auto& column : columns) {
            column.move();
        }
        if(columns.front().downPart.getPosition().x < (-1*columns.front().weight)){
            columns.erase(columns.begin());
            float height = rand() % (625 - 50 + 1) + 50;
            columns.emplace_back(height, columns.back().downPart.getPosition().x + dictance);
        }

        window.clear(sf::Color::Blue);
        window.draw(circle);
        for (auto& column : columns) {
            window.draw(column.downPart);
            window.draw(column.upperPart);
        }
        window.display();
    }
}