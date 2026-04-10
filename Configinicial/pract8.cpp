//Practica 8
//Urzua Cruz Monica Ixchel
//320110450
//Fecha de entrega 10/04/2026
#include <string>
#include <iostream> // Añadido para std::cout

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); // Ajustada posición inicial
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

float angDia = 0.0f;
float angNoche = glm::radians(180.0f);
float diaV = 1.5f;
float nocheV = 1.5f;
float diaR = 18.5f;
float nocheR = 18.5f;
float verticalScale = 1.0f;
glm::vec3 orbitCenter(0.0f, 0.0f, 0.0f);

// Light attributes
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool dia = true;
float tiltNightDeg = 90.0f;
float nightRadiusFactor = 1.5f;
glm::vec3 tiltAxis = glm::vec3(0.0f, 0.0f, 1.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica8 Ixchel Urzua", nullptr, nullptr);
    if (nullptr == window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) return EXIT_FAILURE;

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");

    Model Table((char*)"Models/Table.obj");
    Model Cloth((char*)"Models/Cloth.obj");
    Model Pizza((char*)"Models/Pizza.obj");
    Model Lamp((char*)"Models/Lamp.obj");
    Model Floor((char*)"Models/Floor.obj");
    Model sun((char*)"Models/sun.obj");
    Model moon((char*)"Models/moon.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        angDia = fmod(angDia + diaV * deltaTime, glm::two_pi<float>());
        angNoche = fmod(angNoche + nocheV * deltaTime, glm::two_pi<float>());

        glfwPollEvents();
        DoMovement();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "numLights"), 1);

        // Lógica de posiciones de luz
        glm::vec3 L1 = orbitCenter + glm::vec3(diaR * cos(angDia), diaR * sin(angDia), 15.0f);
        glm::mat4 Rnight = glm::rotate(glm::mat4(1.0f), glm::radians(tiltNightDeg), glm::normalize(tiltAxis));
        glm::vec4 vNight = Rnight * glm::vec4((nocheR * nightRadiusFactor) * cos(angNoche), verticalScale * (nocheR * nightRadiusFactor) * sin(angNoche), (nocheR * nightRadiusFactor) * sin(angNoche), 1.0f);
        glm::vec3 L2 = (orbitCenter + glm::vec3(-10.0f, 0.0f, 0.0f)) + glm::vec3(vNight);

        if (dia) {
            glUniform3fv(glGetUniformLocation(lightingShader.Program, "lights[0].position"), 1, glm::value_ptr(L1));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].ambient"), 0.30f, 0.28f, 0.25f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].diffuse"), 1.20f, 1.10f, 1.00f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].specular"), 1.00f, 0.90f, 0.80f);
        }
        else {
            glUniform3fv(glGetUniformLocation(lightingShader.Program, "lights[0].position"), 1, glm::value_ptr(L2));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].ambient"), 0.05f, 0.10f, 0.20f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].diffuse"), 0.30f, 0.50f, 1.00f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].specular"), 0.40f, 0.60f, 1.00f);
        }

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Material genérico
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.1f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // Dibujar modelos
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        Table.Draw(lightingShader); // CORRECCIÓN
        Cloth.Draw(lightingShader);
        Pizza.Draw(lightingShader);
        Lamp.Draw(lightingShader);
        Floor.Draw(lightingShader);

        // Astros
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        if (dia) {
            model = glm::translate(glm::mat4(1.0f), L1);
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            sun.Draw(lampshader);
        }
        else {
            model = glm::translate(glm::mat4(1.0f), L2);
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            moon.Draw(lampshader);
        }

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

void DoMovement() {
    if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
    if (key == GLFW_KEY_M && action == GLFW_PRESS) dia = !dia;

    // Cambios de velocidad 
    if (keys[GLFW_KEY_O]) diaV += 0.1f;
    if (keys[GLFW_KEY_L]) diaV = glm::max(0.0f, diaV - 0.1f);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos; lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}