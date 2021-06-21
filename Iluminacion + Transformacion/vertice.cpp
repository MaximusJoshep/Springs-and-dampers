#include "vertice.h"

Vertice::Vertice(float x, float y, float z)
{
    posicion = util::Vector3(x,y,z);
    velocidad = util::Vector3(0,0,0);
    aceleracion = util::Vector3(0,0,0);
    masa = 10;
}
Vertice::Vertice()
{
    posicion = util::Vector3(0,0,0);
    velocidad = util::Vector3(0,0,0);
    masa = 10;
}
void Vertice::actualizar()
{
    float h = 0.0001;
    velocidad = velocidad + aceleracion;
    posicion = posicion + (velocidad*h);
}
void Vertice::agregarFuerza(util::Vector3 fuerza)
{
    aceleracion = aceleracion+(fuerza/masa);
}
