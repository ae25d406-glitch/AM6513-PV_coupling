/*
Compiling Instruction: g++ 01_CartesianMesh.cpp src/*.cpp -Iinclude -std=c++17 -o cartesianMesh && ./cartesianMesh
*/
#include <iostream>

#include "Point.h"
#include "Line.h"
#include "Surface.h"

#include "Mesh.h"
#include "MeshStatistics.h"
#include "MeshWriter.h"

int main()
{
    std::cout << "==========================================" << std::endl;
    std::cout << " Example 01 : Cartesian Mesh Generation" << std::endl;
    std::cout << "==========================================" << std::endl;

    //------------------------------------------------------------
    // Step 1 : Create Geometry
    //------------------------------------------------------------

    Point p0(0,0.0,0.0);
    Point p1(1,4.0,0.0);
    Point p2(2,4.0,2.0);
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
    // Print Geometry
    //------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Geometry Created" << std::endl;
    std::cout << "----------------" << std::endl;

    rectangle.print();

    //------------------------------------------------------------
    // Step 2 : Generate Cartesian Mesh
    //------------------------------------------------------------

    Mesh mesh;

    mesh.generateCartesian
    (
        rectangle,
        200,     // Nx
        100      // Ny
    );

    //------------------------------------------------------------
    // Step 3 : Print Mesh Statistics
    //------------------------------------------------------------

    MeshStatistics::print(mesh);

    //------------------------------------------------------------
    // Step 4 : Export Mesh
    //------------------------------------------------------------

    MeshWriter::writeVTK
    (
        mesh,
        "cartesianMesh.vtk"
    );
    return 0;
}