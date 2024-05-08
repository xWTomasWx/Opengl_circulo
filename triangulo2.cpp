#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const char* vertexShaderCodigo = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 posV;\n"
    "uniform float escala;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(posV.x * escala, posV.y * escala, posV.z, 1.0);\n"
    "}\0";

const char* fragmentShaderCodigo = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

class Shader {
private:
    unsigned int id;
public:
    Shader(const char* vertexShaderCodigo, const char* fragmentShaderCodigo) {
        // Crea y compila al vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCodigo, NULL);
        glCompileShader(vertexShader);

        // Crea y compila al fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCodigo, NULL);
        glCompileShader(fragmentShader);

        // Crea un progrma y los attach shaders (se linkean el vertex y el fragment)
        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);

        // Elimina los shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    unsigned int getId(){
        return id;
    }
};

class Ventana {
private:
    GLFWwindow* ventana;
    int ancho, alto;
public:
    static void framebufferSizeCallback(GLFWwindow* ventana, int ancho, int alto) {
        glViewport(0, 0, ancho, alto);
    }

    Ventana(int _ancho, int _alto, const char* titulo) : ancho(_ancho), alto(_alto){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        ventana = glfwCreateWindow(ancho, alto, titulo, NULL, NULL);
        if (ventana == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(0);
        }

        glfwMakeContextCurrent(ventana);
        glfwSetFramebufferSizeCallback(ventana, framebufferSizeCallback);
    }

    GLFWwindow *getVentana(){
        return ventana;
    }
};

class Dibujo{
private:
    unsigned int VAO, VBO;
    Shader shader;
    const int n = 100;
    float escala, aumento;
public:
    Dibujo(const char* vertexShaderCodigo, const char* fragmentShaderCodigo) : shader(vertexShaderCodigo, fragmentShaderCodigo){
        escala = 0.5f;
        aumento = 0.002f;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        

        float vertices[(n+1)*3];

        for(int i = 0; i <= n; i++){
            float t = 2.0f*M_PI*i/n;
            vertices[i*3] = cos(t); // Esto es x
            vertices[(i*3)+1] = sin(t); // Esto es y
            vertices[(i*3)+2] = 0.0f; //Esto es cero porque estamos trabajando en 2D, esto seria z
        }


        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    ~Dibujo(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    Shader getShader(){
        return shader;
    }
    void cambiarEscala(int numero){
        escala += aumento*numero;
        if(escala < 0.01f){
            escala = 0.01f;
        }
        if(escala > 1.0f){
            escala = 1.0f;
        }
    }
    void dibujar(){
        glUseProgram(shader.getId());
        glUniform1f(glGetUniformLocation(shader.getId(), "escala"), escala);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, n);
    }
};

void processInput(GLFWwindow* ventana, Dibujo& d){
    if(glfwGetKey(ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(ventana, true);
    }
    if(glfwGetKey(ventana, GLFW_KEY_DOWN) == GLFW_PRESS){
        d.cambiarEscala(-1);
    }
    if(glfwGetKey(ventana, GLFW_KEY_UP) == GLFW_PRESS){
        d.cambiarEscala(1);
    }
    if(glfwGetKey(ventana, GLFW_KEY_1) == GLFW_PRESS){
        d.cambiarEscala(1);
    }
    if(glfwGetKey(ventana, GLFW_KEY_2) == GLFW_PRESS){
        d.cambiarEscala(1);
    }
    if(glfwGetKey(ventana, GLFW_KEY_3) == GLFW_PRESS){
        d.cambiarEscala(1);
    }
    if(glfwGetKey(ventana, GLFW_KEY_4) == GLFW_PRESS){
        d.cambiarEscala(1);
    }
    if(glfwGetKey(ventana, GLFW_KEY_5) == GLFW_PRESS){
        d.cambiarEscala(1);
    }
    if(glfwGetKey(ventana, GLFW_KEY_6) == GLFW_PRESS){
        d.cambiarEscala(1);
    }
}




int main(){
    Ventana v(800, 800, "OpenGL Circle");

    // Make the window's context current before loading GLAD
    glfwMakeContextCurrent(v.getVentana());

    // Initialize GLAD after making the context current
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Dibujo d(vertexShaderCodigo, fragmentShaderCodigo);

    while(!glfwWindowShouldClose(v.getVentana())){
        // input
        processInput(v.getVentana(), d);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        d.dibujar();

        glfwSwapBuffers(v.getVentana());
        glfwPollEvents();
    }

    glDeleteProgram(d.getShader().getId());

    glfwTerminate();
    return 0;
}