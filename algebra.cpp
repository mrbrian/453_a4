//---------------------------------------------------------------------------
//
// CPSC453 -- Introduction to Computer Graphics
// Assignment 2
//
// Classes and functions for manipulating points, vectors, matrices, 
// and colours.  You probably won't need to modify anything in these
// two files.
//
// Adapted from CS488 A2 University of Waterloo Computer Graphics Lab / 2003
//
//---------------------------------------------------------------------------

#include "algebra.h"

double Vector3D::normalize()
{
  double denom = 1.0;
  double x = (v_[0] > 0.0) ? v_[0] : -v_[0];
  double y = (v_[1] > 0.0) ? v_[1] : -v_[1];
  double z = (v_[2] > 0.0) ? v_[2] : -v_[2];

  if(x > y) {
    if(x > z) {
      if(1.0 + x > 1.0) {
        y = y / x;
        z = z / x;
        denom = 1.0 / (x * sqrt(1.0 + y*y + z*z));
      }
    } else { /* z > x > y */ 
      if(1.0 + z > 1.0) {
        y = y / z;
        x = x / z;
        denom = 1.0 / (z * sqrt(1.0 + y*y + x*x));
      }
    }
  } else {
    if(y > z) {
      if(1.0 + y > 1.0) {
        z = z / y;
        x = x / y;
        denom = 1.0 / (y * sqrt(1.0 + z*z + x*x));
      }
    } else { /* x < y < z */
      if(1.0 + z > 1.0) {
        y = y / z;
        x = x / z;
        denom = 1.0 / (z * sqrt(1.0 + y*y + x*x));
      }
    }
  }

  if(1.0 + x + y + z > 1.0) {
    v_[0] *= denom;
    v_[1] *= denom;
    v_[2] *= denom;
    return 1.0 / denom;
  }

  return 0.0;
}

/*
 * Define some helper functions for matrix inversion.
 */

static void swaprows(Matrix4x4& a, size_t r1, size_t r2)
{
  std::swap(a[r1][0], a[r2][0]);
  std::swap(a[r1][1], a[r2][1]);
  std::swap(a[r1][2], a[r2][2]);
  std::swap(a[r1][3], a[r2][3]);
}

static void dividerow(Matrix4x4& a, size_t r, double fac)
{
  a[r][0] /= fac;
  a[r][1] /= fac;
  a[r][2] /= fac;
  a[r][3] /= fac;
}

static void submultrow(Matrix4x4& a, size_t dest, size_t src, double fac)
{
  a[dest][0] -= fac * a[src][0];
  a[dest][1] -= fac * a[src][1];
  a[dest][2] -= fac * a[src][2];
  a[dest][3] -= fac * a[src][3];
}

/*
 * invertMatrix
 *
 * I lifted this code from the skeleton code of a raytracer assignment
 * from a different school.  I taught that course too, so I figured it
 * would be okay.
 */
Matrix4x4 Matrix4x4::invert() const
{
  /* The algorithm is plain old Gauss-Jordan elimination 
     with partial pivoting. */

  Matrix4x4 a(*this);
  Matrix4x4 ret;

  /* Loop over cols of a from left to right, 
     eliminating above and below diag */

  /* Find largest pivot in column j among rows j..3 */
  for(size_t j = 0; j < 4; ++j) { 
    size_t i1 = j; /* Row with largest pivot candidate */
    for(size_t i = j + 1; i < 4; ++i) {
      if(fabs(a[i][j]) > fabs(a[i1][j])) {
        i1 = i;
      }
    }

    /* Swap rows i1 and j in a and ret to put pivot on diagonal */
    swaprows(a, i1, j);
    swaprows(ret, i1, j);

    /* Scale row j to have a unit diagonal */
    if(a[j][j] == 0.0) {
      // Theoretically throw an exception.
      return ret;
    }

    dividerow(ret, j, a[j][j]);
    dividerow(a, j, a[j][j]);

    /* Eliminate off-diagonal elems in col j of a, doing identical 
       ops to b */
    for(size_t i = 0; i < 4; ++i) {
      if(i != j) {
        submultrow(ret, i, j, a[i][j]);
        submultrow(a, i, j, a[i][j]);
      }
    }
  }

  return ret;
}

Line3D::Line3D() { }

Line3D::Line3D(const Point3D &p1, Point3D &p2) : p1_(p1), p2_(p2) { }

Line3D::Line3D(const Line3D &other) : p1_(other.p1_), p2_(other.p2_) { }

Line3D::~Line3D() { }

Point3D &Line3D::getP1()
{
    return p1_;
}

Point3D &Line3D::getP2()
{
    return p2_;
}

Shape::Shape(ShapeType sType)
{
    switch(sType)                           // populate vertex list
    {
    case CYLINDER:
        {
            float seg = 10;
            for (int i = 0; i < seg; i++)
            {
                float rad = 2.0 * M_PI / seg * i;
                float x1 = cos(rad);
                float z1 = sin(rad);
                verts_.push_back(Point3D(x1,-1,z1));
                verts_.push_back(Point3D(x1,1, z1));
            }

            for (int i = 0; i <= seg; i++)
            {
                float rad1 = 2.0 * M_PI / seg * i;
                float x1 = cos(rad1);
                float z1 = sin(rad1);

                float rad2 = 2.0 * M_PI / seg * (i + 1);
                float x2 = cos(rad2);
                float z2 = sin(rad2);

                verts_.push_back(Point3D(x1,1,z1));
                verts_.push_back(Point3D(x2,1,z2));

                verts_.push_back(Point3D(x1,-1,z1));
                verts_.push_back(Point3D(x2,-1,z2));
            }
        }
        break;
    case PYRAMID:
        verts_.push_back(Point3D(-1,-1,-1));    // populate vertex list
        verts_.push_back(Point3D(-1,-1,1));
        verts_.push_back(Point3D(-1,-1,1));
        verts_.push_back(Point3D(1,-1,1));
        verts_.push_back(Point3D(1,-1,1));
        verts_.push_back(Point3D(1,-1,-1));
        verts_.push_back(Point3D(1,-1,-1));
        verts_.push_back(Point3D(-1,-1,-1));

        verts_.push_back(Point3D(0,1,0));
        verts_.push_back(Point3D(-1,-1,-1));
        verts_.push_back(Point3D(0,1,0));
        verts_.push_back(Point3D(-1,-1,1));
        verts_.push_back(Point3D(0,1,0));
        verts_.push_back(Point3D(1,-1,1));
        verts_.push_back(Point3D(0,1,0));
        verts_.push_back(Point3D(1,-1,-1));
        break;
    case CUBE:
        verts_.push_back(Point3D(-1,1,-1));     // populate vertex list
        verts_.push_back(Point3D(-1,1,1));
        verts_.push_back(Point3D(-1,1,1));
        verts_.push_back(Point3D(1,1,1));
        verts_.push_back(Point3D(1,1,1));
        verts_.push_back(Point3D(1,1,-1));
        verts_.push_back(Point3D(1,1,-1));
        verts_.push_back(Point3D(-1,1,-1));

        verts_.push_back(Point3D(-1,-1,-1));
        verts_.push_back(Point3D(-1,-1,1));
        verts_.push_back(Point3D(-1,-1,1));
        verts_.push_back(Point3D(1,-1,1));
        verts_.push_back(Point3D(1,-1,1));
        verts_.push_back(Point3D(1,-1,-1));
        verts_.push_back(Point3D(1,-1,-1));
        verts_.push_back(Point3D(-1,-1,-1));

        verts_.push_back(Point3D(-1,1,-1));
        verts_.push_back(Point3D(-1,-1,-1));
        verts_.push_back(Point3D(-1,1,1));
        verts_.push_back(Point3D(-1,-1,1));
        verts_.push_back(Point3D(1,1,1));
        verts_.push_back(Point3D(1,-1,1));
        verts_.push_back(Point3D(1,1,-1));
        verts_.push_back(Point3D(1,-1,-1));
        break;
    }
}

Shape::~Shape() { }

Matrix4x4 Shape::getTransform() const
{
    return this->transform_ * scale_;
}

Matrix4x4 Shape::getGnomonTransform() const
{
    return this->transform_;
}

void Shape::resetTransform()
{
    this->transform_ = Matrix4x4();
    this->scale_ = Matrix4x4();
}

void Shape::appendTransform(const Matrix4x4 &xform)
{
    this->transform_ = this->transform_ * xform;
}

void Shape::scale(const Matrix4x4 &xform)
{
    this->scale_ = this->scale_ * xform;
}

std::vector<Line3D> Shape::getLines()
{
    std::vector<Line3D> lines;

    for(std::vector<Point3D>::iterator it = verts_.begin(); it != verts_.end(); ++it)
    {
        Point3D p1 = *(it++), p2 = *it;
        lines.push_back(Line3D(p1,p2));
    }
    return lines;
}
