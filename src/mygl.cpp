#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      prog_flat(this),
      m_geomGrid(this), m_geomSquare(this, {glm::vec3(0.5f, 0.5f, 1.f),
                                            glm::vec3(-0.5f, 0.5f, 1.f),
                                            glm::vec3(-0.5f, -0.5f, 1.f),
                                            glm::vec3(0.5f, -0.5f, 1.f)}),
      m_showGrid(true)
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();

    glDeleteVertexArrays(1, &vao);
    m_geomSquare.destroy();
    m_geomGrid.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    // Create the scene geometry
    m_geomGrid.create();
    m_geomSquare.create();

    geoRectangle = std::make_unique<RectangleGeometry>(this);
    geoRectangle->create();

    geoCircle = std::make_unique<CircleGeometry>(this);
    geoCircle->create();

    geoTrapezoid = std::make_unique<TrapezoidGeometry>(this);
    geoTrapezoid->create();

    geoShoe = std::make_unique<ShoeGeometry>(this);
    geoShoe->create();

    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    // Initialize the whole scene graph;
    RootNode = ConstructSceneGraph();
    emit SendNode();
}

void MyGL::resizeGL(int w, int h)
{
    glm::mat3 viewMat = glm::scale(glm::mat3(), glm::vec2(0.2, 0.2)); // Screen is -5 to 5

    // Upload the view matrix to our shader (i.e. onto the graphics card)
    prog_flat.setViewMatrix(viewMat);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_showGrid)
    {
        prog_flat.setModelMatrix(glm::mat3());
        prog_flat.draw(*this, m_geomGrid);
    }

    // Here is a good spot to call your scene graph traversal function.
    // Any time you want to draw an instance of geometry, call
    // prog_flat.draw(*this, yourNonPointerGeometry);

    TraversalDraw(RootNode, glm::mat3());
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    switch(e->key())
    {
    case(Qt::Key_Escape):
        QApplication::quit();
        break;

    case(Qt::Key_G):
        m_showGrid = !m_showGrid;
        break;
    }
}

uPtr<Node> MyGL::ConstructSceneGraph()
{
    uPtr<Node> root = std::make_unique<TranslateNode>(0.f, 0.f, QString("Root"));

    // For Upper Body.
    Node& upperBodyTRef = root->AddChild(std::make_unique<TranslateNode>(0.f, 2.f, QString("upper body T")));
    Node& upperBodyRRef = upperBodyTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("upper body R")));
    Node& upperBodySRef = upperBodyRRef.AddChild(std::make_unique<ScaleNode>(1.f, 1.f, QString("upper body S")));


    // For Right Leg.
    Node& rightLegTRef = upperBodySRef.AddChild(std::make_unique<TranslateNode>(0.5f, -2.f, QString("right leg T")));
    Node& rightLegRRef = rightLegTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("right leg R")));
    Node& rightLegSRef = rightLegRRef.AddChild(std::make_unique<ScaleNode>(1.f, 1.f, QString("right leg S")));

    Node& rightShoeTRef = rightLegSRef.AddChild(std::make_unique<TranslateNode>(0.55f, -3.75f, QString("right shoe T")));
    Node& rightForeLegTRef = rightLegSRef.AddChild(std::make_unique<TranslateNode>(0.3f, 0.25f, QString("right fore leg T")));
    Node& rightBackLegTRef = rightLegSRef.AddChild(std::make_unique<TranslateNode>(0.3f, -1.5f, QString("right back leg T")));


    // For Right Back Leg.
    Node& rightBackLegRRef = rightBackLegTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("right back leg R")));
    Node& rightBackLegTInnerRef = rightBackLegRRef.AddChild(std::make_unique<TranslateNode>(-0.3f, -1.f, QString("right fore leg inner T")));
    Node& rightBackLegSRef = rightBackLegTInnerRef.AddChild(std::make_unique<ScaleNode>(0.8f, 2.5f, QString("right back leg S")));
    rightBackLegSRef.AddGeo(geoRectangle.get());
    rightBackLegSRef.ModifyColor(glm::vec3(1.f, 0.5f, 0.f));

    // For Right Fore Leg.
    Node& rightForeLegRRef = rightForeLegTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("right fore leg R")));
    Node& rightForeLegTInnerRef = rightForeLegRRef.AddChild(std::make_unique<TranslateNode>(-0.3f, -1.f, QString("right fore leg inner T")));
    Node& rightForeLegSRef = rightForeLegTInnerRef.AddChild(std::make_unique<ScaleNode>(0.8f, 2.5f, QString("right fore leg S")));
    rightForeLegSRef.AddGeo(geoRectangle.get());
    rightForeLegSRef.ModifyColor(glm::vec3(0.5f, 0.2f, 0.8f));

    // For Right Shoe.
    Node& rightShoeRRef = rightShoeTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("right shoe R")));
    Node& rightShoeTInnerRef = rightShoeRRef.AddChild(std::make_unique<TranslateNode>(-0.3f, -0.3f, QString("left shoe inner T")));
    Node& rightShoeSRef = rightShoeTInnerRef.AddChild(std::make_unique<ScaleNode>(0.6f, 0.6f, QString("right shoe S")));
    rightShoeSRef.AddGeo(geoShoe.get());
    rightShoeSRef.ModifyColor(glm::vec3(0.0f, 0.0f, 0.0f));


    // For left leg.
    Node& leftLegTRef = upperBodySRef.AddChild(std::make_unique<TranslateNode>(-0.5f, -2.f, QString("left leg T")));
    Node& leftLegRRef = leftLegTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("left leg R")));
    Node& leftLegSRef = leftLegRRef.AddChild(std::make_unique<ScaleNode>(1.f, 1.f, QString("left back leg S")));

    // For Left Fore Leg.
    Node& leftForeLegTRef = leftLegSRef.AddChild(std::make_unique<TranslateNode>(0.f, 0.25f, QString("left fore leg T")));
    Node& leftForeLegRRef = leftForeLegTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("left fore leg R")));
    Node& leftForeLegTInnerRef = leftForeLegRRef.AddChild(std::make_unique<TranslateNode>(0.f, -1.f, QString("left fore leg inner T")));
    Node& leftForeLegSRef = leftForeLegTInnerRef.AddChild(std::make_unique<ScaleNode>(0.8f, 2.5f, QString("left fore leg S")));
    leftForeLegSRef.AddGeo(geoRectangle.get());
    leftForeLegSRef.ModifyColor(glm::vec3(0.5f, 0.2f, 0.8f));

    // For left Shoe.
    Node& leftShoeTRef = leftLegSRef.AddChild(std::make_unique<TranslateNode>(-0.05f, -3.75f, QString("left shoe T")));
    Node& leftShoeRRef = leftShoeTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("left shoe R")));
    Node& leftShoeTInnerRef = leftShoeRRef.AddChild(std::make_unique<TranslateNode>(-0.3f, -0.3f, QString("left shoe inner T")));
    Node& leftShoeSRef = leftShoeTInnerRef.AddChild(std::make_unique<ScaleNode>(-0.6f, 0.6f, QString("right shoe S")));
    leftShoeSRef.AddGeo(geoShoe.get());
    leftShoeSRef.ModifyColor(glm::vec3(0.0f, 0.0f, 0.0f));

    // For Left Back leg.
    Node& leftBackLegTRef = leftLegSRef.AddChild(std::make_unique<TranslateNode>(0.f, -1.5f, QString("left back leg T")));
    Node& leftBackLegRRef = leftBackLegTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("left back leg R")));
    Node& leftBackLegTInnerRef = leftBackLegRRef.AddChild(std::make_unique<TranslateNode>(0.f, -1.f, QString("left back leg inner T")));
    Node& leftBackLegSRef = leftBackLegTInnerRef.AddChild(std::make_unique<ScaleNode>(0.8f, 2.5f, QString("left back leg S")));
    leftBackLegSRef.AddGeo(geoRectangle.get());
    leftBackLegSRef.ModifyColor(glm::vec3(1.f, 0.5f, 0.f));


    // For Right Arm.
    Node& rightArmTRef = upperBodySRef.AddChild(std::make_unique<TranslateNode>(0.5f, 0.5f, QString("right arm T")));
    Node& rightArmRRef = rightArmTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("right arm R")));
    Node& rightArmSRef = rightArmRRef.AddChild(std::make_unique<ScaleNode>(1.f, 1.f, QString("right arm S")));

    // For Right Fore Arm (RFA).
    Node& RFATRef = rightArmSRef.AddChild(std::make_unique<TranslateNode>(1.5f, 0.f, QString("right fore arm T")));
    Node& RFARRef = RFATRef.AddChild(std::make_unique<RotateNode>(0.f, QString("right fore arm R")));
    Node& rightForeArmTInnerRef = RFARRef.AddChild(std::make_unique<TranslateNode>(0.f, -0.5f, QString("right fore arm inner T")));
    Node& RFASRef = rightForeArmTInnerRef.AddChild(std::make_unique<ScaleNode>(0.5f, 2.0f, QString("right fore arm S")));
    RFASRef.AddGeo(geoRectangle.get());
    RFASRef.ModifyColor(glm::vec3(1.f, 0.5f, 0.f));

    // For Right Back Arm(RBA).
    Node& RBATRef = rightArmSRef.AddChild(std::make_unique<TranslateNode>(0.25f, 0.25f, QString("Back right Arm T")));
    Node& RBARRef = RBATRef.AddChild(std::make_unique<RotateNode>(0.f, QString("Back right Arm R")));
    Node& rightBackArmTInnerRef = RBARRef.AddChild(std::make_unique<TranslateNode>(0.5f, 0.f, QString("right fore arm inner T")));
    Node& RBASRef = rightBackArmTInnerRef.AddChild(std::make_unique<ScaleNode>(2.f, 0.5f, QString("Back right Arm S")));
    RBASRef.AddGeo(geoRectangle.get());
    RBASRef.ModifyColor(glm::vec3(0.f, 1.f, 0.f));


    // For Left Arm
    Node& leftArmTRef = upperBodySRef.AddChild(std::make_unique<TranslateNode>(-1.f, 0.5f, QString("left arm T")));
    Node& leftArmRRef = leftArmTRef.AddChild(std::make_unique<RotateNode>(0.f, QString("left arm R")));
    Node& leftArmSRef = leftArmRRef.AddChild(std::make_unique<ScaleNode>(1.f, 1.f, QString("left arm S")));

    // For Left Fore Arm (LFA).
    Node& FLATRef = leftArmSRef.AddChild(std::make_unique<TranslateNode>(-1.f, 0.f, QString("Fore Left Arm T")));
    Node& FLARRef = FLATRef.AddChild(std::make_unique<RotateNode>(0.f, QString("Fore Left Arm R")));
    Node& leftForeArmTInnerRef = FLARRef.AddChild(std::make_unique<TranslateNode>(0.0f, -0.5f, QString("left fore arm inner T")));
    Node& FLASRef = leftForeArmTInnerRef.AddChild(std::make_unique<ScaleNode>(0.5f, 2.0f, QString("Fore Left Arm S")));
    FLASRef.AddGeo(geoRectangle.get());
    FLASRef.ModifyColor(glm::vec3(1.f, 0.5f, 0.f));

    // For Left Back Arm (LBA).
    Node& BLATRef = leftArmSRef.AddChild(std::make_unique<TranslateNode>(-0.25f, 0.25f, QString("Back Left Arm T")));
    Node& BLARRef = BLATRef.AddChild(std::make_unique<RotateNode>(0.f, QString("Back Left Arm R")));
    Node& BLASRef = BLARRef.AddChild(std::make_unique<ScaleNode>(2.f, 0.5f, QString("Back Left Arm S")));
    BLASRef.AddGeo(geoRectangle.get());
    BLASRef.ModifyColor(glm::vec3(0.f, 1.f, 0.f));


    // For Head.
    Node& headTRef = upperBodySRef.AddChild(std::make_unique<TranslateNode>(0.f, 1.5f, QString("head T")));
    Node& headSRef = headTRef.AddChild(std::make_unique<ScaleNode>(1.f, 1.f, QString("head S")));
    headSRef.AddGeo(geoCircle.get());
    headSRef.ModifyColor(glm::vec3(0.f, 1.f, 0.f));

    // For downward body.
    Node& downwardBodyTRef = upperBodySRef.AddChild(std::make_unique<TranslateNode>(0.f, -1.5f, QString("downward Body T")));
    Node& downwardBodyRRef = downwardBodyTRef.AddChild(std::make_unique<RotateNode>(0.f, "downward Body R"));
    Node& downwardBodySRef = downwardBodyRRef.AddChild(std::make_unique<ScaleNode>(1.f, 1.f, "downward Body S"));
    downwardBodySRef.AddGeo(geoTrapezoid.get());
    downwardBodySRef.ModifyColor(glm::vec3(1.f, 1.f, 0.f));

    // For upper Body.
    Node& upperBodyRRRef = upperBodySRef.AddChild(std::make_unique<RotateNode>(180.f, "upper Body R"));
    Node& upperBodySSRef = upperBodyRRRef.AddChild(std::make_unique<ScaleNode>(1.f, 2.f, "upper Body S"));
    upperBodySSRef.AddGeo(geoTrapezoid.get());
    upperBodySSRef.ModifyColor(glm::vec3(1.f, 1.f, 0.f));

    return root;
}

void MyGL::TraversalDraw(uPtr<Node>& iNodePtr, glm::mat3 iTransformationMat)
{
    glm::mat3 T =  iTransformationMat * iNodePtr->ComputeTransformation();
    for (std::vector<uPtr<Node>>::iterator p = iNodePtr->childrenPtrVec.begin(); p != iNodePtr->childrenPtrVec.end(); p++)
    {
        TraversalDraw(*p, T);
    }

    if (iNodePtr->GetGeoPtr() != nullptr)
    {
        prog_flat.setModelMatrix(T);
        iNodePtr->GetGeoPtr()->setColor(iNodePtr->GetColor());
        prog_flat.draw(*this, *iNodePtr->GetGeoPtr());
    }
}

Node* TraversalSupport(Node* currentPtr, QTreeWidgetItem* target)
{
    if (currentPtr == target)
    {
        //std::cout << currentPtr->GetName().toStdString() << std::endl;
        return currentPtr;
    }

    for (std::vector<uPtr<Node>>::iterator p = currentPtr->childrenPtrVec.begin(); p != currentPtr->childrenPtrVec.end(); p++)
    {
        Node* result = TraversalSupport(p->get(), target);
        if (result != nullptr)
        {
            return result;
        }
    }

    return nullptr;
}

Node* MyGL::TraversalFind(QTreeWidgetItem* target)
{
    return TraversalSupport(this->RootNode.get(), target);
}

Node& MyGL::GetRoot()
{
    return *RootNode.get();
}

RectangleGeometry* MyGL::GetRectangle()
{
    return geoRectangle.get();
}

