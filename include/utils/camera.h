/*
Camera class
- creation of a reference system for the camera
- management of camera movement (FPS-style) using WASD keys and mouse movement


N.B.) adaptation of https://github.com/JoeyDeVries/LearnOpenGL/blob/master/includes/learnopengl/camera.h

author: Davide Gadia

Real-time Graphics Programming - a.a. 2017/2018
Master degree in Computer Science
Universita' degli Studi di Milano
*/

#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <glad/glad.h> // Contains all the necessery OpenGL includes
// we use GLM to create the view matrix and to manage camera transformations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// possible camera movements
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera settings
// Initial camera orientation on Y and X (Z not considered)
const GLfloat YAW        = -90.0f; //Y
const GLfloat PITCH      =  0.0f; //X

// parameters to manage mouse movement
const GLfloat SPEED      =  30.0f;
const GLfloat SENSITIVTY =  0.25f;
const GLfloat RUN_BOOST	 =	10.0f;

///////////////////  CAMERA class ///////////////////////
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 WorldFront;
    glm::vec3 Up; // camera local UP vector
    glm::vec3 Right;
    glm::vec3 WorldUp; //  camera world UP vector -> needed for the initial computation of Right vector
    GLboolean onGround; // it defines if the camera is "anchored" to the ground, or if it strictly follows the current Front direction (even if this means that the camera "flies" in the scene)
    // N.B.) this version works only for flat terrains
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
	GLboolean Running = GL_FALSE;

    //////////////////////////////////////////
    // full constructor with member initializer lists
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    GLfloat yaw = YAW, GLfloat pitch = PITCH, GLboolean onGround = GL_TRUE) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->onGround = onGround;
        // initialization of the camera reference system using the vectors passed to the constructor
        this->updateCameraVectors();
    }

    // simplified constructor with member initializer lists
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), GLboolean onGround = GL_TRUE) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY)
    {
        this->Position = position;
        this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        this->Yaw = YAW;
        this->Pitch = PITCH;
        this->onGround = onGround;
        // initialization of the camera reference system using the vectors passed to the constructor
        this->updateCameraVectors();
    }


    //////////////////////////////////////////
    // it returns the current view matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
    }

    //////////////////////////////////////////
    // it updates camera position when a WASD key is pressed
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->MovementSpeed * deltaTime;
		if (this->Running) velocity *= RUN_BOOST;
        if (direction == FORWARD)
            this->Position += (this->onGround ? this->WorldFront : this->Front) * velocity;
        if (direction == BACKWARD)
            this->Position -= (this->onGround ? this->WorldFront : this->Front) * velocity;
        if (direction == LEFT)
            this->Position -= this->Right * velocity;
        if (direction == RIGHT)
            this->Position += this->Right * velocity;
        if(direction == UP)
            this->Position += this->Up * velocity;
        if(direction == DOWN)
            this->Position -= this->Up * velocity;
    }

    //////////////////////////////////////////
    // Sulla base del movimento del mouse, aggiorna l'orientamento della camera
    // it updates camera orientation when mouse is moved
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
    {
        // the sensitivity is applied to weight the movement
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        // rotation angles on Y and X are updated
        this->Yaw   += xoffset;
        this->Pitch += yoffset;

        // we apply a constraint to the rotation on X, to avoid to have the camera flipped upside down
        // N.B.) this constraint helps to avoid gimbal lock, if all the 3 rotations are considered
        if (constrainPitch)
        {
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }

        // the camera reference system is updated using the new camera rotations
        this->updateCameraVectors();
    }

private:
    //////////////////////////////////////////
    // Aggiorna il sistema di riferimento della camera
    // it updates the camera reference system
    void updateCameraVectors()
    {
        // it computes the new Front vector using trigonometric calculations using Yaw an Pitch angles
        // https://learnopengl.com/#!Getting-started/Camera
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->WorldFront = this->Front = glm::normalize(front);
        // if the camera is "anchored" to the ground, the world Front vector is equal to the local Front vector, but with the y component = 0
        this->WorldFront.y = 0.0f;
        // Once calculated the new view direction, we re-calculate the Right vector as cross product between Front and world Up vector
        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
        // we calculate the camera local Up vector as cross product between Front and Right vectors
        this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
    }
};
