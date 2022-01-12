#include <iostream>
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"

#define __TEST__ std::cout<<"TEST"<<std::endl;

int main()
{
    // sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);
    
    std::array<std::array<Value, 4>, 4> testKmap = {{
        {Value::one, Value::one, Value::one, Value::one},
        {Value::one, Value::one, Value::zero, Value::one},
        {Value::one, Value::one, Value::zero, Value::one},
        {Value::zero, Value::one, Value::zero, Value::one}
    }};

    Map4x4 kmapObject;
    kmapObject.initializeKmapWith(testKmap);
    kmapObject.countZeroesAndOnes();
    kmapObject.printKmap();
    kmapObject.findGroups();
    kmapObject.printSquare2x2Groups();
    kmapObject.printRect4x2Group();
    kmapObject.printRect4x1Group(); 
    kmapObject.printRect2x1Group(); 
    kmapObject.print1x1Group(); 
    kmapObject.printCellsNumberOfGroups();
    // kmap.printKmap();
    // kmap.printGroup();
    // kmap.moveGroup(-2, -2);
    // kmap.printGroup();
    // std::cout << -1 % 4 << std::endl;

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

    //     window.clear();
    //     window.draw(shape);
    //     window.display();
    // }

    return 0;
}