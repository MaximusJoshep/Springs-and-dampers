#ifndef VERTICE_H
#define VERTICE_H

#include "util.h"
class Vertice
{
public:
    util::Vector3 posicion;
    util::Vector3 velocidad;
    util::Vector3 aceleracion;
    float masa;

    Vertice(float x, float y, float z);
    Vertice();

    void actualizar();
    void agregarFuerza(util::Vector3 fuerza);
};

#endif // VERTICE_H
