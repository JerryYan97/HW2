#pragma once
#include "drawable.h"
#include <iostream>
class Polygon2D : public Drawable
{
public:
    // Instantiate an empty Polygon
    Polygon2D(OpenGLContext* context);
    // Instantiate a regular polygon with N sides and
    // a bounding box of side length 1 centered at the origin
    Polygon2D(OpenGLContext* context, int numSides);
    // Instantiate a polygon with its vertex positions defined
    // in counter-clockwise order. These vertices must form a convex
    // polygon in order to be drawn correctly.
    Polygon2D(OpenGLContext* context, const std::vector<glm::vec3>& positions);
    // Initialize data required by OpenGL to render the shape
    void create() override;
    // Set the color of the polygon when it's drawn by OpenGL
    void setColor(glm::vec3 c);

    void show(){
        for (std::vector<glm::vec3>::iterator p = m_vertPos.begin(); p != m_vertPos.end(); p++) {
            std::cout << p->x << " " << p->y << " " << p->z << std::endl;
        }
    }

protected:
    // The list of vertex positions that define this polygon's shape
    std::vector<glm::vec3> m_vertPos;
    // The order in which vertices should be read to assemble triangles
    // that, all together, form the polygon.
    std::vector<GLuint> m_vertIdx;
    // How many vertices compose this Polygon. Read by setColor
    // in order to know how many vertices need to be assigned the
    // Polygon's color.
    unsigned int m_numVertices;
};

class RectangleGeometry : public Polygon2D
{
public:
    RectangleGeometry(OpenGLContext* context);
    ~RectangleGeometry();
    void SetRectangle(std::array<glm::vec3, 4>& iVertices);
private:
    std::array<glm::vec3, 4> vertices; // leftUpper, rightUpper, leftBottom, rightBottom;
};

class CircleGeometry : public Polygon2D
{
public:
    CircleGeometry(OpenGLContext* context);
    ~CircleGeometry();
private:
};

class TrapezoidGeometry : public Polygon2D
{
public:
    TrapezoidGeometry(OpenGLContext* context);
    ~TrapezoidGeometry();
private:
};

class ShoeGeometry : public Polygon2D
{
public:
    ShoeGeometry(OpenGLContext* context);
    ~ShoeGeometry();
};
