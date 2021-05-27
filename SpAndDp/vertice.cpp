#include "vertice.h"

Vertice::Vertice(float x, float y, float z)
{
    posicion = util::Vector3(x,y,z);
    masa = 1;
}
Vertice::Vertice()
{
    posicion = util::Vector3(0,0,0);
    masa = 1;
}
void Vertice::actualizar()
{
    float h = 0.001;
    velocidad = velocidad + aceleracion;
    posicion = posicion + (velocidad*h);
}
void Vertice::agregarFuerza(util::Vector3 fuerza)
{
    aceleracion = aceleracion+(fuerza/masa);
}
