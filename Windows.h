//
//  Windows.h
//  SFML_Refined
//
//  Created by Shambhu Dahal on 11/17/20.
//  Copyright © 2020 Sampanna Dahal. All rights reserved.

// 10 degrees EAST/WEST = 37 units
//10 degrees NORTH/SOUTH = 38 units
#ifndef Windows_h
#define Windows_h

#define WIDTH 1354
#define HEIGHT 632
#define ORIGIN_X 677
#define ORIGIN_Y 379

#include "_api_parsers.h"
#include <string>
#include <SFML/Graphics.hpp>
#include "TextBox.hpp"
#include "Menu.hpp"
#include "TextBox.hpp"
#include "infoBox.h"
#include "statBox.h"

static const sf::Vector2u windowSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
static const sf::Vector2f displayCenter(windowSize.x / 2, windowSize.y / 2);
static sf::Vector2i mousePosScreen = {0, 0};
static sf::Vector2i mousePosWindow = {0, 0};
static sf::Vector2f mousePosView = {0, 0};

auto updateMousePosition(sf::RenderWindow &window) -> void;
void enterFlight();
void drawMap(string callsign, float latitude, float longitude, float tilt, float velocity, float altitude);
void drawAllMap();

void drawStartWindow();
void drawDialog(std::string msg);
void bookFlight();
void showFlights(BinaryTree_<string, string> intl_flights, BinaryTree_<string, string> dom_flights);
void displayTraffic();

void drawStartWindow()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "GLOBAL AVIATION DATABASE");

    auto fetch_data = []() {
        thread t1{[]() { fetch_all_states(); }};
        auto ret = fetch_busiest_airports();
        t1.join();
        return ret;
    };
    auto busiest = async(launch::async, fetch_data);
    StatBox statbox(busiest);

    sf::Font noteworthy;
    noteworthy.loadFromFile("fonts/Noteworthy.ttf");
    sf::Font thuner;
    thuner.loadFromFile("fonts/thuner.otf");

    sf::Font caviardreams;
    caviardreams.loadFromFile("fonts/CaviarDreams.ttf");

    sf::Image bgImg;
    if (!bgImg.loadFromFile("bg.jpg"))
        std::cout << "Couldn't load menu background!" << std::endl;

    sf::Texture bgTexture;
    bgTexture.loadFromImage(bgImg); //Load Texture from file

    sf::Sprite bgSprite(bgTexture);
    bgSprite.setOrigin(1920 / 2, 1200 / 2);
    bgSprite.setPosition(sf::Vector2f(1920 / 2, 1200 / 2));

    sf::Text headText("GLOBAL AVIATION DATABASE", caviardreams);
    headText.setCharacterSize(60);
    headText.setFillColor(sf::Color::Yellow);
    headText.setStyle(sf::Text::Bold);
    headText.setPosition(window.getSize().x / 2, 50);
    headText.setOrigin(headText.getLocalBounds().width / 2, headText.getLocalBounds().height / 2);

    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen())
    {
        sf::Event event;

        //handle all events
        while (window.pollEvent(event))
        {

            switch (event.type)
            {

            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Up:
                    menu.MoveUp();
                    break;

                case sf::Keyboard::Key::Down:
                    menu.MoveDown();
                    break;

                case sf::Keyboard::Key::Return:
                    switch (menu.GetPressedItem())
                    {
                    case 0:
                    {
                        enterFlight();
                        break;
                    }
                    case 1:
                    {
                        drawAllMap();
                        break;
                    }
                    case 2:
                    {
                        bookFlight();
                        break;
                    }
                    case 3:
                    {
                        displayTraffic();
                        // window.close();
                        break;
                    }
                    }

                    break;
                }
            }
        }

        //update the game

        window.clear();
        window.draw(bgSprite);
        window.draw(headText);
        menu.draw(window);
        statbox.render(window);
        window.display();
    }
}

void enterFlight()
{
    bool flightFound = false;
    sf::RenderWindow flightWindow(sf::VideoMode(500, 500), "ENTER YOUR FLIGHT");
    sf::Text text1;

    sf::Font caviardreams;
    caviardreams.loadFromFile("fonts/CaviarDreams.ttf");

    text1.setFont(caviardreams);
    text1.setString("Enter your flight's callsign");
    text1.setFillColor(sf::Color::White);
    text1.setOrigin(text1.getLocalBounds().width / 2, text1.getLocalBounds().height / 2);
    text1.setPosition(flightWindow.getSize().x / 2, 40);

    TextBox textbox1(sf::Color::Red, true);
    textbox1.setPosition({100, 100});
    textbox1.setSize(28);
    textbox1.setlimit(8);

    while (flightWindow.isOpen())
    {
        sf::Event flightEvent;
        while (flightWindow.pollEvent(flightEvent))
        {
            switch (flightEvent.type)
            {
            case sf::Event::Closed:
                flightWindow.close();
                break;
            case sf::Event::TextEntered:
                textbox1.typedOn(flightEvent);
                break;
            case sf::Event::KeyPressed:
                switch (flightEvent.key.code)
                {
                case sf::Keyboard::Key::Return:
                {
                    textbox1.setSelected(false);
                    std::string choice = textbox1.getText();
                    flightWindow.close();
                    auto val = parse_all_state("callsign");
                    for (auto f : val.Keys())
                    {
                        if (f == upper(choice))
                        {
                            flightFound = true;
                            //(*val[f]).show();
                            drawMap((*val[f])[1],stof((*val[f])[6]), stof((*val[f])[5]), stof((*val[f])[10]), stof((*val[f])[9]), stof((*val[f])[13]));
                        }
                    }
                    if (flightFound == false)
                    {
                        drawDialog("ERROR: FLIGHT WITH INPUT CALLSIGN NOT FOUND, PRESS ESC TO TRY AGAIN");
                        enterFlight();
                    }
                    break;
                }
                case sf::Keyboard::Key::Escape:
                    flightWindow.close();
                    break;
                }
                break;
            }
        }
        flightWindow.clear();
        flightWindow.draw(text1);
        textbox1.draw(flightWindow);
        flightWindow.display();
    }
}
void drawMap(string callsign, float latitude, float longitude, float tilt, float velocity, float altitude)
{

    sf::RenderWindow mapWindow(sf::VideoMode(WIDTH, HEIGHT), "Map Window");

    sf::Image mapImg;
    if (!mapImg.loadFromFile("map.jpg"))
        std::cout << "Coudln't load the map image!" << std::endl;

    sf::Texture mapTexture;
    mapTexture.loadFromImage(mapImg); //Load Texture from file

    sf::Sprite mapSprite(mapTexture);
    mapSprite.setPosition(sf::Vector2f(WIDTH / 2, HEIGHT / 2));
    mapSprite.setOrigin(WIDTH / 2, HEIGHT / 2);

    sf::Texture planeTexture;
    planeTexture.loadFromFile("plane.png");

    sf::Sprite planeSprite(planeTexture);
    planeSprite.setScale(0.2, 0.2);
    planeSprite.setPosition(ORIGIN_X + (longitude * 3.7), ORIGIN_Y - (latitude * 3.8));
    planeSprite.setRotation(90);
    planeSprite.rotate(tilt);
    planeSprite.setOrigin(planeSprite.getLocalBounds().width / 2, planeSprite.getLocalBounds().height / 2);

    while (mapWindow.isOpen())
    {
        sf::Event mapEvent;
        while (mapWindow.pollEvent(mapEvent))
        {

            switch (mapEvent.type)
            {
            case sf::Event::KeyPressed:
                switch (mapEvent.key.code)
                {
                case sf::Keyboard::Key::Escape:
                    mapWindow.close();
                    break;
                }
                break;
            case sf::Event::Closed:
                mapWindow.close();
                break;
            }
        }

        mapWindow.clear();
        mapWindow.draw(mapSprite);
        mapWindow.draw(planeSprite);
        drawInfoBox(mapWindow, ORIGIN_X + (longitude * 3.7), ORIGIN_Y - (latitude * 3.8),callsign, velocity, latitude, longitude, altitude);
        mapWindow.display();
    }
}

void drawDialog(std::string msg)
{
    sf::RenderWindow dialogWindow(sf::VideoMode(1100, 200), "ERROR");
    sf::Text emsg;

    sf::Font caviardreams;
    caviardreams.loadFromFile("fonts/CaviarDreams.ttf");

    emsg.setFont(caviardreams);
    emsg.setString(msg);
    emsg.setFillColor(sf::Color::Red);
    emsg.setOrigin(emsg.getLocalBounds().width / 2, emsg.getLocalBounds().height / 2);
    emsg.setPosition(1100 / 2, 200 / 2);

    while (dialogWindow.isOpen())
    {
        sf::Event dialogEvent;
        while (dialogWindow.pollEvent(dialogEvent))
        {

            switch (dialogEvent.type)
            {
            case sf::Event::Closed:
                dialogWindow.close();
                break;
            case sf::Event::KeyPressed:
                switch (dialogEvent.key.code)
                {
                case sf::Keyboard::Key::Escape:
                case sf::Keyboard::Key::Enter:
                    dialogWindow.close();
                    break;
                }
            }
        }

        dialogWindow.clear();
        dialogWindow.draw(emsg);
        dialogWindow.display();
    }
}

void bookFlight()
{
    bool flightFound = false;
    sf::RenderWindow flightWindow(sf::VideoMode(500, 500), "ENTER DEPARTURE COUNTRY");
    sf::Text text1;

    sf::Font caviardreams;
    caviardreams.loadFromFile("fonts/CaviarDreams.ttf");

    text1.setFont(caviardreams);
    text1.setString("Enter your departure country");
    text1.setFillColor(sf::Color::White);
    text1.setOrigin(text1.getLocalBounds().width / 2, text1.getLocalBounds().height / 2);
    text1.setPosition(flightWindow.getSize().x / 2, 40);

    TextBox textbox1(sf::Color::Red, true);
    textbox1.setPosition({100, 100});
    textbox1.setSize(28);

    BinaryTree_<string, string> intl_flights;
    BinaryTree_<string, string> dom_flights;
    string choice = "";

    sf::Event flightEvent;
    while (flightWindow.isOpen())
    {
        while (flightWindow.pollEvent(flightEvent))
        {
            switch (flightEvent.type)
            {
            case sf::Event::Closed:
                flightWindow.close();
                break;
            case sf::Event::TextEntered:
                textbox1.typedOn(flightEvent);
                break;
            case sf::Event::KeyPressed:
                switch (flightEvent.key.code)
                {
                case sf::Keyboard::Key::Return:
                {
                    textbox1.setSelected(false);
                    choice = textbox1.getText();
                    flightWindow.close();
                    if (choice.empty())
                        break;

                    auto data = find_flight_by_departure(upper(strip(choice)));
                    intl_flights = data.first, dom_flights = data.second;
                    if (intl_flights.size() > 0 || dom_flights.size() > 0)
                    {
                        flightFound = true;
                        showFlights(intl_flights, dom_flights);
                    }

                    if (flightFound == false)
                    {
                        drawDialog("ERROR: FLIGHTS FROM INPUT COUNTRY NOT FOUND, PRESS ESC TO TRY AGAIN");
                    }
                    break;
                }
                case sf::Keyboard::Key::Escape:
                    flightWindow.close();
                    break;
                }
                break;
            }
        }
        flightWindow.clear();
        flightWindow.draw(text1);
        textbox1.draw(flightWindow);
        flightWindow.display();
    }
}

void showFlights(BinaryTree_<string, string> intl_flights, BinaryTree_<string, string> dom_flights)
{
    sf::RenderWindow flightWindow(sf::VideoMode::getDesktopMode(), "AVAILABLE FLIGHTS");

    int intl_n = intl_flights.size();
    int dom_n = dom_flights.size();
    int n = intl_n + dom_n;

    sf::Text *intl = new sf::Text[intl_n];
    sf::Text *dom = new sf::Text[dom_n];

    sf::View titleView, intl_view, dom_view;

    intl_view.reset(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
    intl_view.setViewport({0, 0.1, .5, 1});

    dom_view.reset(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
    dom_view.setViewport({.5, 0.1, 1, 1});

    titleView.reset(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
    titleView.setViewport({0, 0, 1, .1});

    sf::Font font;
    font.loadFromFile("fonts/CaviarDreams.ttf");

    auto gt_header = [&](string title, float pos) {
        sf::Text ret(title, font);
        ret.setFillColor(sf::Color::Red);
        ret.setOrigin(ret.getLocalBounds().width / 2, ret.getLocalBounds().height / 2);
        ret.setCharacterSize(32);
        ret.setPosition(float(windowSize.x) * .25 * pos, 350);
        ret.setScale(1, 9);
        return ret;
    };

    auto intl_header = gt_header("AVAILABLE INTERNATIONAL FLIGHTS:", 1);
    auto dom_header = gt_header("AVAILABLE DOMESTIC FLIGHTS:", 2.9);

    sf::Text unav_msg1("International flights currently unavailable", font);
    unav_msg1.setOrigin(unav_msg1.getLocalBounds().width / 2, unav_msg1.getLocalBounds().height / 2);
    unav_msg1.setPosition(flightWindow.getSize().x / 2, (flightWindow.getSize().y / (n + 5)) * 2);

    sf::Text unav_msg2("Domestic flights currently unavailable", font);
    unav_msg2.setOrigin(unav_msg2.getLocalBounds().width / 2, unav_msg2.getLocalBounds().height / 2);
    unav_msg2.setPosition(flightWindow.getSize().x / 2, (flightWindow.getSize().y / (n + 5)) * (1 + 4));

    int i = 0;
    intl_flights.traverse([&](string first, string second) -> void {
        intl[i].setString(first + ("\n->") + second);
        intl[i].setFont(font);
        intl[i].setCharacterSize(25);
        intl[i].setScale(2, 1);
        intl[i].setFillColor(sf::Color::White);
        intl[i].setPosition(windowSize.x * .05, windowSize.y / 7 * (i + 1));

        i++;
    });

    i = 0;
    dom_flights.traverse([&](string first, string second) {
        dom[i].setString(first + ("\n->") + second);
        dom[i].setFont(font);
        dom[i].setCharacterSize(25);
        // dom[i].setScale(2, 1);
        dom[i].setFillColor(sf::Color::White);
        dom[i].setPosition(0, windowSize.y / 7 * (i + 1));
        i++;
    });

    auto vc = intl_view.getCenter(), vc1 = dom_view.getCenter();
    sf::Event flightEvent;

    auto handle_scroll = [&](int mnmnt = 0) {
        mnmnt = mnmnt * 50;
        sf::View &view =
            (intl_view.getViewport().contains(float(mousePosWindow.x) / float(windowSize.x), float(mousePosWindow.y) / float(windowSize.y)))
                ? intl_view
                : dom_view;

        //print<'h'>('\r', mousePosWindow.x / windowSize.x, mousePosWindow.y / windowSize.y, mnmnt, vc.y, view.getCenter().y, vc1.y);
        if (mnmnt < 0 and vc.y <= view.getCenter().y - 100)
            view.move(0, mnmnt);
        else
            view.move(0, mnmnt);
    };

    while (flightWindow.isOpen())
    {

        while (flightWindow.pollEvent(flightEvent))
        {
            updateMousePosition(flightWindow);
            switch (flightEvent.type)
            {
            case sf::Event::Closed:
                flightWindow.close();
                break;
            case sf::Event::KeyPressed:
                switch (flightEvent.key.code)
                {
                case sf::Keyboard::Key::Escape:
                    flightWindow.close();
                    break;
                case sf::Keyboard::Key::Up:
                    handle_scroll(1);
                    break;
                case sf::Keyboard::Key::Down:
                    handle_scroll(-1);
                    break;
                }
            }
            if (flightEvent.type == flightEvent.MouseWheelScrolled)
                handle_scroll(flightEvent.mouseWheelScroll.delta);
        }

        //update the game

        flightWindow.clear();
        flightWindow.setView(titleView);
        flightWindow.draw(intl_header);
        flightWindow.draw(dom_header);

        flightWindow.setView(intl_view);
        if (intl_n == 0)
            flightWindow.draw(unav_msg1);
        for (i = 0; i < intl_n; i++)
            flightWindow.draw(intl[i]);

        flightWindow.setView(dom_view);
        if (dom_n == 0)
            flightWindow.draw(unav_msg2);
        for (i = 0; i < dom_n; i++)
            flightWindow.draw(dom[i]);

        flightWindow.display();
    }
}

auto getrect() -> sf::RectangleShape
{
    sf::RectangleShape rect;
    rect.setSize({100, 100});
    rect.setFillColor({255, 255, 255});
    rect.setOrigin({0, 0});
    rect.setPosition(displayCenter);
    return rect;
}

void displayTraffic()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "GLOBAL AVIATION DATABASE");
    sf::Font font;
    font.loadFromFile("fonts/CaviarDreams.ttf");
    linkedlist_<sf::Text> countryName = {};
    linkedlist_<sf::Text> departures = {};
    BinaryTree_<string, int> traffic;
    BinaryTree_<string, int> arr_traffic;
    sf::Clock cl;
    sf::Vector2f startpos = {(float)(windowSize.x * .25), (float)(windowSize.y / 7)};
    int totalCountries = 0, i = 0;
    bool loaded = false;
    int frame_cntr = 0;

    auto get_ttl = [&](string title, int position, bool scale = 1) -> sf::Text {
        sf::Text ret;
        ret.setString(title);
        ret.setFont(font);
        ret.setCharacterSize(30);
        ret.setFillColor(sf::Color::White);
        ret.setPosition(windowSize.x / 2, windowSize.y / 2);
        if (scale)
            ret.setScale(1, 9),
                ret.setPosition(startpos.x * position, windowSize.y * .3);
        ret.setOrigin(ret.getLocalBounds().width / 2, ret.getLocalBounds().height / 2);

        return ret;
    };

    auto cnt_ttl_txt = get_ttl("Country", 1);
    auto arr_ttl_txt = get_ttl("Arrivals", 2);
    auto dep_ttl_txt = get_ttl("Departures", 3);
    auto loading_txt = get_ttl("Loading...", 1, 0);
    string txt = "";
    auto load_ani_frm = [&]() {
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
    };

    auto show_data = [&]() {
        string country = "";

        auto arr_data_ = parse_flight("estArrivalAirport");
        // thread th();
        auto flt_data_ = parse_flight("estDepartureAirport");
        print("Data parsing completed!!");

        auto fill_traffic_ = [&]() {
            for (auto f : flt_data_.Keys())
            {
                country = upper(gt_ct_name(f));
                if (country != "NA")
                    ++traffic.getDefault({country, 1});
            }
        };
        auto fill_arr_traffic_ = [&]() {
            for (auto f : arr_data_.Keys())
            {
                country = upper(gt_ct_name(f));
                if (country != "NA")
                    ++arr_traffic.getDefault({country, 1});
            }
        };

        thread t1{fill_traffic_}, t2{fill_arr_traffic_};
        t1.join();
        t2.join();
    };

    auto data_to_text = [&]() {
        totalCountries = traffic.size();

        for (int j = 0; j < totalCountries; j++)
        {
            auto text = new sf::Text;
            auto text2 = new sf::Text;
            countryName.einsert(*text);
            departures.einsert(*text2);
        }

        traffic.traverse([&](string country, int dep_fights) -> void {
            countryName[i].setString(country);
            countryName[i].setFont(font);
            countryName[i].setCharacterSize(22);
            countryName[i].setFillColor(sf::Color::Cyan);
            countryName[i].setOrigin(countryName[i].getLocalBounds().width / 2, countryName[i].getLocalBounds().height / 2);
            countryName[i].setPosition(startpos.x, startpos.y * (i + 1));

            departures[i].setString(tostr(dep_fights) + "\t\t\t\t\t\t\t" + tostr((arr_traffic.getDefault({country, 0}))));
            departures[i].setFont(font);
            departures[i].setCharacterSize(40);
            departures[i].setFillColor(sf::Color::White);
            departures[i].setOrigin(departures[i].getLocalBounds().width / 2, departures[i].getLocalBounds().height / 2);
            departures[i].setPosition(startpos.x * 2.4, startpos.y * (i + 1));

            i++;
        });
        loaded = true;
        print("loaded");
    };

    auto value_loaded = async(launch::async, show_data);

    sf::View view, titleView;

    view.reset(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
    view.setViewport({0, 0.1, 1, .9});
    titleView.reset(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
    titleView.setViewport({0, 0, 1, .1});
    sf::Event event;
    auto vc = view.getCenter(); /* view center */

    while (window.isOpen())
    {

        updateMousePosition(window);
        while (window.pollEvent(event))
        {
            
            
            if (event.type == sf::Event::Closed)
                window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                if (vc.y <= view.getCenter().y - 100)
                    view.move({0, -10 * 5});
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                view.move({0, 10 * 5});
            if (event.type == event.MouseWheelScrolled)
                if (event.mouseWheelScroll.delta < 0)
                {
                    if (vc.y <= view.getCenter().y - 100)
                        view.move({0, event.mouseWheelScroll.delta * 50});
                }
                else
                    view.move({0, event.mouseWheelScroll.delta * 50});
        }

        window.setView(titleView);
        window.draw(cnt_ttl_txt);
        window.draw(arr_ttl_txt);
        window.draw(dep_ttl_txt);
        window.setView(view);
        if (is_ready(value_loaded))
        {
            if (!loaded)
                data_to_text();
            for (int j = 0; j < totalCountries; j++)
            {
                // print<'h'>("\rloaded", countryName.size(), countryName[j].getCharacterSize(), string(countryName[j].getString()));
                window.draw(countryName[j]);
                window.draw(departures[j]);
            }
        }
        else
            load_ani_frm(),
                window.draw(loading_txt);

        window.display();
        window.clear();
    }
}

auto updateMousePosition(sf::RenderWindow &window) -> void
{
    mousePosScreen = sf::Mouse::getPosition();
    mousePosWindow = sf::Mouse::getPosition(window);
    mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window)); //now it give the position of the coordinates of a view inside the window no the scree
}
void drawAllMap()
{
    linkedlist_<string> callsign;
    linkedlist_<float> latitude;
    linkedlist_<float> longitude;
    linkedlist_<float> tilt;
    linkedlist_<float> velocity;
    linkedlist_<float> altitude;

    auto val = parse_all_state("callsign");
    
    string callsign_[10000];
    float latitude_[10000];
    float longitude_[10000];
    float tilt_[10000];
    float velocity_[10000];
    float altitude_[10000];
    
    int i = 0;
    for (auto f : val.Keys())
    {
        try
        {
            /*
            callsign.einsert(f);
            latitude.einsert(stof((*val[f])[6]));
            longitude.einsert(stof((*val[f])[5]));
            tilt.einsert(stof((*val[f])[10]));
            velocity.einsert(stof((*val[f])[9]));
            altitude.einsert(stof((*val[f])[13]));
             */
            callsign_[i]=f;
            latitude_[i]=stof((*val[f])[6]);
            longitude_[i]=stof((*val[f])[5]);
            tilt_[i]=stof((*val[f])[10]);
            velocity_[i]=stof((*val[f])[9]);
            altitude_[i]=stof((*val[f])[13]);
            i++;
            }
    catch (exception e) {
        continue;
        }
    //(*val[f]).show();
    }
    int size = i;
    //print(callsign.size(),latitude.size(),longitude.size(),velocity.size(),altitude.size(),tilt.size());

    sf::RenderWindow mapWindow(sf::VideoMode(WIDTH, HEIGHT), "Map Window");

    sf::Image mapImg;
    if (!mapImg.loadFromFile("map.jpg"))
        std::cout << "Coudln't load the map image!" << std::endl;

    sf::Texture mapTexture;
    mapTexture.loadFromImage(mapImg); //Load Texture from file

    sf::Sprite mapSprite(mapTexture);
    mapSprite.setPosition(sf::Vector2f(WIDTH / 2, HEIGHT / 2));
    mapSprite.setOrigin(WIDTH / 2, HEIGHT / 2);

    sf::Texture planeTexture;
    planeTexture.loadFromFile("plane.png");

    sf::Sprite planeSprite[size];
    
    for(int i=0;i<size;i++)
    {
    planeSprite[i].setTexture(planeTexture);
    planeSprite[i].setScale(0.08, 0.08);
    planeSprite[i].setPosition(ORIGIN_X + (longitude_[i] * 3.7), ORIGIN_Y - (latitude_[i] * 3.8));
    planeSprite[i].setRotation(90);
    planeSprite[i].rotate(tilt_[i]);
    planeSprite[i].setOrigin(planeSprite[i].getLocalBounds().width / 2, planeSprite[i].getLocalBounds().height / 2);
    }
    while (mapWindow.isOpen())
    {
        updateMousePosition(mapWindow);
        sf::Event mapEvent;
        while (mapWindow.pollEvent(mapEvent))
        {

            switch (mapEvent.type)
            {
            case sf::Event::KeyPressed:
                switch (mapEvent.key.code)
                {
                case sf::Keyboard::Key::Escape:
                    mapWindow.close();
                    break;
                }
                break;
            case sf::Event::Closed:
                mapWindow.close();
                break;
            }
        }

        mapWindow.clear();
        mapWindow.draw(mapSprite);
        
        for(int i=0; i<size; i++)
        {
                mapWindow.draw(planeSprite[i]);
        
        }
        for(int i=0; i<size; i++)
        {
            if(mousePosWindow.x<(int)(ORIGIN_X + (longitude_[i] * 3.7)+3) &&mousePosWindow.x>(int)(ORIGIN_X + (longitude_[i] * 3.7)-3) && mousePosWindow.y<(int) (ORIGIN_Y - (latitude_[i] * 3.8)+3) && mousePosWindow.y>(int) (ORIGIN_Y - (latitude_[i] * 3.8)-3))
            {
                planeSprite[i].setScale(0.5,0.5);
                planeSprite[i].setColor(sf::Color::Blue);
                mapWindow.draw(planeSprite[i]);
                planeSprite[i].setColor(sf::Color::Red);
                planeSprite[i].setScale(0.08,0.08);

                
                drawInfoBox(mapWindow, ORIGIN_X + (longitude_[i] * 3.7), ORIGIN_Y - (latitude_[i] * 3.8)-7,callsign_[i], velocity_[i], latitude_[i], longitude_[i], altitude_[i]);
            break;
            
            }
        }
        mapWindow.display();
    }
}

#endif
