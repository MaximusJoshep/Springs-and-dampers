#include "model.h"

Model::Model()
{

}
void Model::setVertex(std::vector<float> vertexList)
{
    this->vertex.clear();
    for(std::size_t i=0 ; i<vertexList.size() ; i++)
    {
        this->vertex.emplace_back(vertexList[i]);
    }
}
const std::vector<float> Model::getVertex()
{
    return this->vertex;
}
void Model::setPosition(std::vector<float> vector)
{
    this->position = vector;
}
void Model::setVelocity(std::vector<float> vector)
{
    if(vector.size() != 3)
        return;
    this->velocity = vector;
}
void Model::addForce(std::vector<float> vector)
{
    if(vector.size() != 3)
        return;
    this->aceleration[0]+=vector[0];
    this->aceleration[1]+=vector[1];
    this->aceleration[2]+=vector[2];
}
std::vector<float> Model::getPosition()
{
    return this->position;
}
std::vector<float> Model::getVelocity()
{
    return this->velocity;
}
void Model::init(GLuint& vbo)
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertex.size() * sizeof(float), &this->vertex[0], GL_STATIC_DRAW);
}

void Model::show(GLuint& vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertex.size()*sizeof(float), &this->vertex[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, this->vertex.size()/3);}
