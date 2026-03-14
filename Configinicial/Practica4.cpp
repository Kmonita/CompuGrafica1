//Práctica4
//Urzua Cruz Monica Ixchel
//320110450
//Fecha de entrega 02 de marzo 2026 


#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -6.5f;   
float rot = 0.0f;

static inline void DrawCube(GLint modelLoc, GLint colorLoc,
    const glm::vec3& pos,
    const glm::vec3& scl,
    const glm::vec3& color)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scl);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Gato - Ixchel p4", nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window)
    {
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

    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Vertices para un cubo genérico (36 vértices / 12 triángulos)
    float vertices[] = {
        // Front
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        // Back
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // Right
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,

         // Left
         -0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,

         // Bottom
         -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f, -0.5f,

         // Top
         -0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f, -0.5f,
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posición (solo vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (GLfloat)screenWidth / (GLfloat)screenHeight,
        0.1f, 100.0f
    );

    // Colores del gato
    glm::vec3 catGray(0.65f, 0.70f, 0.78f);
    glm::vec3 catGrayDark(0.45f, 0.50f, 0.60f);
    glm::vec3 eyeYellow(1.0f, 0.85f, 0.10f);
    glm::vec3 eyeBlack(0.05f, 0.05f, 0.05f);
    glm::vec3 toothWhite(0.95f, 0.95f, 0.95f);
    glm::vec3 nosePink(0.90f, 0.30f, 0.45f);

    while (!glfwWindowShouldClose(window))
    {
        Inputs(window);
        glfwPollEvents();

        // Fondo 
        glClearColor(0.02f, 0.04f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint colorLoc = glGetUniformLocation(ourShader.Program, "inColor");

        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // ======================= GATO =======================

        // Cuerpo principal
        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.4f, 1.0f, 1.0f),
            catGray);

        // Cabeza (frente)
        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.0f, 0.15f, 0.80f),
            glm::vec3(1.0f, 0.85f, 0.55f),
            catGray);

        // Orejas
        DrawCube(modelLoc, colorLoc,
            glm::vec3(-0.35f, 0.65f, 0.80f),
            glm::vec3(0.25f, 0.25f, 0.25f),
            catGray);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.35f, 0.65f, 0.80f),
            glm::vec3(0.25f, 0.25f, 0.25f),
            catGray);

        // Bloque superior (detalle/lomo)
        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.20f, 0.65f, 0.15f),
            glm::vec3(0.90f, 0.25f, 0.45f),
            catGrayDark);

        // Cola (barra)
        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.85f, 0.25f, -0.40f),
            glm::vec3(0.25f, 0.25f, 0.90f),
            catGrayDark);

        // Patas (4 bloques)
        DrawCube(modelLoc, colorLoc,
            glm::vec3(-0.45f, -0.55f, 0.35f),
            glm::vec3(0.30f, 0.25f, 0.35f),
            catGrayDark);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.45f, -0.55f, 0.35f),
            glm::vec3(0.30f, 0.25f, 0.35f),
            catGrayDark);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(-0.45f, -0.55f, -0.35f),
            glm::vec3(0.30f, 0.25f, 0.35f),
            catGrayDark);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.45f, -0.55f, -0.35f),
            glm::vec3(0.30f, 0.25f, 0.35f),
            catGrayDark);

        // Ojos (amarillo)
        DrawCube(modelLoc, colorLoc,
            glm::vec3(-0.25f, 0.25f, 1.10f),
            glm::vec3(0.30f, 0.30f, 0.08f),
            eyeYellow);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.25f, 0.25f, 1.10f),
            glm::vec3(0.30f, 0.30f, 0.08f),
            eyeYellow);

        // Pupilas
        DrawCube(modelLoc, colorLoc,
            glm::vec3(-0.25f, 0.25f, 1.16f),
            glm::vec3(0.14f, 0.14f, 0.05f),
            eyeBlack);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.25f, 0.25f, 1.16f),
            glm::vec3(0.14f, 0.14f, 0.05f),
            eyeBlack);

        // Brillos
        DrawCube(modelLoc, colorLoc,
            glm::vec3(-0.18f, 0.32f, 1.18f),
            glm::vec3(0.05f, 0.05f, 0.03f),
            toothWhite);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.32f, 0.32f, 1.18f),
            glm::vec3(0.05f, 0.05f, 0.03f),
            toothWhite);

        // Hocico blanco
        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.0f, -0.05f, 1.12f),
            glm::vec3(0.75f, 0.35f, 0.10f),
            toothWhite);

        // Dientes (dos cubitos)
        DrawCube(modelLoc, colorLoc,
            glm::vec3(-0.18f, -0.20f, 1.16f),
            glm::vec3(0.20f, 0.20f, 0.08f),
            toothWhite);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.18f, -0.20f, 1.16f),
            glm::vec3(0.20f, 0.20f, 0.08f),
            toothWhite);

        // Nariz rosa
        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.0f, 0.02f, 1.18f),
            glm::vec3(0.12f, 0.10f, 0.06f),
            nosePink);

        // Bigotes (4 barras)
        DrawCube(modelLoc, colorLoc,
            glm::vec3(-0.55f, -0.02f, 1.12f),
            glm::vec3(0.30f, 0.05f, 0.05f),
            catGrayDark);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(-0.55f, -0.12f, 1.12f),
            glm::vec3(0.30f, 0.05f, 0.05f),
            catGrayDark);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.55f, -0.02f, 1.12f),
            glm::vec3(0.30f, 0.05f, 0.05f),
            catGrayDark);

        DrawCube(modelLoc, colorLoc,
            glm::vec3(0.55f, -0.12f, 1.12f),
            glm::vec3(0.30f, 0.05f, 0.05f),
            catGrayDark);

        // ==========================================================

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movX += 0.0008f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movX -= 0.0008f;

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        movY += 0.0008f;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        movY -= 0.0006f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movZ -= 0.0006f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movZ += 0.008f;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rot += 0.04f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rot -= 0.04f;
}