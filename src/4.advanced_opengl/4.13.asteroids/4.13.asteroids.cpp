#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <openglapp/camera.h>
#include <openglapp/filesystem.h>
#include <openglapp/model.h>
#include <openglapp/shader.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
double stepAngle = 0.0;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool pause = false, isPausePressed = false;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    Shader shader(FileSystem::getPath("bin/shaders/4.advanced_opengl/asteroids.vs").c_str(),
                  FileSystem::getPath("bin/shaders/4.advanced_opengl/asteroids.fs").c_str());

    Model asteroid(FileSystem::getPath("resources/objects/rock/rock.obj"));
    Model planet(FileSystem::getPath("resources/objects/planet/planet.obj"));

    camera.setMovementSpeed(30);
    // render loop
    // -----------

    unsigned int amount = 1000;
    glm::mat4 *modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime());
    float radius = 50.0;
    float offset = 2.5f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);

        // 1. translation: displace along circle with radius [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller than x/z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)random rotation axis
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }
    float time = 0;
    camera.setPosition(glm::vec3(-10, 0, 0));
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        time += deltaTime;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)800 / (float)600, 0.1f, 400.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // planet
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        model = glm::translate(model, glm::vec3(0.0, -3.0, 0.0));
        model = glm::scale(model, glm::vec3(3.0, 3.0, 3.0));
        shader.setMat4("model", model);
        planet.Draw(shader);

        // asteroids
        for (size_t i = 0; i < amount; i++)
        {
            if (!pause)
            {
                float test = (2 * 3.14 * 1 * stepAngle) / 360;
                float originX = 0, originY = 0;
                float x = originX + cos(stepAngle) * 0.1;
                float y = originY + sin(stepAngle) * 0.1;
                /* code */
                modelMatrices[i] = glm::translate(modelMatrices[i], glm::vec3(test, 0, test));
            }
            shader.setMat4("model", modelMatrices[i]);
            asteroid.Draw(shader);
        }
        if (time > 2)
        {
            stepAngle += 1;
            time = 0;
            if (stepAngle > 360)
                stepAngle = 0;
            if (stepAngle < 0)
                stepAngle = 0;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete modelMatrices;
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !isPausePressed)
    {
        pause = !pause;
        isPausePressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        isPausePressed = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseScrool(xoffset, yoffset);
}
void mouse_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseMovement(xoffset, yoffset);
}
