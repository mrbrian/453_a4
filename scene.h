#ifndef SCENE
#define SCENE

#include "shapes.h"
#include "algebra.h"
#include <vector>

class Camera
{
public:
    int imgWidth;       // image resolution width
    int imgHeight;      // image resolution height
    Point3D position;   // camera position
    Point3D lookAt;     // camera lookat
};

class Light
{
public:
    Light(Point3D pos, Colour a, Colour d, Colour s);
    Colour Ia;      // ambient
    Colour Id;      // diffuse
    Colour Is;      // specular
    Point3D position;
    void Transform(Matrix4x4 m)
    {
        position = m * position;
    }
};

class Plane
{
public:
    Point3D *points;

    Plane(Point3D a, Point3D b, Point3D c, Point3D d)
    {
        points = new Point3D[4] {a,b,c,d};
    }

    void Transform(Matrix4x4 m)
    {
        for (int i = 0; i < 4 ;i ++)
        {
            points[i] = m * points[i];
        }
    }

    Point3D ImageToWorldSpace(int x, int y, int imgWidth, int imgHeight)
    {
        Point3D topleft  = points[0];
        Point3D topright = points[3];
        Point3D botleft  = points[1];

        Vector3D right = topright - topleft;
        Vector3D down  = botleft - topleft;

        double h_pct = (float)x / imgWidth;
        double v_pct = (float)y / imgHeight;

        Point3D result = (topleft + (h_pct * right) + (v_pct * down));
        return result;
    }
};

class ImagePlane
{
public:
    Point3D center;
    int width;
    int height;

    ImagePlane(Point3D c, int w, int h)
    {
        center = c;
        width = w;
        height = h;
    }
};

class Scene
{
public:
    bool debug;
    Camera cam;
    Plane *imgPlane;
    std::vector<SceneObject*> objects;
    std::vector<Light*> lights;
    Colour * Render();

    void Transform(Matrix4x4 m)
    {
        for(std::vector<SceneObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
        {
            SceneObject *obj = (*it);
            obj->Transform(m);
        }
        for(std::vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
        {
            Light *obj = (*it);
            obj->Transform(m);
        }
    }
};

bool trace_ray(Point3D o, Vector3D v, Scene *scene, Colour *color, int depth);

#endif //SCENE
