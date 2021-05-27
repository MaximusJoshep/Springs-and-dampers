#include "vector3.h"
#include <cmath>

Vector3::Vector3(float newX, float newY, float newZ)
{
    this->x = newX;
    this->y = newY;
    this->z = newZ;
}
Vector3::Vector3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}
Vector3::Vector3(Vector3*)
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}
Vector3 Vector3::add(const Vector3 &otherVector3)
{
    return Vector3(this->x+otherVector3.x , this->y+otherVector3.y , this->z+otherVector3.z);
}
Vector3 Vector3::sub(const Vector3 &otherVector3)
{
    return Vector3(this->x-otherVector3.x , this->y-otherVector3.y , this->z-otherVector3.z);
}
Vector3 Vector3::mult(float number)
{
    return Vector3(this->x*number , this->y*number , this->z*number);
}
Vector3 Vector3::div(float number)
{
    return this->mult(1/number);
}

Vector3 Vector3::normalize()
{
    int sq = sqrt(pow(this->x,2) + pow(this->y,2) + pow(this->z,2));
    return Vector3(this->x/sq , this->y/sq , this->z/sq);

}
void Vector3::setX(float newX){this->x = newX;}
void Vector3::setY(float newY){this->y = newY;}
void Vector3::setZ(float newZ){this->z = newZ;}
float Vector3::getX(){return this->x;}
float Vector3::getY(){return this->y;}
float Vector3::getZ(){return this->z;}
float Vector3::dist(const Vector3 otherVector3)
{
    return sqrt(pow(this->x-otherVector3.x,2)+pow(y-otherVector3.y,2));
}
float Vector3::norm()
{
    return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}
Vector3 Vector3::multElement(const Vector3 otherVector3)
{
    return Vector3(x*otherVector3.x , y*otherVector3.y , z*otherVector3.z);
}

Vector3 Vector3::negate()
{
    return Vector3(-x,-y,-z);
}
