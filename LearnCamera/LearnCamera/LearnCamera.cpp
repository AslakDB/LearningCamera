#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Includes.h"

struct Vertex {
    float x, y, z, r, g, b, u, v;
};
struct Line {
    std::vector<Vertex> mVertices;
    glm::mat4 matrix = glm::mat4(1.0f);
};
struct TriangleSurface {
    std::vector<Vertex> mVertices;
    glm::mat4 matrix = glm::mat4(1.0f);
    TriangleSurface();
};

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);  // Camera is at (4,3,3), in World Space
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);     // and looks at the origin
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); // The camera's direction is the vector from the camera to the cameraTarget

glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Head is up (set to 0,-1,0 to look upside-down)
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection)); // The Right vector is the cross product of Up and Direction
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight); // The Up vector is the cross product of Direction and Right

void processInput(GLFWwindow *window);
const char *vertexSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos, 1.0);\n"
    "ourColor = aColor;\n"
    "}\n";
    
const char *fragmentSource =
    "#version 330 core\n"
    "in vec3 ourColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(ourColor, 1.0f);\n"
    "}\n";


int main()
{
    
   
    SettUp settup(800, 600, "LearnCamera");

    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    
    // Create and compile the fragment shader
 
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    while(!glfwWindowShouldClose(settup.window))
    {
        //input here
        processInput(settup.window);

        //Rendering commands here----------------------------------------------------------------------------------------------------------------------------

        glClearColor(0.498f, 1.0f, 0.831f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        //------------------------------------------------------
        glfwSwapBuffers(settup.window);
        glfwPollEvents();
    }
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}