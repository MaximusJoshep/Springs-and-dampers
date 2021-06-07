#include "archivoobj.h"

ArchivoObj::ArchivoObj(std::string file)
{
    std::ifstream archivo{file , std::ios_base::in};

    if(archivo.is_open())
    {
        while(!archivo.eof())
        {
            std::string line;
            getline(archivo , line);
            std::stringstream lineStream(line);
            std::string firstSymbol;
            lineStream >> firstSymbol;
            if(firstSymbol == "v")
            {
                float vx{};
                float vy{};
                float vz{};

                lineStream >> vx;
                lineStream >> vy;
                lineStream >> vz;

                todosVertices.emplace_back(Vertice(vx , vy , vz));

            }
            else if(firstSymbol == "vt")
            {
                float textureCoordinate{};
                for(std::size_t i=0 ; i<2 ; i++)
                {
                    lineStream >> textureCoordinate;
                    coordenadasTextura.emplace_back(textureCoordinate);
                }
            }
            else if(firstSymbol == "vn")
            {
                float normal{};
                for(std::size_t i=0 ; i<3 ; i++)
                {
                    lineStream >> normal;
                    normales.emplace_back(normal);
                }
            }
            else if(firstSymbol == "f")
            {
                std::string cad;

                //Indices de las 3 caras
                int indice1{};
                lineStream>>cad;
                indice1 = stof(cad);
                if(indice1 != 0)
                {
                    indices.emplace_back(indice1-1);
                }

                int indice2{};
                lineStream>>cad;
                indice2 = stof(cad);
                if(indice2 != 0)
                {
                    indices.emplace_back(indice2-1);
                }

                int indice3{};
                lineStream>>cad;
                indice3 = stof(cad);
                if(indice3 != 0)
                {
                    indices.emplace_back(indice3-1);
                }

                aristas.emplace_back(std::vector<int>{indice1-1,indice2-1});
                aristas.emplace_back(std::vector<int>{indice1-1,indice3-1});
                aristas.emplace_back(std::vector<int>{indice2-1,indice3-1});
            }
        }
        archivo.close();
    }
}

void ArchivoObj::springsAndDampers()
{

    float k = 15; //Constante spring
    float d = 0.99999; //Constante damper
    for(std::size_t i=0 ; i<aristas.size() ; i++)
    {
        Vertice v1 = todosVertices[aristas[i][0]];
        Vertice v2 = todosVertices[aristas[i][1]];

        util::Vector3 r = v2.posicion - v1.posicion;     //Distancia de resposo
        util::Vector3 vr = v2.velocidad - v1.velocidad;    //Diferencia de velocidades
        float dl = r.modulo() - distancias[i];                   //Diferencia de longitud = (L-r)

        float f = k * dl;
        util::Vector3 rNorm = r.normalizar();       // L/L

        util::Vector3 F = (r*f) + ((vr*rNorm*d)*rNorm);    //F = (r*f) + (d *(vr.*r)).*r;

        todosVertices[aristas[i][0]].agregarFuerza(F);
        todosVertices[aristas[i][1]].agregarFuerza(F*-1);
    }
}

void ArchivoObj::iniciar(GLuint& vbo)
{
    util::Vector3 gravedad(0,-9.8,0);
    for(std::size_t i=0; i<todosVertices.size() ; i++)
        todosVertices[i].agregarFuerza(gravedad);

    //Distancias de reposo
    distancias = std::vector<float>(aristas.size(),0);
    for(std::size_t i=0 ; i<aristas.size() ; i++)
    {
        Vertice v1 = todosVertices[aristas[i][0]];
        Vertice v2 = todosVertices[aristas[i][1]];
        util::Vector3 dif = v2.posicion-v1.posicion;
        distancias[i] = dif.modulo();
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);
}

void ArchivoObj::mostrar(GLuint& vbo)
{
    springsAndDampers();

    vertices.clear();
    for(std::size_t i=0 ; i<indices.size() ; i++)
    {
        vertices.emplace_back(todosVertices[indices[i]].posicion.x);
        vertices.emplace_back(todosVertices[indices[i]].posicion.y);
        vertices.emplace_back(todosVertices[indices[i]].posicion.z);
    }
    for(std::size_t i=0 ; i<todosVertices.size() ; i++)
    {
        todosVertices[i].actualizar();
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(float), &this->vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size()/3);
}

void ArchivoObj::trasladar(float x, float y, float z)
{
    util::Vector3 mover(x,y,z);
    for(std::size_t i=0 ; i<todosVertices.size() ; i++)
    {
        todosVertices[i].posicion = todosVertices[i].posicion + mover;

    }
}

