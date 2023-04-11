
#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QOpenGLShader>//为了glfloat 这些变量，所以引进这个头文件
#include <QtMath>
#include <QMap>
#include <QDebug>


enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};


const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 0.01f;
const GLfloat SENSITIVITY = 0.06f;
const GLfloat ZOOM = 45.0f;

class Camera {
public:
    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
    GLfloat yaw = YAW, GLfloat pitch = PITCH): front(QVector3D(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
    mouseSensitivity(SENSITIVITY), zoom(ZOOM){

        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();

        for(GLuint i = 0; i != 1024; ++i)
            keys[i] = GL_FALSE;
    }

    QMatrix4x4 getViewMatrix() const; //返回lookat函数
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch = true); //相应鼠标操作函数
    void processMouseScroll(GLfloat yoffset);//鼠标滚轮事件
    void processInput(GLfloat dt);//键盘循环函数

    QVector3D position;
    QVector3D worldUp;
    QVector3D front;

    QVector3D up; //view坐标系 的上方向
    QVector3D right; // view坐标系的 右方向

    //Eular Angles
    GLfloat pitch;
    GLfloat yaw;

    //Camera options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

    //键盘多键触控所需
    QMap<int,bool> keys;
private:
    void updateCameraVectors();
    void processKeyboard(Camera_Movement direction, GLfloat deltaTime);//键盘处理事件函数

};
#endif // CAMERA_H
