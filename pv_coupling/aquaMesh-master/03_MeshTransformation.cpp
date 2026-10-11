/*
Compiler instruction: g++ 03_MeshTransformation.cpp src/*.cpp -Iinclude -std=c++17 -o MeshTransformation && ./MeshTransformation
*/

#include <iostream>

#include "Point.h"
#include "Line.h"
#include "Surface.h"

#include "Mesh.h"
#include "MeshWriter.h"
#include "MeshStatistics.h"
#include "MeshTransformation.h"

int main()
{
    std::cout << "==========================================\n";
    std::cout << " Example 03 : Mesh Transformation\n";
    std::cout << "==========================================\n\n";

    //------------------------------------------------------------
    // Step 1 : Create Geometry
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
    // Step 2 : Generate Original Mesh
    //------------------------------------------------------------

    Mesh originalMesh;

    originalMesh.generateCartesian
    (
        rectangle,
        40,
        20
    );

    MeshStatistics::print(originalMesh);

    MeshWriter::writeVTK
    (
        originalMesh,
        "mesh_original.vtk"
    );

    //------------------------------------------------------------
    // Translation
    //------------------------------------------------------------

    Mesh translatedMesh = originalMesh;

    MeshTransformation::translate
    (
        translatedMesh,
        2.0,
        1.0
    );

    MeshWriter::writeVTK
    (
        translatedMesh,
        "mesh_translate.vtk"
    );

    //------------------------------------------------------------
    // Scaling
    //------------------------------------------------------------

    Mesh scaledMesh = originalMesh;

    MeshTransformation::scale
    (
        scaledMesh,
        1.5,
        0.75
    );

    MeshWriter::writeVTK
    (
        scaledMesh,
        "mesh_scale.vtk"
    );

    //------------------------------------------------------------
    // Rotation
    //------------------------------------------------------------

    Mesh rotatedMesh = originalMesh;

    MeshTransformation::rotate
    (
        rotatedMesh,
        30.0
    );

    MeshWriter::writeVTK
    (
        rotatedMesh,
        "mesh_rotate.vtk"
    );

    //------------------------------------------------------------
    // Shear
    //------------------------------------------------------------

    Mesh shearedMesh = originalMesh;

    MeshTransformation::shear
    (
        shearedMesh,
        0.30,
        0.00
    );

    MeshWriter::writeVTK
    (
        shearedMesh,
        "mesh_shear.vtk"
    );

    //------------------------------------------------------------
    // Sinusoidal Transformation
    //------------------------------------------------------------

    Mesh waveMesh = originalMesh;

    MeshTransformation::sinusoidal
    (
        waveMesh,
        0.25,
        5.0
    );

    MeshWriter::writeVTK
    (
        waveMesh,
        "mesh_wave.vtk"
    );

    //------------------------------------------------------------
    // Summary
    //------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Generated Meshes\n";
    std::cout << "----------------\n";
    std::cout << "mesh_original.vtk\n";
    std::cout << "mesh_translate.vtk\n";
    std::cout << "mesh_scale.vtk\n";
    std::cout << "mesh_rotate.vtk\n";
    std::cout << "mesh_shear.vtk\n";
    std::cout << "mesh_wave.vtk\n";

    return 0;
}