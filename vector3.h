#ifndef VECTOR3_H
#define VECTOR3_H


class Vector3
{
    float x;
    float y;
    float z;
public:
    Vector3(float newX, float newY, float newZ);
    Vector3();
    Vector3(Vector3*);
    Vector3 add(const Vector3 &otherVector3);
    Vector3 sub(const Vector3 &otherVector3);
    Vector3 mult(float number);
    Vector3 div(float number);

    Vector3 normalize();
    void setX(float newX);
    void setY(float newY);
    void setZ(float newZ);
    float getX();
    float getY();
    float getZ();
    float norm();
    float dist(const Vector3 otherVector3);
    Vector3 multElement(const Vector3 otherVector3);
    Vector3 negate();
};

#endif // VECTOR3_H
