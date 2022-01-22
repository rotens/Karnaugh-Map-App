#include <iostream>
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"
#include "Map1x2.hpp"
#include "Map2x2.hpp"

#define __TEST__ std::cout<<"TEST"<<std::endl;

int main()
{
    // sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);
    
    // std::array<std::array<Value, 4>, 4> testKmap = {{
    //     {Value::one, Value::one, Value::one, Value::one},
    //     {Value::one, Value::one, Value::zero, Value::one},
    //     {Value::one, Value::one, Value::one, Value::one},
    //     {Value::zero, Value::one, Value::one, Value::one}
    // }};


    // Map1x2 map1x2Object;
    // map1x2Object.changeValue(0, Value::one);
    // map1x2Object.findAlgebraicMinterm();
    // std::cout << map1x2Object.getAlgebraicMinterm() << std::endl;

    // map1x2Object.changeValue(1, Value::one);
    // map1x2Object.findAlgebraicMinterm();
    // std::cout << map1x2Object.getAlgebraicMinterm() << std::endl;

    // map1x2Object.changeValue(0, Value::zero);
    // map1x2Object.findAlgebraicMinterm();
    // std::cout << map1x2Object.getAlgebraicMinterm() << std::endl;


    std::array<std::array<Value, 2>, 2> testKmap = {{
        {Value::zero, Value::one},
        {Value::one, Value::zero}
    }};

    Map2x2 map2x2Object;
    map2x2Object.initializeKmapWith(testKmap);
    map2x2Object.solve();
    for (const auto& minterm : map2x2Object.getAlgebraicMinterms())
    {
        std::cout << minterm << " + "; 
    }


    // while (window.isOpen())
    // {
    //     sf::Event event;
    //     while (window.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //         if (event.type == sf::Event::MouseButtonPressed)
    //             std::cout << "Test" << std::endl;
    //     }

    //     window.clear();s
    //     window.draw(shape);
    //     window.display();
    // }

    return 0;
}