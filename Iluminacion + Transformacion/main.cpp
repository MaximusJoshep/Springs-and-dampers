#include <iostream>
#include <GL/glew.h>
#include <vector>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "archivoobj.h"
#include "transformacion.h"

void CheckSphereCollition(Vertice& vertice)
{
    float sphereRadius = 1;
    float sLength = vertice.posicion.modulo();
    if( sLength < sphereRadius*sphereRadius)
    {
        util::Vector3 np = vertice.posicion.normalizar();
        vertice.posicion = np*sphereRadius;
        vertice.velocidad = np;
        vertice.agregarFuerza(np);
    }
}
bool CheckBorderCollition(Vertice& vertice)
{
    int distX = 10;
    int distY = 6;
    if( vertice.posicion.x <= -distX ||
        vertice.posicion.x >= distX  ||
        vertice.posicion.y <= -distY ||
        vertice.posicion.y >= distY  ||
        vertice.posicion.z >= 8)
    {
        return true;
    }
    return false;
}
void CheckCollisions(ArchivoObj& obj)
{
    if(!obj.estatico)
        for(std::size_t i=0 ; i<obj.todosVertices.size() ; i++)
        {
            CheckSphereCollition(obj.todosVertices[i]);
            if(CheckBorderCollition(obj.todosVertices[i]))
                obj.estatico = true;
        }
}


int main()
{
    //Se usaron los modelos: icoesfera.obj y entrada.obj
    std::string direccion2 = "/home/michael/icoesfera.obj";
    std::string direccion1 = "/home/michael/claw.obj";
    ArchivoObj ao(direccion1);
    ArchivoObj bo(direccion2);

    int width = 600;
    int height = 600;
    GLFWwindow* window;
    if( !glfwInit() )
    {
        std::cerr<<"Failed to initialize GLFW"<<std::endl;
        return -1;
    }
    window = glfwCreateWindow(width ,height , "Window Title", nullptr,nullptr);
    if(window == nullptr)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr<<"Failed to initialize GLEW"<<std::endl;
        glfwTerminate();
        return -1;
    }
    //Setting up model on OpenGL---------------------
    GLuint vbo[4];
    glGenBuffers(4, vbo);

    //ICOESFERA
    ao.iniciar(vbo[0]);
    Transformacion t1;
    t1.trasladar(util::Vector3(4,3,0));
    ao.transformar(t1);

    //CONEJO
    bo.iniciar(vbo[2]);
    Transformacion t2;
    t2.trasladar(util::Vector3(0,4,0));
    bo.transformar(t2);


    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //shaders
    const char* vertex_shader =
        "#version 400\n"
        "layout (location=0) in vec3 vertPos;"
        "layout(location = 1) in vec3 vertNormal;"
        "out vec3 varyingNormal;" // eye-space vertex normal
        "out vec3 varyingLightDir;" // vector pointing to the light
        "out vec3 varyingVertPos;" // vertex position in eye space
        "struct PositionalLight {"
        "vec4 ambient;"
        "vec4 diffuse;"
        "vec4 specular;"
        "vec3 position;"
        "};"
        "struct Material  {"
        "vec4 ambient;"
        "vec4 diffuse;"
        "vec4 specular;"
        "float shininess;"
        "};"
        "uniform vec4 globalAmbient;"
        "uniform PositionalLight light;"
        "uniform Material material;"
        "uniform mat4 mv_matrix;"
        "uniform mat4 proj_matrix;"
        "uniform mat4 norm_matrix;"
        "void main() {"
        "varyingVertPos=(mv_matrix * vec4(vertPos,1.0)).xyz;"
        "varyingLightDir = light.position - varyingVertPos;"
        "varyingNormal = (norm_matrix * vec4(vertNormal, 1.0)).xyz;"
        "gl_Position = proj_matrix * mv_matrix * vec4(vertPos, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 400\n"
        "in vec3 varyingNormal;"
        "in vec3 varyingLightDir;"
        "in vec3 varyingVertPos;"
        "out vec4 fragColor;"
        "struct PositionalLight {"
        "vec4 ambient;"
        "vec4 diffuse;"
        "vec4 specular;"
        "vec3 position;"
        "};"
        "struct Material  {"
        "vec4 ambient;"
        "vec4 diffuse;"
        "vec4 specular;"
        "float shininess;"
        "};"
        "uniform vec4 globalAmbient;"
        "uniform PositionalLight light;"
        "uniform Material material;"
        "uniform mat4 mv_matrix;"
        "uniform mat4 proj_matrix;"
        "uniform mat4 norm_matrix;"
        "void main() {"

        "vec3 L = normalize(varyingLightDir);"
        "vec3 N = normalize(varyingNormal);"
        "vec3 V = normalize(-varyingVertPos);"
        // compute light reflection vector with respect to N:
        "vec3 R = normalize(reflect(-L, N));"
        // get the angle between the light and surface normal:
        "float cosTheta = dot(L, N);"
        // angle between the view vector and reflected light:
        "float cosPhi = dot(V, R);"
        // compute ADS contributions (per pixel), and combine to build output color:
        "vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;"
        "vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta, 0.0);"
        "vec3 specular =light.specular.xyz * material.specular.xyz * pow(max(cosPhi, 0.0), material.shininess);"
        "fragColor = vec4((ambient + diffuse + specular), 1.0);"
        "}";


    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    GLuint  fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
    glUseProgram(shader_programme);


    float cameraX = 0.f;
    float cameraY = 0.f;
    float cameraZ = 12.f;

    float lockX = 0.0;
    float lockY = 0.0;
    float lockZ = 0.0;

    //Matrices de perspectiva
    glm::mat4 pMat, vMat, mMat, mvMat;

    // white light properties
    float globalAmbient[4] = { 0.2f, 0.3f, .2f, 1.0f };
    float lightAmbient[4] = { 0.0f, 0.01f, 0.0f, 1.0f };
    float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float lightSpecular[4] = { 0.2f, 0.0f, 0.0f, 1.0f };


    float matAmb[4] = { 0.1, 0.0, 0.1, 1 };
    float matDif[4] = { 0.4, 0.9, 0.5, 1 };
    float matSpe[4] = { 0.1, 0.1, 0.1, 1 };
    float matShi = 51.2f;

    glm::vec3 currentLightPos, lightPosV; // light position as Vector3f, in both model and view space
    float lightPos[3]; // light position as float array
    // gold material properties

    //Posicion de la luz
    glm::vec3 initialLightLoc = glm::vec3(3 , 4 , 0);
    currentLightPos = initialLightLoc;
    //Transformacion para la rotacion
    Transformacion tRotacion;
    Transformacion rotacionLuz;
    while(glfwWindowShouldClose(window) == 0 )
    {
        glClear( GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.03f, 0.05f, 0.0f);


        //----------------------ILUMINACION-----------------------
        GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;

        globalAmbLoc = glGetUniformLocation(shader_programme, "globalAmbient");
        ambLoc = glGetUniformLocation(shader_programme, "light.ambient");
        diffLoc = glGetUniformLocation(shader_programme, "light.diffuse");
        specLoc = glGetUniformLocation(shader_programme, "light.specular");
        posLoc = glGetUniformLocation(shader_programme, "light.position");
        mAmbLoc = glGetUniformLocation(shader_programme, "material.ambient");
        mDiffLoc = glGetUniformLocation(shader_programme, "material.diffuse");
        mSpecLoc = glGetUniformLocation(shader_programme, "material.specular");
        mShiLoc = glGetUniformLocation(shader_programme, "material.shininess");

        GLuint projLoc = glGetUniformLocation(shader_programme, "proj_matrix");
        //------------------------CAMARA--------------------------
        //Obtencion de Variables
        GLuint mvLoc = glGetUniformLocation(shader_programme, "mv_matrix");
        //Perpectiva
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = static_cast<float>(width)/static_cast<float>(height);
        pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
        //Traslacion
        vMat = glm::translate(glm::mat4(1.f), glm::vec3(-cameraX, -cameraY, -cameraZ));

        mMat = glm::translate(glm::mat4(1.f), glm::vec3(lockX, lockY, lockZ));
        mvMat = vMat * mMat;

        glm::mat4 invTrMat = glm::transpose(glm::inverse(mvMat));
        GLuint nLoc = glGetUniformLocation(shader_programme, "norm_matrix");

        //Luces
        //ROtacion de la luz
        rotacionLuz.identidad();
        rotacionLuz.rotar(util::Vector3(1,0,0) , util::Vector3(0,0,0) , 0.05);
        util::Vector3 posicionActualLuz(currentLightPos.x , currentLightPos.y , currentLightPos.z);
        util::Vector3 posLuz = rotacionLuz.nuevaPosicion(posicionActualLuz);

        //Inicializacion de las luces de acuerdo a su posicion
        currentLightPos = glm::vec3(posLuz.x , posLuz.y , posLuz.z);
        //installLights(vMat);
        lightPosV = glm::vec3(vMat * glm::vec4(currentLightPos, 15.0));
        lightPos[0] = lightPosV.x;
        lightPos[1] = lightPosV.y;
        lightPos[2] = lightPosV.z;
        // set the uniform light and material values in the shader
        glProgramUniform4fv(shader_programme, globalAmbLoc, 1, globalAmbient);
        glProgramUniform4fv(shader_programme, ambLoc, 1, lightAmbient);
        glProgramUniform4fv(shader_programme, diffLoc, 1, lightDiffuse);
        glProgramUniform4fv(shader_programme, specLoc, 1, lightSpecular);

        glProgramUniform3fv(shader_programme, posLoc, 1, lightPos);

        glProgramUniform4fv(shader_programme, mAmbLoc, 1, matAmb);
        glProgramUniform4fv(shader_programme, mDiffLoc, 1, matDif);
        glProgramUniform4fv(shader_programme, mSpecLoc, 1, matSpe);
        glProgramUniform1f(shader_programme, mShiLoc, matShi);
        //Save changes
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
        glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));


        //--------------------------COLISIONES---------------------
        CheckCollisions(ao);
        CheckCollisions(bo);

        //-----------------------TRANSFORMACIONES-------------------
        tRotacion.identidad();
        tRotacion.rotar(util::Vector3(0,1,0), util::Vector3(0,0,0) , 0.05);
        ao.transformar(tRotacion);
        tRotacion.identidad();
        tRotacion.rotar(util::Vector3(0,1,0), util::Vector3(0,0,0) , 0.05);
        bo.transformar(tRotacion);

        //-----------------------MOSTRAR OBJETOS--------------------
        ao.mostrar(vbo[0], vbo[1]);
        bo.mostrar(vbo[2], vbo[3]);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

