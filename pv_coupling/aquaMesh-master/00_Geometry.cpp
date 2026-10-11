/*
Compilation Instruction:
g++ 00_Geometry.cpp src/*.cpp -Iinclude -std=c++17 -o geometry && ./geometry
*/

#include <iostream>

#include "Point.h"
#include "Line.h"
#include "Surface.h"

int main()
{
    std::cout << "==========================" << std::endl;
    std::cout << " Example 00 : Geometry" << std::endl;
    std::cout << "==========================\n" << std::endl;

    //--------------------------------------------------
    // Create Points
    //--------------------------------------------------

    Point p0(0,0.0,0.0);
    Point p1(1,5.0,0.0);
    Point p2(2,5.0,2.0);
    Point p3(3,0.0,2.0);

    p0.print();
    p1.print();
    p2.print();
    p3.print();

    //--------------------------------------------------
    // Create Lines
    //--------------------------------------------------

    Line l0(0,&p0,&p1);
    Line l1(1,&p1,&p2);
    Line l2(2,&p2,&p3);
    Line l3(3,&p3,&p0);

    std::cout << "\n";

    l0.print();
    l1.print();
    l2.print();
    l3.print();

    //--------------------------------------------------
    // Create Surface
    //--------------------------------------------------

    Surface rectangle(0);

    rectangle.addBoundary(&l0);
    rectangle.addBoundary(&l1);
    rectangle.addBoundary(&l2);
    rectangle.addBoundary(&l3);

    rectangle.print();

    return 0;
}