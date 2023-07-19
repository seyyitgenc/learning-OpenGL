#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <openglapp/camera.h>
#include <openglapp/filesystem.h>
#include <openglapp/mesh.h>
#include <openglapp/model.h>
#include <openglapp/shader.h>

// GLM Libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const float screenWidth = 800;
const float screenHeight = 600;

Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow *window, int w, int h)
{
    glViewport(0, 0, w, h);
}
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xoffset, double yoffset);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ----------------
// main function
// ----------------
int main(int argc, char **argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Hello OpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    stbi_set_flip_vertically_on_load(true);
    // callback initalization
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initializes pointers for some OPENGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return EXIT_FAILURE;
    }
    glEnable(GL_DEPTH_TEST);
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
        0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
        0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
        0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f,
    };

    unsigned int VBO, lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &VBO);

    // VBO
    glBindVertexArray(lightCubeVAO);
    //---------
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader ourShader("shaders/3.model_loading/1.model_loading.vs", "shaders/3.model_loading/1.model_loading.fs");
    Shader lightCubeShader("shaders/2.lighting/light_cube.vs", "shaders/2.lighting/light_cube.fs");
    Shader shaderSingleColor("shaders/3.model_loading/1.model_loading.vs", "shaders/4.advanced_opengl/1.1.stencil_border.fs");

    Model backPack(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_STENCIL_TEST);
    while (!glfwWindowShouldClose(window))
    {
        // input
        // ----
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilMask(0x00);

        ourShader.use();
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourShader.setVec3("viewPos", camera.getPosition());
        ourShader.setFloat("shininess", 32);

        ourShader.setVec3("light.position", glm::vec3(-3.0f, 1.0f, 2.0f));
        glm::vec3 diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        ourShader.setVec3("light.ambient", ambientColor);
        ourShader.setVec3("light.diffuse", diffuseColor);
        ourShader.setVec3("light.specular", glm::vec3(1));
        ourShader.setFloat("light.constant", 1.0f);
        ourShader.setFloat("light.linear", 0.009f);
        ourShader.setFloat("light.quadratic", 0.032f);
        backPack.Draw(ourShader);

        // borders
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        shaderSingleColor.use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.01f, 1.01f, 1.01f));  // it's a bit too big for our scene, so scale it down
        shaderSingleColor.setMat4("projection", projection);
        shaderSingleColor.setMat4("view", view);

        shaderSingleColor.setMat4("model", model);
        shaderSingleColor.setVec3("viewPos", camera.getPosition());
        shaderSingleColor.setFloat("shininess", 32);

        shaderSingleColor.setVec3("light.position", glm::vec3(-3.0f, 1.0f, 2.0f));
        shaderSingleColor.setVec3("light.ambient", ambientColor);
        shaderSingleColor.setVec3("light.diffuse", diffuseColor);
        shaderSingleColor.setVec3("light.specular", glm::vec3(1));
        shaderSingleColor.setFloat("light.constant", 1.0f);
        shaderSingleColor.setFloat("light.linear", 0.009f);
        shaderSingleColor.setFloat("light.quadratic", 0.032f);
        backPack.Draw(shaderSingleColor);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CAMERA_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CAMERA_BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CAMERA_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CAMERA_RIGHT, deltaTime);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseScrool(xoffset, yoffset);
}
void mouse_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseMovement(xoffset, yoffset);
}
