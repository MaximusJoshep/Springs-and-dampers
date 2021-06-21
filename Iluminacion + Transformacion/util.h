#ifndef UTIL_H
#define UTIL_H

#endif // UTIL_H

#include <vector>
#include <cmath>
#include <iostream>
namespace util
{

    class Vector3
    {
        friend Vector3 operator+(const Vector3& v1, const Vector3& v2);
        friend Vector3 operator-(const Vector3& v1, const Vector3& v2);
        friend Vector3 operator*(const Vector3& v1, const float n);
        friend Vector3 operator*(const Vector3& v1, const Vector3& v2);
        friend Vector3 operator/(const Vector3& v1, const float n);
    public:
        float x,y,z;
        Vector3(float xp, float yp, float zp){x=xp; y=yp; z=zp;}
        Vector3(){this->x=0; this->y=0; this->z=0;}

        float modulo(){return sqrt(pow(x,2)+pow(y,2)+pow(z,2));}
        Vector3 normalizar()
        {
            float mod = modulo();
            return Vector3(x/mod , y/mod , z/mod);
        }
        void mostrar()
        {
            std::cout<<"x: "<<this->x<<"\ty:"<<this->y<<"\tz:"<<this->z<<std::endl;
        }
    };
    inline Vector3 operator+(const Vector3& v1, const Vector3& v2){return Vector3(v1.x+v2.x , v1.y+v2.y , v1.z+v2.z);}
    inline Vector3 operator-(const Vector3& v1, const Vector3& v2){return Vector3(v1.x-v2.x , v1.y-v2.y , v1.z-v2.z);}
    inline Vector3 operator*(const Vector3& v1, const Vector3& v2){return Vector3(v1.x*v2.x , v1.y*v2.y , v1.z*v2.z);}
    inline Vector3 operator*(const Vector3& v1, const float n){return Vector3(v1.x*n , v1.y*n , v1.z*n);}
    inline Vector3 operator/(const Vector3& v1, const float n){return v1*(1/n);}


    class Matriz4
    {
    public:
        std::vector<std::vector<float>> matriz{{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}};
        Matriz4()
        {
            this->matriz = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
        }
        Matriz4(std::vector<std::vector<float>> m)
        {
            for(std::size_t i=0 ; i<m.size() ; i++)
                for(std::size_t j=0 ; j<m[i].size() ; j++)
                    this->matriz[i][j] = m[i][j];
        }
        void mostrar()
        {
            for(std::size_t i=0 ; i<matriz.size() ; i++)
            {
                for(std::size_t j=0 ; j<matriz[i].size() ; j++)
                   std::cout<<matriz[i][j]<<"\t";
                std::cout<<std::endl;
            }
        }
        friend Matriz4 operator*(const Matriz4& m1, const Matriz4& m2);
        friend Matriz4 operator*(const Matriz4& m1, const float& num);
        friend Matriz4 operator*(const float& num , const Matriz4& m1);
        friend Matriz4 operator+(const Matriz4& m1, const Matriz4& m2);
    };
    inline Matriz4 operator*(const Matriz4& m1, const Matriz4& m2)
    {
        Matriz4 mult;
        for(std::size_t i=0 ; i<m1.matriz.size() ; i++)
        {
            for(std::size_t j=0 ; j<m2.matriz[0].size() ; j++)
            {
                mult.matriz[i][j]=0;
                for(std::size_t k=0 ; k<m2.matriz[0].size() ; k++)
                   mult.matriz[i][j] += m1.matriz[i][k] * m2.matriz[k][j];
            }
        }
        return mult;
    }
    inline Matriz4 operator*(const Matriz4& m1, const float& num)
    {
        Matriz4 mult;
        for(std::size_t i=0 ; i<m1.matriz.size() ; i++)
            for(std::size_t j=0 ; j<m1.matriz[i].size() ; j++)
                mult.matriz[i][j] = m1.matriz[i][j]*num;
        return mult;
    }
    inline Matriz4 operator*(const float& num , const Matriz4& m1){return m1*num;}
    inline Matriz4 operator+(const Matriz4& m1, const Matriz4& m2)
    {
        Matriz4 suma;
        for(std::size_t i=0 ; i<m1.matriz.size() ; i++)
            for(std::size_t j=0 ; j<m2.matriz[0].size() ; j++)
                suma.matriz[i][j] = m1.matriz[i][j] + m2.matriz[i][j];
        return suma;
    }
}

