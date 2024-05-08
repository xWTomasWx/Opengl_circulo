#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

using namespace std;

void redimensionarPantalla(GLFWwindow *, int, int);
void procesarEntrada(GLFWwindow *);
void crearVentana();
void crearShaders();
void configurarDibujo();
	
// settings
const unsigned int ANCHO = 800;
const unsigned int ALTO = 600;

const char *vShaderCodigo = "#version 330 core\n"
    "layout (location = 0) in vec3 posV;\n"
    "out vec4 colorF;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(posV.x, posV.y, posV.z, 1.0);\n"
    "   colorF = vec4(1.0f, 0.0f, 0.0f, 1.0f);"
    "}\0";

const char *fShaderCodigo = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 colorF;\n"
    "void main()\n"
    "{\n"
    "   FragColor = colorF;\n"
    "}\n\0";


///Vables para crear Shaders
unsigned int vShader;
unsigned int fShader;
unsigned int pShader;

///Vables para crear Ventana
GLFWwindow* window;

//Variables para configurar Dibujo
float vertices[]={-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
unsigned int VBO, VAO;

const float PI = 3.14;
const int sides = 100;

int main(){
	//Crear Ventana
	crearVentana();
	
	//Creaci�n de Shaders
	crearShaders();
	
	//Configrar dibujo
	//configurarDibujo();
	
    ////// 
    int nAtributos;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAtributos);
    cout << "Número Máximo de Atributos: " << nAtributos << endl;
    ///////
    
    while (!glfwWindowShouldClose(window)){
        // input
        procesarEntrada(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(pShader);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        configurarDibujo();

        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(pShader);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// Procesar entradas del Teclado
void procesarEntrada(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
//    cout << "Tecla: " << glfw
}

// Redimensinar Ventana (pantalla)
void redimensionarPantalla(GLFWwindow* window, int ancho, int alto){
    glViewport(0, 0, ancho, alto);
}

void crearShaders(){
    // vertex shader vShader
    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderCodigo, NULL);
    glCompileShader(vShader);
    
    // fragment shader
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderCodigo, NULL);
    glCompileShader(fShader);

    // link shaders
    pShader = glCreateProgram();
    glAttachShader(pShader, vShader);
    glAttachShader(pShader, fShader);
    glLinkProgram(pShader);

    ///Eliminar variables de Shaders
    glDeleteShader(vShader);
    glDeleteShader(fShader);
	
}

void crearVentana(){
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw: Creación de ventana
    window = glfwCreateWindow(ANCHO, ALTO, "OpenGL Creacion circulo", NULL, NULL);
    
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(0);
    }

//    GLFWwindow* v2 = glfwCreateWindow(ANCHO, ALTO, "LearnOpenGL - Hola Triángulo FFFF", NULL, NULL);
    
    //Establecer la ventana principal!
    glfwMakeContextCurrent(window);
    
    //Establecer la función encargada de redimensionar la ventana
    glfwSetFramebufferSizeCallback(window, redimensionarPantalla);

    // glad: Cargar las funciones de glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(0);
    }
}

void configurarDibujo(){
//    vertices = (float *)malloc(sizeof(float)*9);
/*   vertices[0] = -0.5f; vertices[1] = -0.5f;
    vertices[2] = 0.0f; vertices[3] = 0.5f;
    vertices[4] = -0.5f; vertices[5] = 0.0f;
    vertices[6] = 0.0f; vertices[7] =  0.5f;
    vertices[8] = 0.0f; 
*/
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
}


