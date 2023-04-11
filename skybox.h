#ifndef SKYBOX_H
#define SKYBOX_H

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector>
#include <QString>
#include <memory>
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include "camera.h"

class SkyBox : protected QOpenGLFunctions
{
    //To do
    //天空盒正常显示
    //天空盒独有一个shader进行管理
private:
    const QString skyboxVspath;
    QVector <QString> faces;//天空盒照片地址
    QOpenGLTexture* skyboxTexture;
    QOpenGLShader* skyboxvs;
    QOpenGLShader* skyboxfs;
    QOpenGLShaderProgram* skyboxShader;
    QOpenGLVertexArrayObject* VAO;
    QOpenGLBuffer* VBO;
    QOpenGLFunctions *core;
private:
    void initShader(const QString&,const QString&);
    void initVertex();
public:
    SkyBox(const QVector <QString>,const QString&,const QString&,QOpenGLWidget*);
    void bind();
    void loadTexture();
    void draw(const Camera* camera,const QMatrix4x4& projection);
};

#endif // SKYBOX_H
