#include <iostream>
#include <vector>
#include "archivoobj.h"
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"



void CheckSphereCollition(Vertice& vertice)
{
    float sphereRadius = 1;
    float sLength = vertice.posicion.modulo();
    if (sLength < sphereRadius * sphereRadius)
    {
        util::Vector3 np = vertice.posicion.normalizar();
        vertice.posicion = np * sphereRadius * 1.000001;
        vertice.velocidad = np;
        vertice.agregarFuerza(np);
    }
}


int main()
{
    //Se usaron los modelos: icoesfera.obj y entrada.obj
    std::string direccion1 = "C:/Users/Maxim/OneDrive/Escritorio/Septimo semestre/Computacion Grafica/GL/GL/icoesfera.obj";
    std::string direccion2 = "C:/Users/Maxim/OneDrive/Escritorio/Septimo semestre/Computacion Grafica/GL/GL/entrada.obj";
    ArchivoObj ao(direccion1);
    ArchivoObj bo(direccion2);
    std::cout << "Hola llegamos" << std::endl;
    int width = 600;
    int height = 600;
    GLFWwindow* window;
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    window = glfwCreateWindow(width, height, "Window Title", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
 

   
    GLuint vbo[6];
    glGenBuffers(6, vbo);

    bo.iniciar(vbo[0]);
    bo.trasladar(-1, 3, 0);

    ao.iniciar(vbo[2]);
    ao.trasladar(1, 3, 0);

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
    float cameraZ = 7.f;

    float lockX = 0.0;
    float lockY = -2.0;
    float lockZ = 0.0;





    glm::mat4 pMat, vMat, mMat, mvMat;
  

    // initial light location
    
    // white light properties
    float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
    float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };


    float matAmb[4] = { 0.2473f, 0.1995f, 0.0745f, 1 };
    float matDif[4] = { 0.7516f, 0.6065f, 0.2265f, 1 };
    float matSpe[4] = { 0.6283f, 0.5558f, 0.3661f, 1 };
    float matShi = 51.2f;

    

    
    glm::vec3 currentLightPos, lightPosV; // light position as Vector3f, in both model and view space
    float lightPos[3]; // light position as float array
    // gold material properties

    glm::vec3 initialLightLoc = glm::vec3(5.0f, 5.0f, 2.0f);

    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.03f, 0.05f, 0.0f);


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
        //Get variables
        GLuint mvLoc = glGetUniformLocation(shader_programme, "mv_matrix");
       
     
        //Perspective
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = static_cast<float>(width)/static_cast<float>(height);
        pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
        //Translation
        vMat = glm::translate(glm::mat4(1.f), glm::vec3(-cameraX, -cameraY, -cameraZ));

        mMat = glm::translate(glm::mat4(1.f), glm::vec3(lockX, lockY, lockZ));
        mvMat = vMat * mMat;

        glm::mat4 invTrMat = glm::transpose(glm::inverse(mvMat));
        GLuint nLoc = glGetUniformLocation(shader_programme, "norm_matrix");
     
        //Light
        // set up lights based on the current light’s position
        currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
        //installLights(vMat);
        lightPosV = glm::vec3(vMat * glm::vec4(currentLightPos, 1.0));
        lightPos[0] = lightPosV.x;
        lightPos[1] = lightPosV.y;
        lightPos[2] = lightPosV.z;
        // get the locations of the light and material fields in the shader
    


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
        
        // build the MODEL-VIEW (MV) matrix by concatenating matrices v and m, as before
      
        // build the inverse-transpose of the MV matrix, for transforming normal vectors
      
 

        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
        glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
     
   

   
        //-----------------------FIN CAMARA------------------------
        
        for (std::size_t i = 0; i < ao.todosVertices.size(); i++)
        {
            CheckSphereCollition(ao.todosVertices[i]);
        }
      
        for (std::size_t i = 0; i < bo.todosVertices.size(); i++)
        {
            CheckSphereCollition(bo.todosVertices[i]);
        }
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
        glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
  
        bo.mostrar(vbo[0], vbo[1]);
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
        glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
   
        ao.mostrar(vbo[2], vbo[3]);
        

      

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

