//Previo3
//Monica Ixchel Urzua Cruz
//320110450
//Fecha de entrega 22/02/2026
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
        "Proyecciones y transformaciones basicas",
        nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    float vertices[] = {
        -0.5f,-0.5f, 0.5f, 1,0,0,
         0.5f,-0.5f, 0.5f, 1,0,0,
         0.5f, 0.5f, 0.5f, 1,0,0,
         0.5f, 0.5f, 0.5f, 1,0,0,
        -0.5f, 0.5f, 0.5f, 1,0,0,
        -0.5f,-0.5f, 0.5f, 1,0,0,

        -0.5f,-0.5f,-0.5f, 0,1,0,
         0.5f,-0.5f,-0.5f, 0,1,0,
         0.5f, 0.5f,-0.5f, 0,1,0,
         0.5f, 0.5f,-0.5f, 0,1,0,
        -0.5f, 0.5f,-0.5f, 0,1,0,
        -0.5f,-0.5f,-0.5f, 0,1,0,

         0.5f,-0.5f, 0.5f, 0,0,1,
         0.5f,-0.5f,-0.5f, 0,0,1,
         0.5f, 0.5f,-0.5f, 0,0,1,
         0.5f, 0.5f,-0.5f, 0,0,1,
         0.5f, 0.5f, 0.5f, 0,0,1,
         0.5f,-0.5f, 0.5f, 0,0,1,

        -0.5f, 0.5f, 0.5f, 1,1,0,
        -0.5f, 0.5f,-0.5f, 1,1,0,
        -0.5f,-0.5f,-0.5f, 1,1,0,
        -0.5f,-0.5f,-0.5f, 1,1,0,
        -0.5f,-0.5f, 0.5f, 1,1,0,
        -0.5f, 0.5f, 0.5f, 1,1,0,

        -0.5f,-0.5f,-0.5f, 0,1,1,
         0.5f,-0.5f,-0.5f, 0,1,1,
         0.5f,-0.5f, 0.5f, 0,1,1,
         0.5f,-0.5f, 0.5f, 0,1,1,
        -0.5f,-0.5f, 0.5f, 0,1,1,
        -0.5f,-0.5f,-0.5f, 0,1,1,

        -0.5f, 0.5f,-0.5f, 1,0.2f,0.5f,
         0.5f, 0.5f,-0.5f, 1,0.2f,0.5f,
         0.5f, 0.5f, 0.5f, 1,0.2f,0.5f,
         0.5f, 0.5f, 0.5f, 1,0.2f,0.5f,
        -0.5f, 0.5f, 0.5f, 1,0.2f,0.5f,
        -0.5f, 0.5f,-0.5f, 1,0.2f,0.5f
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)screenWidth / (float)screenHeight,
        0.1f,
        100.0f
    );

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 view = glm::translate(glm::mat4(1),
            glm::vec3(0, 0, -12));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint colorLoc = glGetUniformLocation(ourShader.Program, "tintColor");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        // CUBO 1 - ROJO
        glUniform3f(colorLoc, 1.0f, 0.4f, 0.4f);
        glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(-2, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // CUBO 2 - AZUL
        glUniform3f(colorLoc, 0.4f, 0.4f, 1.0f);
        model = glm::translate(glm::mat4(1), glm::vec3(2, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // CUBO 3 - VERDE
        glUniform3f(colorLoc, 0.4f, 1.0f, 0.4f);
        model = glm::translate(glm::mat4(1), glm::vec3(0, -2, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}