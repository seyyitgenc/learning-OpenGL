#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <openglapp/camera.h>
#include <openglapp/shader.h>
#include <stb_image.h>

// GLM Libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const float screenWidth = 800;
const float screenHeight = 600;
float blend = 0.2;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow *window, int w, int h) { glViewport(0, 0, w, h); }
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xoffset, double yoffset);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int main(int argc, char **argv) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Hello OpenGL", NULL, NULL);
    if (window == NULL) {
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return EXIT_FAILURE;
    }

    //* now there is no overlap
    float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
                        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
    unsigned int indices[] = {
        0, 1, 3,  // first texture
        1, 2, 3   // second texture
    };
    glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),     glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f),
                                 glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f),
                                 glm::vec3(1.3f, -2.0f, -2.5f),   glm::vec3(1.5f, 2.0f, -2.5f),  glm::vec3(1.5f, 0.2f, -1.5f),
                                 glm::vec3(-1.3f, 1.0f, -1.5f)};

    // if we use GL_CLAMP_TO_BORDER we must use border color
    // ------- set Gl texture parameters
    unsigned int container, smileyface;
    glGenTextures(1, &container);
    glBindTexture(GL_TEXTURE_2D, container);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    glGenTextures(1, &smileyface);
    glBindTexture(GL_TEXTURE_2D, smileyface);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, smileyface);
    // note: VAO's attr. pointer itearates over VBO's value
    unsigned int VBO, VAO, EBO;  // VBO(Vertex Buffer Object), VAO(Vertex Array Object),EBO(Element Buffer Object)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the vertex array object first, then bind and set vertex buffer(s), and then configure vertex attribute(s)
    glBindVertexArray(VAO);
    // VBO
    //---------
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    //---------
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // note: this VAO stores glVertexAttribPointer and glEnableVertexAttribArray and assoicated object with it aka VBO and EBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // OpenGl wireframe mode by default it's GL_FILL
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader texture("shaders/1.getting_started/7.1.coordinates.vs", "shaders/1.getting_started/7.1.coordinates.fs");
    // main loop
    texture.use();
    texture.setInt("texture1", 0);
    texture.setInt("texture2", 1);
    texture.setFloat("blend", blend);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        // input
        // ----
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        // render
        // ----
        texture.use();

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            if (blend >= 1.0f) blend = 1.0f;
            blend += 0.0001f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            if (blend <= 0.0f) blend = 0.0f;

            blend -= 0.0001f;
        }

        glUniform1f(glGetUniformLocation(texture.ID, "blend"), blend);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), screenWidth / screenHeight, 0.1f, 100.0f);
        texture.setMat4("projection", projection);
        glm::mat4 view = camera.getViewMatrix();
        texture.setMat4("view", view);

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 1.0f));
            texture.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.processKeyboard(CAMERA_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.processKeyboard(CAMERA_BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.processKeyboard(CAMERA_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.processKeyboard(CAMERA_RIGHT, deltaTime);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) { camera.processMouseScrool(xoffset, yoffset); }
void mouse_callback(GLFWwindow *window, double xoffset, double yoffset) { camera.processMouseMovement(xoffset, yoffset); }
