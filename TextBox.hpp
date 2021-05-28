//
//  TextBox.h
//  SFML_Refined
//
//  Created by Shambhu Dahal on 11/17/20.
//  Copyright © 2020 Sampanna Dahal. All rights reserved.
//

#ifndef TextBox_hpp
#define TextBox_hpp

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#define DELETE_KEY 8
#define ENTER_KEY 10
#define ESCAPE_KEY 27

class TextBox
{
public:
    sf::Font font;
    TextBox(sf::Color color, bool sel)
    {
        if (!font.loadFromFile("fonts/CaviarDreams.ttf"))
        {
            std::cout << "Error loading font for text box" << std::endl;
        }
        textbox.setFont(font);
        textbox.setFillColor(color);
        isSelected = sel;

        if (sel)
        {
            textbox.setString("_");
        }
        else
        {
            textbox.setString("");
        }
    }

    void setSize(int size)
    {
        textbox.setCharacterSize(size);
    }
    void setPosition(sf::Vector2f pos)
    {
        textbox.setPosition(pos);
    }

    void setlimit(int lim)
    {
        hasLimit = true;
        limit = lim;
    }

    void setSelected(bool sel)
    {
        isSelected = sel;
        if (!sel)
        {
            textbox.setString(text.str());
        }
    }

    std::string getText()
    {
        return ret;
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(textbox);
    }

    void typedOn(sf::Event input)
    {

        if (isSelected)
        {
            int charTyped = input.text.unicode;

            if (charTyped < 128)
            {
                if (hasLimit)
                {
                    if (text.str().length() < limit)
                    {
                        inputLogic(charTyped);
                    }
                    else if (text.str().length() == limit && charTyped == DELETE_KEY)
                    {
                        deleteLastChar();
                    }
                }
                else
                    inputLogic(charTyped);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) and sf::Keyboard::isKeyPressed(sf::Keyboard::V))
            textbox.setString(_cboard.getString());
        ret = text.str();
        if (strip(ret).empty())
            ret = textbox.getString();
    }

private:
    sf::Text textbox;
    std::ostringstream text;
    bool isSelected = false;
    bool hasLimit = false;
    int limit;
    string ret = "";
    sf::Clipboard _cboard;
    void inputLogic(int charTyped)
    {
        if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
        {
            text << static_cast<char>(charTyped);
        }
        else if (charTyped == DELETE_KEY)
        {
            if (text.str().length() > 0)
            {
                deleteLastChar();
            }
        }
        textbox.setString(text.str() + "_");
    }
    void deleteLastChar()
    {
        std::string t = text.str();
        std::string newT = "";
        for (int i = 0; i < t.length() - 1; i++)
        {
            newT += t[i];
        }
        text.str("");
        text << newT;

        textbox.setString(text.str());
    }
};
#endif /* TextBox_h */
