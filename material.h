#ifndef MATERIAL
#define MATERIAL

#include "algebra.h"

class Material
{
public:
    Material(Colour a, Colour d, Colour s, double p, Colour r);
    Material(Colour a, Colour d, Colour s, double p);

    // accessor for diffuse color
    virtual Colour GetKd(Point3D p)
    {
        p = Point3D();  // get rid of warning
        return Kd;
    }

    Colour Ka;      // ambient coefficient
    Colour Ks;      // specular coefficient
    double p;       // specular power
    Colour Kr;      // reflection coefficient
    Colour Kd;      // only access kd through accessor func
};

class ChromeMaterial : public Material
{
public:
    ChromeMaterial()
        : Material( Colour(0,0,0), Colour(0,0,0), Colour(1,1,1), 100, Colour(1,1,1))
    {
    }
};

class CheckerMaterial : public Material
{
public:
    float squareSize;      // size of the checkers
    CheckerMaterial(Colour a, Colour d, Colour s, double in_p, Colour r, float sc);
    Colour GetKd(Point3D p);
};

#endif // MATERIAL

