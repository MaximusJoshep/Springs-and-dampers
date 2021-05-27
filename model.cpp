#include "model.h"
#include "utils.cpp"

Model::Model()
{

}
//Set and Get Vertex of Model
void Model::setVertex(std::vector<Vertex> vertexList)
{
    this->vertex = vertexList;
}
std::vector<Vertex> Model::getVertex()
{
    return this->vertex;
}

void Model::setIndex(std::vector<int> indexList)
{
    this->index = indexList;
    for(std::size_t i=0 ; i<indexList.size() ; i++)
    {
        distances.emplace_back(vertex[indexList[i]-1].getPosition().dist(vertex[indexList[i]-1].getPosition()));
    }
}
const std::vector<int> Model::getIndex()
{
    return this->index;
}

void Model::updateModel()
{
    vertexToShow.clear();
    //Adding gravity force for every vertex
    Vector3 gravity(0,-0.98,0);
    for(std::size_t i=0 ; i<vertex.size() ; i++)
    {
        vertex[i].addForce(gravity);
    }
    float k = 100; //spring constant
    float d = 0.5; //damper constant
    float mass = 1;
    float h = 0.001;

    for(std::size_t i=0 ; i<this->index.size()-1 ; i++)
    {
        Vertex v1 = this->vertex[this->index[i]-1];
        Vertex v2 = this->vertex[this->index[i+1]-1];

        Vector3 r = v2.getPosition().sub(v1.getPosition());     //Distancia de resposo
        Vector3 vr = v2.getVelocity().sub(v1.getVelocity());    //Diferencia de velocidades
        float dl = r.norm() - distances[i];                   //Diferencia de longitud = (L-r)
        float f = k * dl;
        Vector3 llr =  r.div(r.norm());                         // L/L

        Vector3 F = r.mult(f).add(vr.multElement(llr).mult(d).multElement(llr));    //F = (r*f) + (d *(vr.*r)).*r;
        v1.addForce(F);
        v2.addForce(F.negate());

        this->vertex[this->index[i]-1] = v1;
        this->vertex[this->index[i+1]-1] = v2;
    }

    //Updating all vertex of the model
    for(std::size_t i=0 ; i<index.size() ; i++)
    {
        vertexToShow.emplace_back(vertex[index[i]-1].getPosX());
        vertexToShow.emplace_back(vertex[index[i]-1].getPosY());
        vertexToShow.emplace_back(vertex[index[i]-1].getPosZ());
    }
}

void Model::translateModel(float x,float y,float z)
{
    Vector3 position(x,y,z);
    for(std::size_t i=0 ; i<this->vertex.size() ; i++)
    {
        vertex[i].setPosition(vertex[i].getPosition().add(position));
    }
}
void Model::init(GLuint& vbo)
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertexToShow.size() * sizeof(float), &this->vertexToShow[0], GL_STATIC_DRAW);
}

void Model::show(GLuint& vbo)
{
    updateModel();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertexToShow.size()*sizeof(float), &this->vertexToShow[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, this->vertexToShow.size()/3);
}

