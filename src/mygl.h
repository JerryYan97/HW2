#pragma once

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/grid.h>
#include <scene/polygon.h>
#include "drawable.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "node.h"


class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    Grid m_geomGrid;        // The instance of the object used to render the 5x5 grid
    Polygon2D m_geomSquare; // The singular instance of our square object that can be re-drawn with different colors
                            // to create the appearance of there being multiple squares

    bool m_showGrid; // Read in paintGL to determine whether or not to draw the grid.

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.

    uPtr<Node> RootNode;

    uPtr<RectangleGeometry> geoRectangle;
    uPtr<CircleGeometry> geoCircle;
    uPtr<TrapezoidGeometry> geoTrapezoid;
    uPtr<ShoeGeometry> geoShoe;


public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    Node& GetRoot();
    RectangleGeometry* GetRectangle();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void TraversalDraw(uPtr<Node>& iNodePtr, glm::mat3 iTransformationMat);
    Node* TraversalFind(QTreeWidgetItem* target);
    uPtr<Node> ConstructSceneGraph();

public slots:

signals:
    void SendNode();
protected:
    void keyPressEvent(QKeyEvent *e);
};


