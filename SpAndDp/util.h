#ifndef UTIL_H
#define UTIL_H

#endif // UTIL_H

#include <vector>
#include <cmath>
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
        Vector3(){x=0; y=0; z=0;}

        float modulo(){return sqrt(pow(x,2)+pow(y,2)+pow(z,2));}
        Vector3 normalizar()
        {
            float mod = modulo();
            return Vector3(x/mod , y/mod , z/mod);
        }
    };
    inline Vector3 operator+(const Vector3& v1, const Vector3& v2){return Vector3(v1.x+v2.x , v1.y+v2.y , v1.z+v2.z);}
    inline Vector3 operator-(const Vector3& v1, const Vector3& v2){return Vector3(v1.x-v2.x , v1.y-v2.y , v1.z-v2.z);}
    inline Vector3 operator*(const Vector3& v1, const float n){return Vector3(v1.x*n , v1.y*n , v1.z*n);}
    inline Vector3 operator/(const Vector3& v1, const float n){return v1*(1/n);}
    inline Vector3 operator*(const Vector3& v1, const Vector3& v2){return Vector3(v1.x*v2.x , v1.y*v2.y , v1.z*v2.z);}
}

