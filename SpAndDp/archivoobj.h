#ifndef OBJFILE_H
#define OBJFILE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "vertice.h"

#include <GL/glew.h>


class ArchivoObj
{

public:
    ArchivoObj(std::string direccion);
    std::vector<float> vertices;
    std::vector<Vertice> todosVertices;
    std::vector<std::vector<int>> aristas;
    std::vector<float> coordenadasTextura;
    std::vector<float> normales;
    std::vector<int> indices;
    std::vector<float> distancias;

    void iniciar(GLuint& vbo);
    void mostrar(GLuint& vbo);
    void springsAndDampers();
    void trasladar(float x, float y, float z);

};

#endif // OBJFILE_H
