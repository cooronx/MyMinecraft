#include "camera.h"

QMatrix4x4 Camera::getViewMatrix()const
{
  QMatrix4x4 view;
  view.lookAt(this->position, this->position + this->front, this->up);
  return view;
}

void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
  GLfloat velocity = this->movementSpeed * deltaTime;
  if (direction == FORWARD)
    this->position += this->front * velocity;
  if (direction == BACKWARD)
    this->position -= this->front * velocity;
  if (direction == LEFT)
    this->position -= this->right * velocity;
  if (direction == RIGHT)
    this->position += this->right * velocity;
  if (direction == UP)
    this->position += this->worldUp * velocity;
  if (direction == DOWN)
    this->position -= this->worldUp * velocity;
}

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch)
{
  xoffset *= this->mouseSensitivity;
  yoffset *= this->mouseSensitivity;

  this->yaw += xoffset;
  this->pitch += yoffset;

  if(this->pitch > 89.0f)                          //将俯视角限制到[-90°,90°]
      pitch =  89.0f;
  if(pitch < -89.0f)
      pitch = -89.0f;
  this->updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yoffset)
{
  if (this->zoom >= 1.0f && this->zoom <= 45.0f)
    this->zoom -= yoffset;
  if (this->zoom > 45.0f)
    this->zoom = 45.0f;
  if (this->zoom < 1.0f)
      this->zoom = 1.0f;
}

void Camera::processInput(GLfloat dt)
{

    if (keys[Qt::Key_W])
      processKeyboard(FORWARD, dt);
    if (keys[Qt::Key_S])
      processKeyboard(BACKWARD, dt);
    if (keys[Qt::Key_A])
      processKeyboard(LEFT, dt);
    if (keys[Qt::Key_D])
      processKeyboard(RIGHT, dt);
    if (keys[Qt::Key_Space])
      processKeyboard(UP, dt);
    if (keys[Qt::Key_Shift])
      processKeyboard(DOWN, dt);
}

void Camera::updateCameraVectors()
{
    GLfloat yawR = qDegreesToRadians(this->yaw);
    GLfloat picthR = qDegreesToRadians(this->pitch);//转换为弧度制Radians

    QVector3D front3(cos(yawR) * cos(picthR), sin(picthR), sin(yawR) * cos(picthR));
    this->front = front3.normalized();
    this->right = QVector3D::crossProduct(this->front, this->worldUp).normalized();
    this->up = QVector3D::crossProduct(this->right, this->front).normalized();
}
