#include "vertex.h"

Vertex::Vertex(float x, float y, float z)
{
    this->position = Vector3(x,y,z);
    this->velocity = Vector3(0,0,0);
}

Vertex::Vertex()
{
    this->position = Vector3(0,0,0);
    this->velocity = Vector3(0,0,0);
}

float Vertex::getPosX(){return this->position.getX();}
float Vertex::getPosY(){return this->position.getY();}
float Vertex::getPosZ(){return this->position.getZ();}


void Vertex::addForce(Vector3 force)
{
    float h = 0.0001;
    float mass = 1;
    velocity = velocity.add(force.div(mass));       //v = a/m
    position = position.add(velocity.mult(h));      //p = p+v
}

Vector3& Vertex::getPosition()
{
    return this->position;
}
void Vertex::setPosition(Vector3 otherPosition)
{
    this->position = otherPosition;
}
Vector3 Vertex::getVelocity()
{
    return this->velocity;
}
