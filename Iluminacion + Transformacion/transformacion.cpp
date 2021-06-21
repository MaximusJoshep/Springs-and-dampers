#include "transformacion.h"


Transformacion::Transformacion()
{

}
void Transformacion::rotar(util::Vector3 eje, util::Vector3 punto , float grados)
{
    util::Vector3 w = eje.normalizar();
    //Rodriguez method
    std::vector<std::vector<float>> matrixJFloat= {
        { 0    ,-w.z  , w.y  , 0 },
        { w.z  , 0    ,-w.x  , 0 },
        {-w.y  , w.x  , 0    , 0 },
        { 0    , 0    , 0    , 1 }
    };
    util::Matriz4 J(matrixJFloat);
    util::Matriz4 I;
    //Matriz de rotacion
    util::Matriz4 M =  (I + J*sinf(grados)) + (J*J)*(1.f-cosf(grados));

    //MOrg
    util::Matriz4 MOrg({
                           {1 , 0 , 0 , -punto.x},
                           {0 , 1 , 0 , -punto.y},
                           {0 , 0 , 1 , -punto.z},
                           {0 , 0 , 0 , 1}
                       });
    //MBack
    util::Matriz4 MBack({
                           {1 , 0 , 0 , punto.x},
                           {0 , 1 , 0 , punto.y},
                           {0 , 0 , 1 , punto.z},
                           {0 , 0 , 0 , 1}
                       });
    //Aplicamos esta matriz al resto de matrices
    util::Matriz4 matrizFinal = ((MBack*M)*MOrg)*matriz4D;
    matriz4D = matrizFinal;
}
void Transformacion::trasladar(util::Vector3 destino)
{
    util::Matriz4 traslacion({
        {1 , 0 , 0 , destino.x},
        {0 , 1 , 0 , destino.y},
        {0 , 0 , 1 , destino.z},
        {0 , 0 , 0 , 1}
    });
    util::Matriz4 matrizFinal = traslacion*matriz4D;
    this->matriz4D = matrizFinal;
}
void Transformacion::mostrarMatriz()
{
    for(std::size_t i=0 ; i<matriz4D.matriz.size() ; i++)
    {
        for(std::size_t j=0 ; j<matriz4D.matriz[i].size() ; j++)
            std::cout<<matriz4D.matriz[i][j];
        std::cout<<std::endl;
    }
}
void Transformacion::identidad()
{
    this->matriz4D.matriz = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
}
util::Vector3 Transformacion::nuevaPosicion(util::Vector3 v)
{
    std::vector<float> vertice = {v.x , v.y, v.z , 1};
    util::Vector3 nuevaUbicacion(0,0,0);
    for(int i=0 ; i<4 ; i++)
    {
        for(int j=0 ; j<4 ; j++)
        {
            switch(i)
            {
            case(0):nuevaUbicacion.x += matriz4D.matriz[i][j]*vertice[j];break;
            case(1):nuevaUbicacion.y += matriz4D.matriz[i][j]*vertice[j];break;
            case(2):nuevaUbicacion.z += matriz4D.matriz[i][j]*vertice[j];break;
            }
        }
    }
    return nuevaUbicacion;
}
