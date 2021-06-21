#ifndef OBJFILE_H
#define OBJFILE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "vertice.h"
#include "transformacion.h"
#include <GL/glew.h>


class ArchivoObj
{
public:

    std::vector<float> vertices;
    std::vector<Vertice> todosVertices;
    std::vector<std::vector<int>> aristas;
    std::vector<float> coordenadasTextura;
    std::vector<float> normales;
    std::vector<int> indices;
    std::vector<float> distancias;
    bool estatico;

    ArchivoObj(std::string direccion);
    void iniciar(GLuint& vbo);
    void mostrar(GLuint& vbo, GLuint& vbn);
    void springsAndDampers();
    void transformar(Transformacion t);
};

#endif // OBJFILE_H
