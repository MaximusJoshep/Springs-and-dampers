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

    void iniciar(GLuint& vbo);
    void mostrar(GLuint& vbo);
    void springsAndDampers();

};

#endif // OBJFILE_H
