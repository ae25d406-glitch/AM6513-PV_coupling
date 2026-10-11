#ifndef COORDINATEMAPPING_H
#define COORDINATEMAPPING_H

#include "Mesh.h"

class CoordinateMapping
{
public:

    //------------------------------------------------------------
    // Identity Mapping
    //------------------------------------------------------------

    static void identity(Mesh& mesh);

    //------------------------------------------------------------
    // Trapezoidal Mapping
    //------------------------------------------------------------

    static void trapezoidal
    (
        Mesh& mesh,
        double topScale
    );

    //------------------------------------------------------------
    // Sinusoidal Mapping
    //------------------------------------------------------------

    static void sinusoidal
    (
        Mesh& mesh,
        double amplitude
    );

    //------------------------------------------------------------
    // Polar Mapping
    //------------------------------------------------------------

    static void polar
    (
        Mesh& mesh,
        double rInner,
        double rOuter
    );

};

#endif