#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <openglapp/shader.h>
#include <stb_image.h>

#include <cmath>
#include <filesystem>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int w, int h) { glViewport(0, 0, w, h); }

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

int main(int argc, char **argv) {
    // getting absoulte file path

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello OpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // initializes pointers for some OPENGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return EXIT_FAILURE;
    }

    //* now there is no overlap
    float vertices[] = {
        // positions        // colors         // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };
    // if we use GL_CLAMP_TO_BORDER we must use border color
    // ------- set Gl texture parameters
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float bordercolor[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);  // unbind texture
    stbi_image_free(data);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // OpenGl wireframe mode by default it's GL_FILL
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // std::cout << "max nr of vertex attrib. supported : " << nrAttributes << std::endl;

    // zort

    //! FIXME :  path is changing due to relative path
    // note use filesystem library 
    // !! IF USER EXECUTING EXE FILE FROM BUILD DIRECTORY THIS WILL BE THE PATH
    // !! BUT IF USER EXECUTING EXE FILE FROM WHERE EXE IS LOCATED PATH WILL BE ../shaders/4.1.texture.vs LATER ON I WILL FIX IT 
    Shader triangle("shaders/1.getting_started/4.1.texture.vs", "shaders/1.getting_started/4.1.texture.fs");
    // main loop
    while (!glfwWindowShouldClose(window)) {
        // input
        // ----
        processInput(window);

        // render
        // ----

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        triangle.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glBindVertexArray(VAO);  // seeing as we only have a single VAO there's no need to bind it every time
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}