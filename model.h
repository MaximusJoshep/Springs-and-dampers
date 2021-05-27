#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <GL/glew.h>
#include <iostream>
#include "vector3.h"
#include "vertex.h"

class Model
{
    std::vector<Vertex> vertex;
    std::vector<float> vertexToShow;
    std::vector<int> index;
    std::vector<float> distances;
public:

    Model();

    //Set and Get Vertex of model
    void setVertex(std::vector<Vertex> vertexList);
    std::vector<Vertex> getVertex();
    //Set and get Index of model
    void setIndex(std::vector<int> indexList);
    const std::vector<int> getIndex();

    //Function to build model with array of floats
    void updateModel();

    //Translate
    void translateModel(float x,float y,float z);

    //OpenGL init and show
    void init(GLuint& vbo);
    void show(GLuint& vbo);
};

#endif // OBJECT_H
