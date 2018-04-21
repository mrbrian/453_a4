#include "material.h"

#define DEF_REFLECT     Colour(0.1, 0.1, 0.1)

// procedural texture
Colour CheckerMaterial::GetKd(Point3D p)
{
    if (((int)(round(p[0] / squareSize) + round(p[1] / squareSize) + round(p[2] / squareSize)) % 2) == 0)
        return Colour(1,1,1) * Kd;
    else
        return Colour(0,0,0);
}

CheckerMaterial::CheckerMaterial(Colour a, Colour d, Colour s, double in_p, Colour r, float sc)
    : Material(a, d, s, in_p, r)
{
    squareSize = sc;
}

Material::Material(Colour a, Colour d, Colour s, double in_p, Colour r)
{
    Kr = r;
    Ka = a;
    Kd = d;
    Ks = s;
    p = in_p;
}

Material::Material(Colour a, Colour d, Colour s, double in_p)
{
    Kr = DEF_REFLECT;
    Ka = a;
    Kd = d;
    Ks = s;
    p = in_p;
}
