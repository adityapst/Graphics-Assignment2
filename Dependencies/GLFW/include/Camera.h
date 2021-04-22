#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

/// <summary>
/// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
/// </summary>
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    LROLL,
    RROLL
};

// Default camera values

/// <summary>
/// Default camera yaw
/// </summary>
const float YAW = -90.0f;
/// <summary>
/// Default camera pitch
/// </summary>
const float PITCH = 0.0f;
/// <summary>
/// Default camera roll angle
/// </summary>
const float ROLL = 0.0f;
/// <summary>
/// Default camera movement speed
/// </summary>
const float SPEED = 1.0f;
/// <summary>
/// Default camera mouse sensitivity
/// </summary>
const float SENSITIVITY = 0.1f;
/// <summary>
/// Default camera roll angle delta
/// </summary>
const float ROLL_SENSITIVITY = 0.5f;
/// <summary>
/// Default camera zoom
/// </summary>
const float ZOOM = 45.0f;

/// <summary>
/// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
/// </summary>
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;
    float Roll;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    /// <summary>
    /// constructor with vectors
    /// </summary>
    /// <param name="position"></param>
    /// <param name="up"></param>
    /// <param name="yaw"></param>
    /// <param name="pitch"></param>
    /// <param name="roll"></param>
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float roll = ROLL) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Up = up;
        Yaw = yaw;
        Pitch = pitch;
        Roll = roll;
        updateCameraVectors();
    }
    /// <summary>
    /// constructor with scalar values
    /// </summary>
    /// <param name="posX"></param>
    /// <param name="posY"></param>
    /// <param name="posZ"></param>
    /// <param name="upX"></param>
    /// <param name="upY"></param>
    /// <param name="upZ"></param>
    /// <param name="yaw"></param>
    /// <param name="pitch"></param>
    /// <param name="roll"></param>
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float roll) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        Roll = roll;
        updateCameraVectors();
    }

    /// <summary>
    /// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    /// </summary>
    /// <returns>View matrix</returns>
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    /// <summary>
    /// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    /// </summary>
    /// <param name="direction">Movement key direction pressed on keyboard</param>
    /// <param name="deltaTime">Time difference between consecutive frames</param>
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        Roll = 0.0f;
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == LROLL)
        {
            Roll -= ROLL_SENSITIVITY;
            updateCameraRollVectors();
        }
        if (direction == RROLL)
        {
            Roll += ROLL_SENSITIVITY;
            updateCameraRollVectors();
        }
    }

    /// <summary>
    /// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    /// </summary>
    /// <param name="xoffset">Mouse xoffset</param>
    /// <param name="yoffset">Moust yoffset</param>
    /// <param name="constrainPitch">True, if -90 < pitch < 90</param>
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    /// <summary>
    /// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    /// </summary>
    /// <param name="yoffset">Mouse wheel scroll parameter</param>
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    /// <summary>
    /// calculates the front vector from the Camera's (updated) Euler Angles after change in the pitch or yaw
    /// </summary>
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, Up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement
    }

    /// <summary>
    /// calculates the up and right vector from the Camera's (updated) Euler Angles after rolling
    /// </summary>
    void updateCameraRollVectors()
    {
        // calculates the new Up and Right vectors
        glm::mat4 roll_mat = glm::rotate(glm::mat4(1.0f), glm::radians(Roll), Front);
        Up = glm::normalize(glm::mat3(roll_mat) * Up);
        Right = glm::normalize(glm::cross(Front, Up));
    }
};
#endif