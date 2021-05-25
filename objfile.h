#ifndef OBJFILE_H
#define OBJFILE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <iostream>

class ObjFile
{
    std::vector<float> vertex;
    std::vector<float> textureCoordinates;
    std::vector<float> normals;
    std::vector<int> indexes;

public:
    ObjFile(std::string file);
    const std::vector<float> getVertex();
    const std::vector<float> getTextureCoordinates();
    const std::vector<float> getNormals();
    const std::vector<int> getIndexes();

};

#endif // OBJFILE_H
