#ifndef VERTEX_H
#define VERTEX_H

#include "vector3.h"
class Vertex
{
    Vector3 position;
    Vector3 velocity;
public:
    Vertex(float x, float y, float z);
    Vertex();
    float getPosX();
    float getPosY();
    float getPosZ();
    Vector3& getPosition();
    void setPosition(Vector3 otherPosition);

    Vector3 getVelocity();
    void addForce(Vector3 force);
};

#endif // VERTEX_H
