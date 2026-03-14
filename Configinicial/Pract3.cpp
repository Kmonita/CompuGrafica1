// Practica 3
// Urzua Cruz Monica Ixchel
// Fecha de entrega: 27/02/2026
// 320110450
#include <iostream>
#include <cfloat> // FLT_MAX
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 1200, HEIGHT = 800;
float movX = 0.0f, movY = 0.0f, movZ = -11.0f, rot = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 5 - Ixchel ", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return -1;

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Vértices del cubo (posiciones + normales)
    float vertices[] = {
        // Cara Trasera
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        // Cara Frontal
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        // Cara Izquierda
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        // Cara Derecha
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

         // Cara Inferior
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

         // Cara Superior
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    // Escalas (6 cubos: I X C H E L)
    glm::vec3 scales[6] = {
        {2.2f, 0.9f, 2.2f},
        {1.9f, 0.85f, 1.9f},
        {1.6f, 0.80f, 1.6f},
        {1.3f, 0.75f, 1.3f},
        {1.0f, 0.70f, 1.0f},
        {0.7f, 0.65f, 0.7f}
    };

    // Colores
    glm::vec3 colors[6] = {
        {0.9f, 0.3f, 0.3f},
        {0.3f, 0.9f, 0.3f},
        {0.3f, 0.6f, 0.9f},
        {0.9f, 0.9f, 0.3f},
        {0.9f, 0.3f, 0.9f},
        {0.3f, 0.9f, 0.9f}
    };

    // =========================
    // offsets (X,Z) para que no sea una pirámide recta
  
    glm::vec2 offsetsXZ[6] = {
        { 0.00f,  0.00f},  // base
        { 0.35f,  0.10f},
        {-0.30f,  0.20f},
        { 0.25f, -0.25f},
        {-0.20f,  0.15f},
        { 0.00f,  0.00f}   
    };

    //  rotación en Y por cubo (orientaciones distintas)
    float rotYdeg[6] = { 0.0f, 18.0f, -22.0f, 28.0f, -16.0f, 45.0f };
    // =========================

    // Apilado en Y (centros)
    float currentYOffset = 0.0f;
    glm::vec3 positions[6];
    for (int i = 0; i < 6; i++) {
        positions[i] = glm::vec3(0.0f, currentYOffset + (scales[i].y / 2.0f), 0.0f);
        currentYOffset += scales[i].y;
    }

    float towerYAdjustment = -3.0f;

    // Rotación 
    const float topRotY = 45.0f;
    const float topRotX = 35.264f; // hace que una esquina “apunte” hacia abajo
    const float topRotZ = 0.0f;

    // Esquinas del cubo unitario
    const glm::vec3 corners[8] = {
        {-0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f,-0.5f},
        {-0.5f, 0.5f,-0.5f}, { 0.5f, 0.5f,-0.5f},
        {-0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f}
    };

    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        glfwPollEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        // Cámara
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        GLint colorLoc = glGetUniformLocation(ourShader.Program, "ourColor");
        glBindVertexArray(VAO);

        for (int i = 0; i < 6; i++) {
            glm::mat4 model(1.0f);

            // base del cubo i, pero con offset en X/Z
            glm::vec3 basePos = positions[i] + glm::vec3(offsetsXZ[i].x, 0.0f, offsetsXZ[i].y)
                + glm::vec3(0.0f, towerYAdjustment, 0.0f);

            if (i == 5) {
          
                // El centro objetivo NO es (0,0,0): es el offset del cubo de abajo (i-1)
                glm::vec3 belowCenter = positions[i - 1]
                    + glm::vec3(offsetsXZ[i - 1].x, 0.0f, offsetsXZ[i - 1].y)
                    + glm::vec3(0.0f, towerYAdjustment, 0.0f);

                float belowTopY = belowCenter.y + (scales[i - 1].y / 2.0f);
                glm::vec3 targetPoint(belowCenter.x, belowTopY, belowCenter.z);

                // Construir RS
                glm::mat4 RS(1.0f);
                RS = glm::rotate(RS, glm::radians(topRotY), glm::vec3(0.0f, 1.0f, 0.0f));
                RS = glm::rotate(RS, glm::radians(topRotX), glm::vec3(1.0f, 0.0f, 0.0f));
                RS = glm::rotate(RS, glm::radians(topRotZ), glm::vec3(0.0f, 0.0f, 1.0f));
                RS = glm::scale(RS, scales[i]);

                float minY = FLT_MAX;
                glm::vec3 lowestCorner(0.0f);

                for (int c = 0; c < 8; c++) {
                    glm::vec4 p = RS * glm::vec4(corners[c], 1.0f);
                    if (p.y < minY) {
                        minY = p.y;
                        lowestCorner = glm::vec3(p);
                    }
                }

                // Traslación para que esa esquina caiga en targetPoint
                glm::vec3 fixedPos = targetPoint - lowestCorner;

                model = glm::translate(model, fixedPos);
                model = glm::rotate(model, glm::radians(topRotY), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(topRotX), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(topRotZ), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, scales[i]);
            }
            else {
                // ===== Cubos normales: distintos offsets + distinta orientación =====
                model = glm::translate(model, basePos);
                model = glm::rotate(model, glm::radians(rotYdeg[i]), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, scales[i]);
            }

            glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniform3fv(colorLoc, 1, glm::value_ptr(colors[i]));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movX += speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movX -= speed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) movY += speed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) movY -= speed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movZ -= speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movZ += speed;

    float rotSpeed = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rot += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rot -= rotSpeed;
}