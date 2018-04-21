#ifndef SHAPES
#define SHAPES

#include <qdebug.h>
#include "algebra.h"
#include "scene.h"
#include "material.h"
#include "polyroots.h"

class SceneObject
{
public:
    Material *material;
    virtual char const *type()
    {
        return "SceneObject";
    }

    virtual void Transform(Matrix4x4 m)
    {
    }

    virtual double intersect(Point3D o, Vector3D v, Vector3D *n)
    {
        o = Point3D();  // get rid of warnings
        v = *n;
        return -1;
    }        
};

class Triangle : public SceneObject
{
public:
    Point3D verts[3];

    char const *type()
    {
        return "Triangle";
    }

    Triangle(Point3D p1, Point3D p2, Point3D p3, Material *mat)
    {
        material = mat;
        verts[0] = p1;
        verts[1] = p2;
        verts[2] = p3;
    }

    double area(Point3D a, Point3D b, Point3D c)
    {
        Vector3D ab = b - a;
        Vector3D ac = c - a;
        double area2 = 0.5 * ab.cross(ac).length();
        return area2;
    }

    double intersect(Point3D o, Vector3D v, Vector3D *n)
    {
        v.normalize();
        Vector3D &tri_normal = *n;
        tri_normal = (verts[1] - verts[0]).cross(verts[2] - verts[0]);
        tri_normal.normalize();

        Point3D tri_pt;
        tri_pt[0] = (verts[0][0] + verts[1][0] + verts[2][0]) / 3;
        tri_pt[1] = (verts[0][1] + verts[1][1] + verts[2][1]) / 3;
        tri_pt[2] = (verts[0][2] + verts[1][2] + verts[2][2]) / 3;

        Vector3D delta = o - tri_pt;
        double t = -delta.dot(tri_normal)/(v.dot(tri_normal));
        Point3D p = o + t * v;

        double total_area = 0.5 * ((verts[0] - verts[1]).cross(verts[0] - verts[2])).length();

        double s1 = 0.5 * ((verts[2] - verts[1]).cross(p - verts[1])).dot(tri_normal);
        double s2 = 0.5 * ((verts[0] - verts[2]).cross(p - verts[2])).dot(tri_normal);

        double alpha = s1 / total_area;
        double beta  = s2 / total_area;
        double y     = 1 - alpha - beta;

        if (alpha < 0 || alpha > 1)
            t = -1;
        if (beta < 0 || beta > 1)
            t = -1;
        if (y < 0 || y > 1)
            t = -1;

        return t;
    }

    void Transform(Matrix4x4 m)
    {
        for (int i = 0; i < 3 ;i ++)
        {
            verts[i] = m * verts[i];
        }
    }
};

class Quad : public SceneObject
{
public:
    Triangle **tris;    // array of triangle pointers
    Point3D *points;

    char const *type()
    {
        return "Quad";
    }

    Quad(Point3D a, Point3D b, Point3D c, Point3D d, Material *mat)
    {
        points = new Point3D[4] {a,b,c,d};
        tris = new Triangle*[2];
        tris[0] = new Triangle(a,b,c,mat);
        tris[1] = new Triangle(a,c,d,mat);

        material = mat;
    }

    void Transform(Matrix4x4 m)
    {
        for (int j = 0; j < 2 ; j++)
        {
            tris[j]->Transform( m );
        }
    }

    double intersect(Point3D o, Vector3D v, Vector3D *n)
    {
        for (int i = 0 ; i < 2; i++)
        {
            double t = tris[i]->intersect(o,v,n);
            if (t >= 0)
            {
                return t;
            }
        }
        return -1;
    }

    Point3D ImageToWorldSpace(int x, int y, int imgWidth, int imgHeight)
    {
        Point3D topleft  = points[0];
        Point3D topright = points[3];
        Point3D botleft  = points[1];

        Vector3D right = topright - topleft;
        Vector3D down = topleft - botleft;

        double h_pct = (float)x / imgWidth;
        double v_pct = (float)y / imgHeight;

        Point3D result = (topleft + (h_pct * right) + (v_pct * down));
        return result;
    }
};

class Sphere : public SceneObject
{
public:
    Point3D center;
    double radius;

    char const *type()
    {
        return "Sphere";
    }

    void Transform(Matrix4x4 m)
    {
        center = m * center;
    }

    Sphere(Point3D p, double r, Material *mat)
    {
        material = mat;
        radius = r;
        center = p;
    }

    double intersect(Point3D o, Vector3D v, Vector3D *n)
    {
        double t = 0;

        Vector3D D = o - center;

        double a = 1;
        double b = 2 * (v.dot(D));
        double c = D.dot(D) - (radius * radius);
        double roots[2];
        int count = quadraticRoots(a, b, c, roots);

        if (count == 0)
            return -1;

        if (count >= 1)
        {
            double minRoot = fmin(roots[0], roots[1]);

            if (minRoot < 0)
                t = fmax(roots[0], roots[1]);
            else
                t = (fmin(roots[0], roots[1]));
        }

        Vector3D &outN = *n;
        outN = (o + t * v) - center;
        outN.normalize();

        return t;
    }
};


#endif //SHAPES
