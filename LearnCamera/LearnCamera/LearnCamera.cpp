#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Includes.h"
#include "glm/gtx/quaternion.hpp"

struct Vertex
{
    float x, y, z, r, g, b;

    Vertex(float x, float y, float z, float r, float g, float b) : x(x), y(y), z(z), r(r), g(g), b(b)
    {
    }
};


float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.0f); // Camera is at (4,3,3), in World Space
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); // and looks at the origin
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Head is up (set to 0,-1,0 to look upside-down)


void processInput(GLFWwindow* window);
const char* vertexSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec3 aColor;\n"
    "uniform mat4 model;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = model* projection * view * vec4(aPos, 1.0);\n"
    "ourColor = aColor;\n"
    "}\n";

const char* fragmentSource =
    "#version 330 core\n"
    "in vec3 ourColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(ourColor, 1.0f);\n"
    "}\n";


int main()
{
    SettUp settup(1600, 1200, "LearnCamera");

    
    
    glm::mat4 model;
    model = rotate(translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -1.0f)), glm::radians(45.0f),
                   glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(800) / static_cast<float>(600), 0.1f, 100.0f);

    glm::mat4 view;
    view = lookAt(cameraPos, cameraTarget, up);


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);


    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int prodLoc = glGetUniformLocation(shaderProgram, "projection");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    std::vector<Vertex> vertices;
    vertices.emplace_back(-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
    vertices.emplace_back(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
    vertices.emplace_back(0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
    vertices.emplace_back(0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
    vertices.emplace_back(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f);
    vertices.emplace_back(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f);
    vertices.emplace_back(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f);
    vertices.emplace_back(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f);
   

    std::vector<unsigned int> indices = {
         0, 2, 3, 0, 3, 1, // back face
          2, 6, 7, 2, 7, 3, // front face
          6, 4, 5, 6, 5, 7, // top face
          4, 0, 1, 4, 1, 5, // bottom face
          0, 4, 6, 0, 6, 2, // right face
          1, 5, 7, 1, 7, 3,// left face
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(nullptr));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    float rotationAngle = 0.0f;

    glEnable(GL_DEPTH_TEST);
    
    //glDepthMask(GL_FALSE); 
    lastFrame = glfwGetTime();
    while (!glfwWindowShouldClose(settup.window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        rotationAngle += 45*deltaTime; // Increase the rotation angle
        model = rotate(translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -1.0f)), glm::radians(rotationAngle),
        glm::vec3(0.0f, 0.0f, 1.0f))* rotate(glm::mat4(1.f), glm::radians(rotationAngle),
        glm::vec3(0.0f, 1.0f, 0.0f));

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(settup.window);

        view = lookAt(cameraPos, cameraTarget, up);
        //Rendering commands here----------------------------------------------------------------------------------------------------------------------------

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
        glUniformMatrix4fv(prodLoc, 1, GL_FALSE, value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
        //  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glClearColor(1.0f, 0.0f, 1.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        //glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        
        //------------------------------------------------------
        glfwSwapBuffers(settup.window);
        glfwPollEvents();
    }
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos.z += -1.f * deltaTime;
        cameraTarget.z += -1.f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos.z += 1.f * deltaTime;
        cameraTarget.z += 1.f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos.x += -1.f * deltaTime;
        cameraTarget.x += -1.f * deltaTime;

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos.x += 1.f * deltaTime;
        cameraTarget.x += 1.f * deltaTime;
    }
}
