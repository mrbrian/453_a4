#include "scene.h"
#include "polyroots.h"
#include <qdebug.h>

#define MAX_DEPTH   5
#define BG_COLOR    Colour(0,0,0)

Light::Light(Point3D pos, Colour a, Colour d, Colour s)
{
    position = pos;
    Ia = a;
    Id = d;
    Is = s;
}

Colour *Scene::Render()
{
    Colour * result = new Colour[cam.imgWidth * cam.imgHeight];
    // iterate over the pixels & set colour values
    for (int x = 0; x < cam.imgWidth; x++)
    {
        for (int y = 0; y < cam.imgHeight; y++)
        {
            // determine ray vector
            Point3D p = imgPlane->ImageToWorldSpace(x, y, cam.imgWidth, cam.imgHeight);
            Vector3D v = p - cam.position;
            v.normalize();

            Colour &c = result[x + y * cam.imgWidth];

            if (!trace_ray(cam.position, v, this, &c, 1))   // if ray hit nothing
                c = BG_COLOR;                               // use background color
        }
    }
    return result;
}

// check if objects lay between a point and light position
bool intersection_test(Scene *scene, Point3D o, Point3D light_pos)
{
    Vector3D v = light_pos - o;
    double dist = v.length();
    v.normalize();

    float tmin = INFINITY;

    for(std::vector<SceneObject*>::iterator it = scene->objects.begin(); it != scene->objects.end(); ++it)
    {
        SceneObject *object= (*it);

        Vector3D n;
        double t = object->intersect(o, v, &n);      // whats the n?    -1 if no hit.

        if (0 <= t && t <= tmin)
            tmin = t;
    }
    if (tmin > dist)
        return false;
    if (tmin < 0)
        return false;

    return true;
}

bool trace_ray(Point3D o, Vector3D v, Scene *scene, Colour *color, int depth)
{
    if (depth > MAX_DEPTH)  // stop recursing
        return false;

    bool debug = scene->debug;
    if (debug)
       qDebug() << "trace" << depth;

    double t_min = INFINITY;
    Vector3D n_min;
    SceneObject *hitObject = NULL;
    v.normalize();

    Colour &col = *color;
    if (depth == 1)         // start ... with no colour
        col = Colour(0,0,0);

    for(std::vector<SceneObject*>::iterator it = scene->objects.begin(); it != scene->objects.end(); ++it)
    {
        SceneObject *obj = (*it);

        Vector3D n;
        double t = obj->intersect(o, v, &n);      // whats the n?

        if (0 <= t && t < t_min)
        {
            t_min = t;
            if (n.dot(v) >= 0)
                n = -n;
            n_min = n;
            hitObject = obj;
        }
    }

    if (hitObject == NULL)              // check for no intersection
    {
       if (debug)
           qDebug() << "miss";
        return false;
    }

    Point3D p_int = o + t_min * v;      // calculate intersection point

    p_int = p_int + 0.001 * n_min;      // pull back point a bit, avoid self intersection

    if (debug)
    {
        qDebug() << "nmin  " << n_min[0] << n_min[1] << n_min[2];
        qDebug() << "v     " << v[0] << v[1] << v[2];
        qDebug() << "p_int " << p_int[0] << p_int[1] << p_int[2];
        qDebug() << "hit   " << hitObject->type();
    }
    // found closest intersection
    Point3D p = o + t_min * v;

    for(std::vector<Light*>::iterator it = scene->lights.begin(); it != scene->lights.end(); ++it)
    {
        Light *light= (*it);
        col = col + hitObject->material->Ka * light->Ia;

        Point3D new_p = p + 0.1 * (light->position - p);
        if (intersection_test(scene, new_p, light->position))
           continue;

        Vector3D n = n_min;
        Vector3D l = light->position - p;
        l.normalize();
        Vector3D r = -l + 2 * (l.dot(n)) * n;

        // add diffuse color
        col = col + (hitObject->material->GetKd(p) * fmax(n.dot(l), 0) * light->Id);
        // add specular color
        col = col + (hitObject->material->Ks * pow(fmax(r.dot(-v), 0), hitObject->material->p) * light->Is);
    }

    Colour reflect_rgb;     // reflection color;

    // calculate reflect vector
    Vector3D r = v + (2 * n_min.dot(-v)) * n_min;

    if (debug)
        qDebug() << "r     " << r[0] << r[1] << r[2];

    trace_ray(p_int, r, scene, &reflect_rgb, depth + 1);
    // add reflection color
    col = col + reflect_rgb * hitObject->material->Kr;
    return true;
}
