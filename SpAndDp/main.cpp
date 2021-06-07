#include <iostream>
#include <GL/glew.h>
#include <vector>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "archivoobj.h"


void CheckSphereCollition(Vertice& vertice)
{
    float sphereRadius = 1;
    float sLength = vertice.posicion.modulo();
    if( sLength < sphereRadius*sphereRadius)
    {
        util::Vector3 np = vertice.posicion.normalizar();
        vertice.posicion = np*sphereRadius*1.000001;
        vertice.velocidad = np;
        vertice.agregarFuerza(np);
    }
}

int main()
{
    //Se usaron los modelos: icoesfera.obj y entrada.obj
    std::string direccion1 = "/home/michael/icoesfera.obj";
    std::string direccion2 = "/home/michael/entrada.obj";
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
    GLuint vbo[2];


    bo.iniciar(vbo[0]);
    bo.trasladar(-1,3,0);

    ao.iniciar(vbo[1]);
    ao.trasladar(1,3,0);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //shaders
    const char* vertex_shader =
            "#version 400\n"
            "in vec3 vp;"
            "uniform mat4 mvMatrix;"     //Modelo-vista
            "uniform mat4 projMatrix;"   //Proyeccion
            "void main() {"
            "  gl_Position = projMatrix * mvMatrix * vec4(vp,1.0);"
            "}";

    const char* fragment_shader =
            "#version 400\n"
            "uniform mat4 mvMatrix;"     //Modelo-vista
            "uniform mat4 projMatrix;"   //Proyeccion
            "out vec4 frag_color;"
            "void main() {"
            "  frag_color = vec4(0.1, .9, 0.3, 1.0);"
            "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
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
    float lockY = 0.0;
    float lockZ = 0.0;


    GLuint mvLoc;
    GLuint projLoc;

    glm::mat4 pMat, vMat, mMat, mvMat;

    while(glfwWindowShouldClose(window) == 0 )
    {
        glClear( GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.03f, 0.05f, 0.0f);

        //------------------------CAMARA--------------------------
        //Get variables
        mvLoc = glGetUniformLocation(shader_programme , "mvMatrix");
        projLoc = glGetUniformLocation(shader_programme , "projMatrix");
        //Perspective
        glfwGetFramebufferSize(window, &width , &height);
        float aspect = static_cast<float>(width)/static_cast<float>(height);
        pMat = glm::perspective(1.0472f , aspect , 0.1f , 1000.0f);
        //Translation
        vMat = glm::translate(glm::mat4(1.f) , glm::vec3(-cameraX, -cameraY, -cameraZ));
        mMat = glm::translate(glm::mat4(1.f) , glm::vec3(lockX , lockY , lockZ));
        mvMat = vMat * mMat;
        //Save changes
        glUniformMatrix4fv(mvLoc , 1 , GL_FALSE, glm::value_ptr(mvMat));
        glUniformMatrix4fv(projLoc , 1 , GL_FALSE, glm::value_ptr(pMat));
        //-----------------------FIN CAMARA------------------------

        for(std::size_t i=0 ; i<ao.todosVertices.size() ; i++)
        {
            CheckSphereCollition(ao.todosVertices[i]);
        }
        for(std::size_t i=0 ; i<bo.todosVertices.size() ; i++)
        {
            CheckSphereCollition(bo.todosVertices[i]);
        }
        ao.mostrar(vbo[0]);
        bo.mostrar(vbo[1]);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;

    return 0;
}

