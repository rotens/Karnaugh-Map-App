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
        {Value::one, Value::one, Value::one, Value::one},
        {Value::zero, Value::one, Value::one, Value::one}
    }};

    // std::array<std::array<Value, 4>, 4> testKmap = {{
    //     {Value::zero, Value::zero, Value::zero, Value::zero},
    //     {Value::zero, Value::one, Value::one, Value::zero},
    //     {Value::zero, Value::one, Value::one, Value::zero},
    //     {Value::zero, Value::zero, Value::zero, Value::zero}
    // }};

    Map4x4 kmapObject;
    kmapObject.initializeKmapWith(testKmap);
    kmapObject.printKmap();
    kmapObject.solve();
    kmapObject.printSquare2x2Groups();
    // kmapObject.printRect4x2Group();
    // kmapObject.printRect4x1Group(); 
    // kmapObject.printRect2x1Group(); 
    // kmapObject.print1x1Group(); 
    // kmapObject.printMinterms();


    // std::vector<int> test2;
    // std::vector<int> test{1,2,3,4,5,6,7,8,9};

    // int k = 0;
    // for (int i = 0; i <= test.size() - 4; ++i)
    // {
    //     for (int k = i+1; k <= test.size() - 3; ++k)
    //     {
    //         std::cout << test[i];
    //         for (int j = k; j < k+3; ++j)
    //         {
    //             std::cout << test[j];
    //         }
    //         std::cout << std::endl;
    //     }
        
    //     std::cout << std::endl;
    // }


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