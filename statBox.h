//
//  statBox.h
//  SFML_Refined
//
//  Created by Shambhu Dahal on 11/26/20.
//  Copyright © 2020 Sampanna Dahal. All rights reserved.
//

#ifndef statBox_h
#define statBox_h

#include <SFML/Graphics.hpp>

class StatBox
{
private:
    future<BinaryTree_<int, string>> &busiest;
    BinaryTree_<int, string> data;
    sf::RectangleShape statbox;
    sf::Font font;
    sf::Text header;
    sf::Clock cl;
    int frame_cntr = 0;
    sf::Text stats[5];
    sf::Text loading_txt;
    string txt = ""; /* loading txt */
    bool data_loaded = false;
    bool ready = false;
    auto load_ani_frm() -> void
    {
        if (cl.getElapsedTime().asSeconds() > 1)
        {
            cl.restart();
            if (frame_cntr < 3)
                frame_cntr++;
            else
                frame_cntr = 0;
            txt = "Loading";
            for (auto i = 0; i < frame_cntr; i++)
                txt += ".";
            loading_txt.setString(txt);
        }
    }

    auto fill_data() -> void
    {
        int i = 0;
        busiest.get().traverse([&](int first, string second) {
            if (i < 5)
            {
                stats[i].setString(second + ("    Arrivals : ") + tostr(first));
                i++;
            }
        });

        for (int i = 0; i < 5; i++)
        {
            stats[i].setFont(font);
            stats[i].setFillColor(sf::Color::Red);
            stats[i].setStyle(sf::Text::Bold);
            stats[i].setCharacterSize(20);
            stats[i].setPosition(sf::Vector2f(statbox.getPosition().x + statbox.getLocalBounds().width / 2, statbox.getPosition().y + (statbox.getLocalBounds().height / (6 + 1)) * (i + 2)));
            stats[i].setOrigin(stats[i].getLocalBounds().width / 2, stats[i].getLocalBounds().height / 2);
        }
        data_loaded = true;
        ready = true;
    }

public:
    StatBox(future<BinaryTree_<int, string>> &busiet) : busiest(busiet)
    {

        font.loadFromFile("fonts/CaviarDreams.ttf");

        statbox.setSize(sf::Vector2f(730, 300));
        statbox.setOutlineColor(sf::Color(204, 204, 0));
        statbox.setOutlineThickness(2);
        statbox.setFillColor(sf::Color(255, 255, 255, 0));
        statbox.setOrigin(0, 0);
        statbox.setPosition(sf::VideoMode::getDesktopMode().width * 0.45, 110);

        header.setFont(font);
        header.setString("TOP 5 BUSIEST AIRPORTS YESTERDAY");
        header.setCharacterSize(40);
        header.setFillColor(sf::Color::White);
        header.setStyle(sf::Text::Bold);
        header.setOrigin(header.getLocalBounds().width / 2, header.getLocalBounds().height / 2);
        header.setPosition(sf::Vector2f(statbox.getPosition().x + statbox.getLocalBounds().width / 2, statbox.getPosition().y + (statbox.getLocalBounds().height / (6 + 3)) * 1));

        loading_txt.setFont(font);
        loading_txt.setString("Loading");
        loading_txt.setCharacterSize(30);
        loading_txt.setFillColor(sf::Color::Black);
        loading_txt.setStyle(sf::Text::Bold);
        loading_txt.setOrigin(header.getLocalBounds().width / 2, header.getLocalBounds().height / 2);
        loading_txt.setPosition(sf::Vector2f(statbox.getPosition().x + statbox.getLocalBounds().width / 2, statbox.getPosition().y + (statbox.getLocalBounds().height / (6 + 3)) * 1 + 50));
    }
    void render(sf::RenderWindow &window)
    {

        window.draw(statbox);
        window.draw(header);
        if (ready)
            for (int i = 0; i < 5; i++)
                window.draw(stats[i]);
        else if (is_ready(busiest))
        {
            if (!data_loaded)
                fill_data();
            for (int i = 0; i < 5; i++)
                window.draw(stats[i]);
        }
        else
            load_ani_frm(), window.draw(loading_txt);
    }
};

#endif /* statBox_h */
