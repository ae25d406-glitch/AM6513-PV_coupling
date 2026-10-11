/*
Compiler instruction: g++ 04_CoordinateMapping.cpp src/*.cpp -Iinclude -std=c++17 -o CoordinateMapping && ./CoordinateMapping
*/

#include <iostream>

#include "Point.h"
#include "Line.h"
#include "Surface.h"

#include "Mesh.h"
#include "MeshWriter.h"
#include "MeshStatistics.h"

#include "CoordinateMapping.h"

int main()
{
    std::cout << "=========================================\n";
    std::cout << " Example 04 : Coordinate Mapping\n";
    std::cout << "=========================================\n\n";

    //------------------------------------------------------------
    // Geometry
    //------------------------------------------------------------

    Point p0(0,0.0,0.0);
    Point p1(1,5.0,0.0);
    Point p2(2,5.0,2.0);
    Point p3(3,0.0,2.0);

    Line l0(0,&p0,&p1);
    Line l1(1,&p1,&p2);
    Line l2(2,&p2,&p3);
    Line l3(3,&p3,&p0);

    Surface rectangle(0);

    rectangle.addBoundary(&l0);
    rectangle.addBoundary(&l1);
    rectangle.addBoundary(&l2);
    rectangle.addBoundary(&l3);

    //------------------------------------------------------------
    // Original Mesh
    //------------------------------------------------------------

    Mesh original;

    original.generateCartesian
    (
        rectangle,
        40,
        20
    );

    MeshWriter::writeVTK
    (
        original,
        "mapping_original.vtk"
    );

    //------------------------------------------------------------
    // Trapezoidal Mapping
    //------------------------------------------------------------

    Mesh trapezoid = original;

    CoordinateMapping::trapezoidal
    (
        trapezoid,
        1.5
    );

    MeshWriter::writeVTK
    (
        trapezoid,
        "mapping_trapezoid.vtk"
    );

    //------------------------------------------------------------
    // Sinusoidal Mapping
    //------------------------------------------------------------

    Mesh wave = original;

    CoordinateMapping::sinusoidal
    (
        wave,
        0.25
    );

    MeshWriter::writeVTK
    (
        wave,
        "mapping_wave.vtk"
    );

    //------------------------------------------------------------
    // Polar Mapping
    //------------------------------------------------------------

    Mesh polar = original;

    CoordinateMapping::polar
    (
        polar,
        1.0,
        2.0
    );

    MeshWriter::writeVTK
    (
        polar,
        "mapping_polar.vtk"
    );

    std::cout << "\nGenerated Files\n";
    std::cout << "---------------\n";
    std::cout << "mapping_original.vtk\n";
    std::cout << "mapping_trapezoid.vtk\n";
    std::cout << "mapping_wave.vtk\n";
    std::cout << "mapping_polar.vtk\n";

    return 0;
}