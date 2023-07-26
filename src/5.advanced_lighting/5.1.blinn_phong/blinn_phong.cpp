#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <openglapp/camera.h>
#include <openglapp/filesystem.h>
#include <openglapp/model.h>
#include <openglapp/shader.h>
#include <stb_image.h>

// GLM Libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const float screenWidth = 800;
const float screenHeight = 600;
bool blinn = true;

Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow *window, int w, int h)
{
    glViewport(0, 0, w, h);
}
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xoffset, double yoffset);
unsigned int loadTexture(char const *path);
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ========================
// color values
// ========================

glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

glm::vec3 lightPos(1.2f, 0.6f, -3.0f);

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
    // clang-format off
    float cubeVertices[] = {
        //cube pos           tex coords
        //x     y       z     x     y
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

     float planeVertices[] = {
        // positions            // normals         // texcoords
         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };
    // clang-format on

    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glBindVertexArray(0);

    // ==========
    // lightCubeVAO
    // ==========
    unsigned int lightCubeVAO, lightCubeVBO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &lightCubeVBO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int image;
    image = loadTexture(FileSystem::getPath("bin/resources/textures/wood.png").c_str());

    Shader lightingShader(FileSystem::getPath("bin/shaders/5.advanced_lighting/blinn_phong.vs").c_str(),
                          FileSystem::getPath("bin/shaders/5.advanced_lighting/blinn_phong.fs").c_str());

    Shader lightCubeShader(FileSystem::getPath("bin/shaders/5.advanced_lighting/light_cube.vs").c_str(),
                           FileSystem::getPath("bin/shaders/5.advanced_lighting/light_cube.fs").c_str());

    glEnable(GL_DEPTH_TEST);

    lightingShader.use();
    lightingShader.setInt("floorTexture", 0);

    camera.setMovementSpeed(15);
    while (!glfwWindowShouldClose(window))
    {
        // input
        // ----
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        // render
        // ----

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw objects
        lightingShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        // set light uniforms
        lightingShader.setVec3("viewPos", camera.getPosition());
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setInt("blinn", blinn);
        // floor
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, image);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        lightCubeShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::translate(model, lightPos);
        lightCubeShader.setMat4("model", model);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;
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
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (blinn == false)
            blinn = true;
        else
            blinn = false;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseScrool(xoffset, yoffset);
}
void mouse_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseMovement(xoffset, yoffset);
}
unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        format == GL_RGBA ? GL_CLAMP_TO_EDGE
                                          : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent
                                                        // borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
