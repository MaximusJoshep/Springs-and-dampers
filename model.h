#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <GL/glew.h>

class Model
{
    std::vector<float> position = {0,0,0};
    std::vector<float> velocity = {0,0,0};
    std::vector<float> aceleration = {0,0,0};

    std::vector<float> vertex;
public:

    Model();

    void setVertex(std::vector<float> vertexList);
    const std::vector<float> getVertex();
    void init(GLuint& vbo);
    void show(GLuint& vbo);
    void setPosition(std::vector<float> vector);
    void setVelocity(std::vector<float> vector);
    void addForce(std::vector<float> vector);
    std::vector<float> getPosition();
    std::vector<float> getVelocity();
};

#endif // OBJECT_H
