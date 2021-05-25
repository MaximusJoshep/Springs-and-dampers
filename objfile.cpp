#include "objfile.h"

ObjFile::ObjFile(std::string file)
{
    std::ifstream fileStream{file , std::ios_base::in};
    std::vector<float> vertexInput;
    std::vector<float> textureCoordinatesInput;
    std::vector<float> normalsInput;
    std::vector<int> indexesInput;

    if(fileStream.is_open())
    {
        while(!fileStream.eof())
        {
            std::string line;
            getline(fileStream , line);

            std::stringstream lineStream(line);

            std::string firstSymbol;

            lineStream >> firstSymbol;

            if(firstSymbol == "v")
            {
                float vertexPosition{};
                for(std::size_t i=0 ; i<3 ; i++)
                {
                    lineStream >> vertexPosition;
                    vertexInput.emplace_back(vertexPosition);
                }
            }
            else if(firstSymbol == "vt")
            {
                float textureCoordinate{};
                for(std::size_t i=0 ; i<2 ; i++)
                {
                    lineStream >> textureCoordinate;
                    textureCoordinatesInput.emplace_back(textureCoordinate);
                }
            }
            else if(firstSymbol == "vn")
            {
                float normal{};
                for(std::size_t i=0 ; i<3 ; i++)
                {
                    lineStream >> normal;
                    normalsInput.emplace_back(normal);
                }
            }
            else if(firstSymbol == "f")
            {
                std::string cad;
                int index{};
                for(int i=0 ; i<3 ; i++)
                {
                    lineStream>>cad;
                    index = stof(cad);
                    if(index != 0)
                    {
                        indexesInput.emplace_back(index);
                    }
                }
            }
        }
        this->vertex = vertexInput;
        this->textureCoordinates = textureCoordinatesInput;
        this->normals = normalsInput;
        this->indexes = indexesInput;
    }
    fileStream.close();
}

const std::vector<float> ObjFile::getVertex()
{
    return this->vertex;
}
const std::vector<float> ObjFile::getTextureCoordinates()
{
    return this->textureCoordinates;
}
const std::vector<float> ObjFile::getNormals()
{
    return this->normals;
}
const std::vector<int> ObjFile::getIndexes()
{
    return this->indexes;
}
