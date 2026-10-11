/*
Compiler instruction: g++ 02_MultiBlock.cpp src/*.cpp -Iinclude -std=c++17 -o MultiBlock && ./MultiBlock
*/
#include <iostream>

#include "Point.h"
#include "Line.h"
#include "Surface.h"

#include "Block.h"
#include "MultiBlockMesh.h"

#include "MeshStatistics.h"
#include "MeshWriter.h"

int main()
{
    std::cout << "==========================================" << std::endl;
    std::cout << " Example 02 : Multi Block Mesh" << std::endl;
    std::cout << "==========================================" << std::endl;

    //------------------------------------------------------------
    // Block 1 Geometry
    //------------------------------------------------------------

    Point p0(0,0.0,0.0);
    Point p1(1,5.0,0.0);
    Point p2(2,5.0,2.0);
    Point p3(3,0.0,2.0);

    Line l0(0,&p0,&p1);
    Line l1(1,&p1,&p2);
    Line l2(2,&p2,&p3);
    Line l3(3,&p3,&p0);

    Surface block1Surface(0);

    block1Surface.addBoundary(&l0);
    block1Surface.addBoundary(&l1);
    block1Surface.addBoundary(&l2);
    block1Surface.addBoundary(&l3);

    //------------------------------------------------------------
    // Block 2 Geometry
    //------------------------------------------------------------

    Point p4(4,5.0,0.0);
    Point p5(5,10.0,0.0);
    Point p6(6,10.0,2.0);
    Point p7(7,5.0,2.0);

    Line l4(4,&p4,&p5);
    Line l5(5,&p5,&p6);
    Line l6(6,&p6,&p7);
    Line l7(7,&p7,&p4);

    Surface block2Surface(1);

    block2Surface.addBoundary(&l4);
    block2Surface.addBoundary(&l5);
    block2Surface.addBoundary(&l6);
    block2Surface.addBoundary(&l7);

    //------------------------------------------------------------
    // Create Blocks
    //------------------------------------------------------------

    Block block1
    (
        0,
        block1Surface,
        20,
        10
    );

    Block block2
    (
        1,
        block2Surface,
        20,
        10
    );

    //------------------------------------------------------------
    // Assemble Multi Block Mesh
    //------------------------------------------------------------

    MultiBlockMesh mesh;

    mesh.addBlock(block1);
    mesh.addBlock(block2);

    mesh.generate();

    //------------------------------------------------------------
    // Statistics
    //------------------------------------------------------------

    MeshStatistics::print(mesh);

    //------------------------------------------------------------
    // Export
    //------------------------------------------------------------

    MeshWriter::writeVTK
    (
        mesh,
        "twoBlockMesh.vtk"
    );

    return 0;
}