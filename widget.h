#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <memory>
#include <QString>
#include <QDateTime>
#include "skybox.h"
#include "camera.h"
#include "block.h"
#include "chunk.h"
#include "tools.h"

class Widget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

private:
    QMatrix4x4 projection;
    Chunk* chunk_test;
    Camera* camera;
    SkyBox* skybox;
    QVector<QString> faces;
    bool firstMouse = true;
    qint64 deltaTime;//每帧间隔时间
    qint64 lastDrawTime;//上一次画完的时间
    QOpenGLVertexArrayObject* grassVAO;
    QOpenGLBuffer* grassVBO;
    QOpenGLShaderProgram* grassShader;
    QOpenGLTexture* grassTexture;
    std::unique_ptr <CrossCursor> cursor = nullptr;
    std::shared_ptr <QVector3D> ray = nullptr;
public:
    Widget();
    ~Widget() override;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event)override;
private:
    void initGrass();
    QVector3D getCurrentRay(float mx,float my,int width,int height);//生成当前对应的射线
};
#endif // WIDGET_H
