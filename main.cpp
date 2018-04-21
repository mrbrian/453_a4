#include <QImage>
#include <QColor>
#include "algebra.h"
#include "scene.h"
#include <vector>
#include <QDebug>

#define DEF_WIDTH   500
#define DEF_HEIGHT  500

double clamp(double min, double max, double in)
{
    if (in > max)
        return max;
    if (in < min)
        return min;
    return in;
}

// reflection tri ball
void *SetupScene1(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,8);
    cam.position = Point3D(0,0,0);

    scene.cam = cam;
    scene.imgPlane = new Plane(Point3D(-0.5,0.5,1), Point3D(-0.5,-0.5,1), Point3D(0.5,-0.5,1), Point3D(0.5,0.5,1));

    Material *checker = new ChromeMaterial();
    Material *mat1   = new Material(Colour(0,0,0), Colour(1,0.25,0.1), Colour(1,1,1), 1000,  Colour(0.5,0.5,0.5));
    Material *mat2 = new Material(Colour(0,0,0), Colour(0.05,0.6,0.95), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));
    Material *mat3  = new Material(Colour(0,0,0), Colour(0.7,0.95,.13), Colour(1,1,1), 10, 0.5 * Colour(0.7,0.95,.13));

    Quad *q = new Quad(Point3D(-1,0,1),Point3D(-1,0,-1),Point3D(1,0,-1),Point3D(1,0,1), checker);
    q->Transform( Matrix4x4::translation(Vector3D(0,-1,0)) * Matrix4x4::scaling(Vector3D(2000,2000,2000)));
    Sphere *sphere1 = new Sphere(Point3D(-2,0,6), 1, mat1);
    Sphere *sphere2 = new Sphere(Point3D(0,0,10), 1, mat2);
    Sphere *sphere3 = new Sphere(Point3D(2,0,6), 1, mat3);
    scene.objects.push_back(sphere1);
    scene.objects.push_back(sphere2);
    scene.objects.push_back(sphere3);
    scene.objects.push_back(q);

    Light *light = new Light(Point3D(0,5,0), Colour(0.1,0.1,0.1), Colour(0.5,0.5,0.5), Colour(0,0,0));
    scene.lights.push_back(light);
    Light *light2 = new Light(Point3D(10,15,1), Colour(0.1,0.1,0.1), Colour(0.6,0.6,0.5), Colour(1,1,1));
    scene.lights.push_back(light2);
    Light *light3 = new Light(Point3D(-20,20,40), Colour(0.1,0.1,0.1), Colour(0.25,0.25,0.35), Colour(1,1,1));
    scene.lights.push_back(light3);

    scene.Transform(
                Matrix4x4::translation(Vector3D(cam.lookAt)) *
                Matrix4x4::rotation(30*M_PI/180,'y') *
                Matrix4x4::rotation(-35*M_PI/180,'x') *
                Matrix4x4::translation(-Vector3D(cam.lookAt))
                    );
}

// pretty tri ball + checker
void *SetupScene2(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;
    scene.imgPlane = new Plane(Point3D(-0.5,0.5,0), Point3D(-0.5,-0.5,0), Point3D(0.5,-0.5,0), Point3D(0.5,0.5,0));

    Material *checker = new CheckerMaterial(Colour(0,0,0), Colour(0.2,0.2,0.2), Colour(0.5,0.5,0.5), 100,  Colour(0.4,0.4,0.4), 0.4);
    Material *red   = new Material(Colour(0,0,0), Colour(1,0,0), Colour(1,1,1), 1000,  Colour(0.5,0.5,0.5));
    Material *green = new Material(Colour(0,0,0), Colour(0,1,0), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));
    Material *blue  = new Material(Colour(0,0,0), Colour(0,0,1), Colour(1,1,1), 10,  Colour(0.5,0.5,0.5));

    Quad *q1 = new Quad(Point3D(-10,0,10), Point3D(-10,0,-10), Point3D(10,0,-10), Point3D(10,0,10), checker);
    Matrix4x4 m = Matrix4x4::translation(Vector3D(0,-1,0)) * Matrix4x4::scaling(Vector3D(2,2,2));
    q1->Transform(m);
    scene.objects.push_back(q1);

    Sphere *sphere1 = new Sphere(Point3D(-2,0,5), 1,red);
    Sphere *sphere2 = new Sphere(Point3D(0,0,6), 1, green);
    Sphere *sphere3 = new Sphere(Point3D(2.5,0,7), 1, blue);
    scene.objects.push_back(sphere1);
    scene.objects.push_back(sphere2);
    scene.objects.push_back(sphere3);

    Light *light = new Light(Point3D(0,2,0), Colour(0.1,0.1,0.1), Colour(0.5,0.5,0.5), Colour(0,0,0));
    scene.lights.push_back(light);
    Light *light2 = new Light(Point3D(10,15,1), Colour(0.1,0.1,0.1), Colour(0.6,0.6,0.5), Colour(1,1,1));
    scene.lights.push_back(light2);
    Light *light3 = new Light(Point3D(-20,20,40), Colour(0.1,0.1,0.1), Colour(0.25,0.25,0.35), Colour(1,1,1));
    scene.lights.push_back(light3);
}

// smiley
void *SetupScene3(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;
    scene.imgPlane = new Plane(Point3D(-0.5,0.5,0), Point3D(-0.5,-0.5,0), Point3D(0.5,-0.5,0), Point3D(0.5,0.5,0));
    Colour amb = Colour(0.4,0.0,0.4);
    Material *yellow   = new Material(amb, Colour(1,1,0), Colour(1,1,1), 100,  Colour(0.1,0.1,0.1));
    Material *black = new Material(amb, Colour(0,0,0), Colour(1,1,1), 10,  Colour(0.5,0.5,0.5));
    Material *white = new Material(amb, Colour(1,1,1), Colour(1,1,1), 10,  Colour(0.2,0.2,0.2));

    scene.objects.push_back(new Quad(Point3D(-100,100,2), Point3D(-100,-100,2), Point3D(100,-100,2), Point3D(100,100,2), yellow));

    scene.objects.push_back( new Sphere(Point3D(-3,4,     1), 2,white));
    scene.objects.push_back( new Sphere(Point3D(3,4,      1), 2, white));

    scene.objects.push_back( new Sphere(Point3D(-2.5,4,-.5),  1,black));
    scene.objects.push_back( new Sphere(Point3D(2.5, 4,-.5),  1,black));

    scene.objects.push_back( new Sphere(Point3D(-4,-1,1.5), 1, white));
    scene.objects.push_back( new Sphere(Point3D(-3,-2,1.5), 1, white));
    scene.objects.push_back( new Sphere(Point3D(-2,-3,1.5), 1, white));
    scene.objects.push_back( new Sphere(Point3D(-1,-3.15,1.5), 1, white));
    scene.objects.push_back( new Sphere(Point3D(0,-3.15, 1.5), 1, white));
    scene.objects.push_back( new Sphere(Point3D(1,-3.15, 1.5), 1, white));
    scene.objects.push_back( new Sphere(Point3D(4,-1, 1.5), 1, white));
    scene.objects.push_back( new Sphere(Point3D(3,-2, 1.5), 1, white));
    scene.objects.push_back( new Sphere(Point3D(2,-3, 1.5), 1, white));

    scene.lights.push_back(new Light(Point3D(1,5,-10), Colour(0.1,0.1,0.1),  Colour(0.4,0.3,0.3), Colour(1,1,1)));
    scene.lights.push_back(new Light(Point3D(0,6,-10), Colour(0.1,0.1,0.1),  Colour(0.3,0.4,0.3), Colour(1,1,1)));
    scene.lights.push_back(new Light(Point3D(-1,5,-10), Colour(0.1,0.1,0.1), Colour(0.3,0.3,0.4), Colour(1,1,1)));
    scene.lights.push_back(new Light(Point3D(-10,10,1.5), Colour(0,0,0), Colour(0.1,0.1,0.1), Colour(1,1,1)));

    scene.Transform(
                Matrix4x4::translation(Vector3D(0,0,17)) *
                Matrix4x4::rotation(2*M_PI/180, 'x') *
                Matrix4x4::rotation(3*M_PI/180, 'y') *
                Matrix4x4::rotation(5*M_PI/180, 'z')
                    );
}

int main(int argc, char *argv[])
{
    // currently unused parameters
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    Scene scene;
    scene.debug = false;

    // image width and height
    int width  = DEF_WIDTH;
    int height = DEF_HEIGHT;

    char const *outputStr = "output.png";
    if (argc >= 3)                  // prompt user on command line for dimensions
    {
        width  = atoi(argv[1]);
        height = atoi(argv[2]);
        outputStr = argv[3];
    }

    SetupScene3(&scene, width, height);

    // create new image
    QImage image(width, height, QImage::Format_RGB32);

    Colour * resultImg = scene.Render();

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Colour &c = resultImg[x + y * scene.cam.imgWidth];
            // clamp rgb values [0,1]
            uint intClr = qRgb(clamp(0,1,c.R()) * 255,
                               clamp(0,1,c.G()) * 255,
                               clamp(0,1,c.B()) * 255);
            // set pixel value
            image.setPixel(x, y, intClr);
        }
    }
    // save to file
    image.save(outputStr);

    // application successfully returned
    return 0;
}
