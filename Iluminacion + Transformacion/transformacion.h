#ifndef TRANSFORMACION_H
#define TRANSFORMACION_H

#include "vertice.h"
#include <iostream>
class Transformacion
{
public:
    util::Matriz4 matriz4D;

    Transformacion();
    void rotar(util::Vector3 eje,util::Vector3 punto, float grados);
    void trasladar(util::Vector3 destino);
    void mostrarMatriz();
    void identidad();
    util::Vector3 nuevaPosicion(util::Vector3 v);
};

#endif // TRANSFORMACION_H
