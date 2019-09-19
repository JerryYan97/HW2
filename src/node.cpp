#include "node.h"

Node& Node::AddChild(uPtr<Node> childUPtr)
{
    this->addChild(childUPtr.get());
    this->childrenPtrVec.push_back(std::move(childUPtr));
    return *childrenPtrVec.back().get();
}

void Node::ModifyColor(glm::vec3 iColor)
{
    this->color = iColor;
    this->geoPtr->setColor(this->color);
}

Polygon2D* Node::GetGeoPtr()
{
    return this->geoPtr;
}

void Node::AddGeo(Polygon2D* iGeoPtr)
{
    geoPtr = iGeoPtr;
}

glm::vec3 Node::GetColor()
{
    return this->color;
}

QString Node::GetName()
{
    return nodeName;
}

unsigned Node::GetNodeType()
{
    return this->nodeType;
}

TranslateNode::TranslateNode(float ix, float iy, const QString& iNodeName) : Node(iNodeName)
{
    this->nodeType = 1;
    xTranslation = ix;
    yTranslation = iy;
}

glm::mat3 TranslateNode::ComputeTransformation()
{
    return glm::translate(glm::mat3(), glm::vec2(xTranslation, yTranslation));
}

float TranslateNode::GetXTranslation()
{
    return xTranslation;
}

float TranslateNode::GetYTranslation()
{
    return yTranslation;
}

void TranslateNode::SetXTranslation(float i)
{
    xTranslation = i;
}

void TranslateNode::SetYTranslation(float i)
{
    yTranslation = i;
}

RotateNode::RotateNode(float imagnitude, const QString& iNodeName) : Node(iNodeName)
{
    this->nodeType = 2;
    magnitude = imagnitude;
    geoPtr = nullptr;
}

glm::mat3 RotateNode::ComputeTransformation()
{
    return glm::rotate(glm::mat3(), glm::radians(magnitude));
}

float RotateNode::GetMagnitude()
{
    return magnitude;
}

void RotateNode::SetMagnitude(float i)
{
    magnitude = i;
}

ScaleNode::ScaleNode(float ixScalar, float iyScalar, const QString& iNodeName) : Node(iNodeName)
{
    this->nodeType = 3;
    xScalar = ixScalar;
    yScalar = iyScalar;
    geoPtr = nullptr;
}

glm::mat3 ScaleNode::ComputeTransformation()
{
    return glm::scale(glm::mat3(), glm::vec2(xScalar, yScalar));
}

float ScaleNode::GetXScalar()
{
    return xScalar;
}

float ScaleNode::GetYScalar()
{
    return yScalar;
}

void ScaleNode::SetXScalar(float i)
{
    xScalar = i;
}

void ScaleNode::SetYScalar(float i)
{
    yScalar = i;
}
