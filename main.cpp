#include <iostream>
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"
#include "Map1x2.hpp"
#include "Map2x2.hpp"
#include "MapInterface.h"

#define __TEST__ std::cout<<"TEST"<<std::endl;

int main()
{
    
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


    // std::array<std::array<Value, 2>, 2> testKmap = {{
    //     {Value::zero, Value::one},
    //     {Value::one, Value::zero}
    // }};

    // Map2x2 map2x2Object;
    // map2x2Object.initializeKmapWith(testKmap);
    // map2x2Object.solve();
    // for (const auto& minterm : map2x2Object.getAlgebraicMinterms())
    // {
    //     std::cout << minterm << " + "; 
    // }

    MapInterface mapInterface;
    mapInterface.loop();
    

    return 0;
}