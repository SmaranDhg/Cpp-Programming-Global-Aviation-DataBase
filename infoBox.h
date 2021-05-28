//
//  infoBox.h
//  SFML_Refined
//
//  Created by Shambhu Dahal on 11/25/20.
//  Copyright © 2020 Sampanna Dahal. All rights reserved.
//

#ifndef infoBox_h
#define infoBox_h
#define RED sf::Color::Red
#define BLACK sf::Color::Black
#define WHITE sf::Color::White
#include<SFML/Graphics.hpp>
auto drawInfoBox(sf::RenderWindow &window, float pos_x, float pos_y, string callsign, float vel, float latd, float longtd, float altitude)
{
    sf::RectangleShape box;
    sf::Text data[5];
    sf::Font caviardreams;
    caviardreams.loadFromFile("fonts/CaviarDreams.ttf");
    

    box.setSize(sf::Vector2f(250,120));
    box.setFillColor(sf::Color(0,0,0, 110));
    box.setOrigin(0,box.getLocalBounds().height);
    box.setPosition(pos_x,pos_y);
    
    data[0].setFont(caviardreams);
    data[0].setStyle(sf::Text::Bold);
    data[0].setString(tostr("FLIGHT ")+callsign);
    data[0].setFillColor(sf::Color(255,165,0));
    data[0].setCharacterSize(14);
    data[0].setPosition(sf::Vector2f(box.getPosition().x+box.getLocalBounds().width/2, box.getPosition().y+box.getLocalBounds().height/(5+1)*1-box.getLocalBounds().height));
    data[0].setOrigin(data[0].getLocalBounds().width/2, data[0].getLocalBounds().height/2);
    
    data[1].setFont(caviardreams);
    data[1].setStyle(sf::Text::Bold);
    data[1].setString(tostr("Latitude: ")+ tostr(latd)+tostr(" degrees"));
    data[1].setFillColor(WHITE);
    data[1].setCharacterSize(14);
    data[1].setPosition(sf::Vector2f(box.getPosition().x+box.getLocalBounds().width/2, box.getPosition().y+box.getLocalBounds().height/(5+1)*2-box.getLocalBounds().height));
    data[1].setOrigin(data[1].getLocalBounds().width/2, data[1].getLocalBounds().height/2);
    
    data[2].setFont(caviardreams);
    data[2].setStyle(sf::Text::Bold);
    data[2].setString(tostr("Longitude: ")+ tostr(longtd)+tostr(" degrees"));
    data[2].setFillColor(WHITE);
    data[2].setCharacterSize(14);
    data[2].setPosition(sf::Vector2f(box.getPosition().x+box.getLocalBounds().width/2, box.getPosition().y+box.getLocalBounds().height/(5+1)*3-box.getLocalBounds().height));
    data[2].setOrigin(data[2].getLocalBounds().width/2, data[2].getLocalBounds().height/2);
    
    data[3].setFont(caviardreams);
    data[3].setStyle(sf::Text::Bold);
    data[3].setString(tostr("Velocity: ")+ tostr(vel)+tostr(" m/s"));
    data[3].setFillColor(WHITE);
    data[3].setCharacterSize(14);
    data[3].setPosition(sf::Vector2f(box.getPosition().x+box.getLocalBounds().width/2, box.getPosition().y+box.getLocalBounds().height/(5+1)*4-box.getLocalBounds().height));
    data[3].setOrigin(data[3].getLocalBounds().width/2, data[3].getLocalBounds().height/2);
    
    data[4].setFont(caviardreams);
    data[4].setStyle(sf::Text::Bold);
    data[4].setString(tostr("Altitude: ")+ tostr(altitude)+tostr(" meters"));
    data[4].setFillColor(WHITE);
    data[4].setCharacterSize(14);
    data[4].setPosition(sf::Vector2f(box.getPosition().x+box.getLocalBounds().width/2, box.getPosition().y+box.getLocalBounds().height/(5+1)*5-box.getLocalBounds().height));
    data[4].setOrigin(data[4].getLocalBounds().width/2, data[4].getLocalBounds().height/2);
    
    window.draw(box);
    for(int i =0; i<5; i++)
    {
        
        window.draw(data[i]);
    }
}


#endif /* infoBox_h */
