//
//  Menu.hpp
//  SFML_Refined
//
//  Created by Shambhu Dahal on 11/16/20.
//  Copyright © 2020 Sampanna Dahal. All rights reserved.
//

#ifndef Menu_hpp
#define Menu_hpp

#define MENU_ITEMS 4
#include <SFML/Graphics.hpp>

class Menu
{
public:
    Menu(float width, float height);
    ~Menu();

    void draw(sf::RenderWindow &window);
    void MoveUp();
    void MoveDown();
    int GetPressedItem();

private:
    sf::Texture planeTexture;
    sf::Sprite planeSprite;

    float menuWidth;
    float menuHeight;
    int selectedItemIndex;
    sf::Font font;
    sf::Text menu[MENU_ITEMS];
};

Menu::Menu(float width, float height)
{
    selectedItemIndex = 0;
    if (!font.loadFromFile("fonts/CaviarDreams.ttf"))
    {
        // handle error
    }
    menuWidth = width;
    menuHeight = height;

    planeTexture.loadFromFile("plane.png");

    menu[0].setFont(font);
    menu[0].setFillColor(sf::Color::Red);
    menu[0].setString("TRACK MY FLIGHT");
    menu[0].setStyle(sf::Text::Bold);
    menu[0].setPosition(sf::Vector2f(width * .2, height / (MENU_ITEMS + 3) * 1));
    menu[0].setOrigin(menu[0].getLocalBounds().width / 2, menu[0].getLocalBounds().height / 2);

    menu[1].setFont(font);
    menu[1].setFillColor(sf::Color::White);
    menu[1].setString("SHOW ALL FLIGHTS");
    menu[1].setStyle(sf::Text::Bold);
    menu[1].setPosition(sf::Vector2f(width * 0.2, height / (MENU_ITEMS + 3) * 2));
    menu[1].setOrigin(menu[1].getLocalBounds().width / 2, menu[1].getLocalBounds().height / 2);

    menu[2].setFont(font);
    menu[2].setFillColor(sf::Color::White);
    menu[2].setString("BOOK A FLIGHT");
    menu[2].setStyle(sf::Text::Bold);
    menu[2].setPosition(sf::Vector2f(width * 0.2, height / (MENU_ITEMS + 3) * 3));
    menu[2].setOrigin(menu[2].getLocalBounds().width / 2, menu[2].getLocalBounds().height / 2);
    
    menu[3].setFont(font);
    menu[3].setFillColor(sf::Color::White);
    menu[3].setString("FLIGHT TRAFFIC BY COUNTRY");
    menu[3].setStyle(sf::Text::Bold);
    menu[3].setPosition(sf::Vector2f(width * 0.2, height / (MENU_ITEMS + 3) * 4));
    menu[3].setOrigin(menu[3].getLocalBounds().width / 2, menu[3].getLocalBounds().height / 2);


    planeSprite.setTexture(planeTexture);
    planeSprite.setOrigin(planeSprite.getLocalBounds().width / 2, planeSprite.getLocalBounds().height / 2);
    planeSprite.setScale(0.2, 0.2);
    planeSprite.setRotation(180);
    planeSprite.setPosition(sf::Vector2f(width * 0.2 - menu[selectedItemIndex].getLocalBounds().width / 2 - 30, height / (MENU_ITEMS + 3) * (selectedItemIndex + 1)));
}

Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow &window)
{
    for (int i = 0; i < MENU_ITEMS; i++)
    {
        window.draw(menu[i]);
    }
    window.draw(planeSprite);
}

void Menu::MoveUp()
{

    if (selectedItemIndex - 1 >= 0)
    {
        menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex = selectedItemIndex - 1;
        menu[selectedItemIndex].setFillColor(sf::Color::Red);
        planeSprite.setPosition(sf::Vector2f(menuWidth * 0.2 - menu[selectedItemIndex].getLocalBounds().width / 2 - 30, menuHeight / (MENU_ITEMS + 3) * (selectedItemIndex + 1)));
    }
}

void Menu::MoveDown()
{
    if (selectedItemIndex + 1 < MENU_ITEMS)
    {
        menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex = selectedItemIndex + 1;
        menu[selectedItemIndex].setFillColor(sf::Color::Red);
        planeSprite.setPosition(sf::Vector2f(menuWidth * 0.2 - menu[selectedItemIndex].getLocalBounds().width / 2 - 30, menuHeight / (MENU_ITEMS + 3) * (selectedItemIndex + 1)));
    }
}

int Menu::GetPressedItem()
{
    return selectedItemIndex;
}

#endif /* Menu_hpp */
