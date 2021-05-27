#ifndef OBJFILE_H
#define OBJFILE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ObjFile
{

public:
    ObjFile(std::string file);
    std::vector<Vertex> vertex;
    std::vector<float> textureCoordinates;
    std::vector<float> normals;
    std::vector<int> indexes;

};

#endif // OBJFILE_H
