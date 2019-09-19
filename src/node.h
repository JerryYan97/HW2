#ifndef NODE_H
#define NODE_H
#include "smartpointerhelp.h"
#include "drawable.h"
#include "scene/polygon.h"
#include <QTreeWidgetItem>
#include <vector>

class Node : public QTreeWidgetItem
{
public:
    std::vector<uPtr<Node>> childrenPtrVec;

    Node(const QString& iNodeName){
        this->setText(0, iNodeName);
        nodeName = iNodeName;
    }
    virtual ~Node(){}
    virtual glm::mat3 ComputeTransformation() = 0;
    Node& AddChild(uPtr<Node> childUPtr);
    void AddGeo(Polygon2D* iGeoPtr);

    void ModifyColor(glm::vec3 iColor);

    Polygon2D* GetGeoPtr();
    glm::vec3 GetColor();
    QString GetName();
    unsigned GetNodeType();
protected:
    Polygon2D* geoPtr = nullptr;
    glm::vec3 color;
    QString nodeName;
    unsigned int nodeType;// 0 - Node, 1 - TranslateNode, 2 - RotateNode, 3 - ScaleNode;
    unsigned int col = 0;
};

class TranslateNode : public Node
{
public:
    TranslateNode(float ix, float iy, const QString& iNodeName);
    ~TranslateNode(){}

    glm::mat3 ComputeTransformation();

    float GetXTranslation();
    float GetYTranslation();

    void SetXTranslation(float i);
    void SetYTranslation(float i);
private:
    float xTranslation;
    float yTranslation;
};

class RotateNode : public Node
{
public:
    RotateNode(float imagnitude, const QString& iNodeName);
    ~RotateNode(){}

    float GetMagnitude();

    void SetMagnitude(float i);

    glm::mat3 ComputeTransformation();

private:
    float magnitude;// Magnitude is at the unit of radian.
};

class ScaleNode : public Node
{
public:
    ScaleNode(float ixScalar, float iyScalar, const QString& iNodeName);
    ~ScaleNode(){}

    float GetXScalar();
    float GetYScalar();

    void SetXScalar(float i);
    void SetYScalar(float i);

    glm::mat3 ComputeTransformation();
private:
    float xScalar;
    float yScalar;
};


#endif // NODE_H
