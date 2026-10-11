#include "CoordinateMapping.h"

#include <cmath>

using namespace std;

//------------------------------------------------------------//
// Identity Mapping
//------------------------------------------------------------//

void CoordinateMapping::identity(Mesh& mesh)
{
    // No change
}

//------------------------------------------------------------//
// Trapezoidal Mapping
//------------------------------------------------------------//

void CoordinateMapping::trapezoidal
(
    Mesh& mesh,
    double topScale
)
{
    double ymin = mesh.ymin;
    double ymax = mesh.ymax;

    for(auto& node : mesh.nodes)
    {
        double eta = (node.y-ymin)/(ymax-ymin);

        node.x *= (1.0 + eta*(topScale-1.0));
    }
}

//------------------------------------------------------------//
// Sinusoidal Mapping
//------------------------------------------------------------//

void CoordinateMapping::sinusoidal
(
    Mesh& mesh,
    double amplitude
)
{
    const double pi = acos(-1.0);

    double xmin = mesh.xmin;
    double xmax = mesh.xmax;

    double L = xmax-xmin;

    for(auto& node : mesh.nodes)
    {
        node.y += amplitude*
                  sin(2.0*pi*(node.x-xmin)/L);
    }
}

//------------------------------------------------------------//
// Polar Mapping
//------------------------------------------------------------//

void CoordinateMapping::polar
(
    Mesh& mesh,
    double rInner,
    double rOuter
)
{
    double xmin = mesh.xmin;
    double xmax = mesh.xmax;

    double ymin = mesh.ymin;
    double ymax = mesh.ymax;

    for(auto& node : mesh.nodes)
    {
        double xi =
            (node.x-xmin)/(xmax-xmin);

        double eta =
            (node.y-ymin)/(ymax-ymin);

        double r =
            rInner +
            eta*(rOuter-rInner);

        double theta =
            xi*2.0*acos(-1.0);

        node.x =
            r*cos(theta);

        node.y =
            r*sin(theta);
    }
}