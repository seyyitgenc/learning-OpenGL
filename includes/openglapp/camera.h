#pragma once
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement { CAMERA_FORWARD, CAMERA_BACKWARD, CAMERA_RIGHT, CAMERA_LEFT };

// default parameters
static const float YAW = -90.0f;
static const float PITCH = 0.0f;
static const float ZOOM = 45.0f;
static const float SPEED = 2.5f;
static const float SENSIVITY = 0.1f;
class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
           float pitch = PITCH)
        : m_cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensivity(SENSIVITY), zoom(ZOOM) {
        m_cameraPosition = position;
        m_worldUp = up;
        this->m_yaw = yaw;
        this->m_pitch = pitch;
        updateCameraVectors();
    }
    ~Camera() = default;
    void processKeybord(Camera_Movement direction, float deltaTime) {
        float velocity = m_movementSpeed * deltaTime;
        if (direction == CAMERA_FORWARD) m_cameraPosition += m_cameraFront * velocity;
        if (direction == CAMERA_BACKWARD) m_cameraPosition -= m_cameraFront * velocity;
        if (direction == CAMERA_RIGHT) m_cameraPosition += m_cameraRight * velocity;
        if (direction == CAMERA_LEFT) m_cameraPosition -= m_cameraRight * velocity;
    }
    void processMouseMovement(double xpos, double ypos) {
        if (m_firstMouse) {
            lastX = xpos;
            lastY = ypos;
            m_firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;  // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        xoffset *= m_mouseSensivity;
        yoffset *= m_mouseSensivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;
        updateCameraVectors();
    }
    void processMouseScrool(double xoffset, double yoffset) {
        zoom -= (float)yoffset;
        if (zoom < 1.0f) zoom = 1.0f;
        if (zoom > 45.0f) zoom = 45.0f;
    }
    // void compute
    void setMovementSpeed(float movementSpeed) { m_movementSpeed = movementSpeed; }
    void setMouseSensivity(float mouseSensivity) { m_mouseSensivity = mouseSensivity; }
    float getMovementSpeed() { return m_movementSpeed; }
    float getMouseSensivity() { return m_mouseSensivity; }

    glm::mat4 getViewMatrix() { return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp); }

private:
    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        direction.y = sin(glm::radians(m_pitch));
        direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

        m_cameraFront = glm::normalize(direction);
        m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_worldUp));
        m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
    }
    // camera attributes
private:
    bool m_firstMouse = true;
    float lastX, lastY;
    glm::vec3 m_cameraPosition;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;
    glm::vec3 m_cameraRight;
    glm::vec3 m_worldUp;
    // euler agnles
    float m_pitch, m_yaw;
    // camera options
    float m_mouseSensivity;
    float m_movementSpeed;

public:
    float zoom;
};