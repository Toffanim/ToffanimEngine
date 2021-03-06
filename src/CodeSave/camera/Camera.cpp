// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
   $File: $
   $Date: $
   $Modified: 06-02-2017 07h50m20s $
   $Creator:  $
   $Notice:  $
   ======================================================================== */
#include "Camera.h"
//Camera class, pretty straightforward
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw , GLfloat pitch ) : position(position), up(up), yaw(yaw), pitch(pitch)
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    zoom = 45.0f;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix(glm::vec3 focus_position, glm::vec3 up_bias)
{
	return glm::lookAt(position, focus_position, glm::normalize(up + up_bias));
}

glm::mat4 Camera::getViewMatrix( glm::vec3 focus_position )
{
    return glm::lookAt(position, focus_position, up);
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getRotatedViewMatrix()
{
    glm::vec3 rotPos = glm::vec3( position.x, -position.y, position.z);
    glm::vec3 rotFront = glm::vec3( front.x , -front.y, front.z);
    return glm::lookAt(  rotPos, rotPos + rotFront, up);
}

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
}

void Camera::addYaw(float value)
{
    yaw += value;
    updateCameraVectors();
}


void Camera::addPitch( float value )
{
    pitch += value;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    updateCameraVectors();
}
