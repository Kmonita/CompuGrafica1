//Practica2
// Fecha de entrega 20 de febrero 2026
//Monica Ixchel Urzua Cruz
//320110450

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);
const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Dino", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glfwSetFramebufferSizeCallback(window, resize);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // VERTICES (x, y, z, r, g, b)
    float vertices[] = {
        -0.85f,  0.10f, 0.0f,   0.1f, 0.7f, 0.1f, // 0: Cola punta
        -0.50f,  0.05f, 0.0f,   0.1f, 0.7f, 0.1f, // 1: Cola arriba
        -0.45f, -0.15f, 0.0f,   0.1f, 0.7f, 0.1f, // 2: Cola abajo
        -0.20f,  0.25f, 0.0f,   0.2f, 0.8f, 0.2f, // 3: Espalda
         0.15f,  0.15f, 0.0f,   0.2f, 0.8f, 0.2f, // 4: Hombro
         0.15f, -0.30f, 0.0f,   0.2f, 0.8f, 0.2f, // 5: Pecho
        -0.25f, -0.30f, 0.0f,   0.2f, 0.8f, 0.2f, // 6: Cadera
        -0.20f, -0.65f, 0.0f,   0.1f, 0.5f, 0.1f, // 7: Pie atras
         0.10f, -0.65f, 0.0f,   0.1f, 0.5f, 0.1f, // 8: Pie adelante
         0.25f,  0.45f, 0.0f,   0.2f, 0.8f, 0.2f, // 9: Cuello tras
         0.50f,  0.50f, 0.0f,   0.2f, 0.8f, 0.2f, // 10: Cabeza arriba
         0.75f,  0.50f, 0.0f,   0.2f, 0.8f, 0.2f, // 11: Hocico punta
         0.75f,  0.30f, 0.0f,   0.2f, 0.8f, 0.2f, // 12: Mandibula
         0.40f,  0.25f, 0.0f,   0.2f, 0.8f, 0.2f  // 13: Garganta
    };

    // Vértices para las rayas
    float lineVertices[] = {
        -0.85f,  0.10f, 0.0f,   0.0f, 0.0f, 0.0f, -0.50f,  0.05f, 0.0f,   0.0f, 0.0f, 0.0f,
        -0.45f, -0.15f, 0.0f,   0.0f, 0.0f, 0.0f, -0.20f,  0.25f, 0.0f,   0.0f, 0.0f, 0.0f,
         0.15f,  0.15f, 0.0f,   0.0f, 0.0f, 0.0f,  0.15f, -0.30f, 0.0f,   0.0f, 0.0f, 0.0f,
        -0.25f, -0.30f, 0.0f,   0.0f, 0.0f, 0.0f, -0.20f, -0.65f, 0.0f,   0.0f, 0.0f, 0.0f,
         0.10f, -0.65f, 0.0f,   0.0f, 0.0f, 0.0f,  0.25f,  0.45f, 0.0f,   0.0f, 0.0f, 0.0f,
         0.50f,  0.50f, 0.0f,   0.0f, 0.0f, 0.0f,  0.75f,  0.50f, 0.0f,   0.0f, 0.0f, 0.0f,
         0.75f,  0.30f, 0.0f,   0.0f, 0.0f, 0.0f,  0.40f,  0.25f, 0.0f,   0.0f, 0.0f, 0.0f
    };

    unsigned int fillIndices[] = {
        0, 1, 2,  1, 3, 6,  1, 6, 2,  3, 4, 5,  3, 5, 6,
        6, 5, 8,  6, 8, 7,  4, 9, 13,  4, 13, 5,  9, 10, 12,
        9, 12, 13,  10, 11, 12
    };

    // --- RAYAS INTERNAS Y EXTERNAS ---

    unsigned int lineIndices[] = {
        // Cola
        0,1, 1,2, 2,0,
        // Cuerpo y divisiones internas
        1,3, 3,6, 6,1, 6,2, 3,4, 4,5, 5,6,
        // Patas
        6,7, 7,8, 8,5,
        // Cuello y cabeza
        4,9, 9,13, 13,4, 13,5, 9,10, 10,12, 12,9, 12,13, 10,11, 11,12
    };

    GLuint VAO_F, VBO_F, EBO_F, VAO_L, VBO_L, EBO_L;

    // Config Relleno
    glGenVertexArrays(1, &VAO_F); glGenBuffers(1, &VBO_F); glGenBuffers(1, &EBO_F);
    glBindVertexArray(VAO_F);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_F);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_F);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fillIndices), fillIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Config Rayas
    glGenVertexArrays(1, &VAO_L); glGenBuffers(1, &VBO_L); glGenBuffers(1, &EBO_L);
    glBindVertexArray(VAO_L);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_L);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_L);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndices), lineIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();

        // 1. Relleno
        glBindVertexArray(VAO_F);
        glDrawElements(GL_TRIANGLES, sizeof(fillIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        // 2. RAYAS (Internas y externas)
        glLineWidth(2.5f);
        glBindVertexArray(VAO_L);
        glDrawElements(GL_LINES, sizeof(lineIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void resize(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

    