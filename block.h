#ifndef MYTEXTURE_H
#define MYTEXTURE_H


#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector>
#include <QVector3D>
#include <QVector2D>

#define CUBE_SIZE 16.0f//每一小个多大

#define TEXTURE_SIZE 256.0f//


class Block : public QOpenGLTexture
{
    //自定义纹理类2023.3.26
public:
    enum BlockType{
        Dirt,
        DirtWithGrass//就是有草的那一个方块
    };
private:
    QOpenGLFunctions* core;
    QOpenGLVertexArrayObject* VAO;
    QOpenGLBuffer* VBO;
    QOpenGLBuffer* IBO;
    QOpenGLShaderProgram* blockShader;
    QVector2D id;//相对位置
    QVector2D t_id;//顶上面纹理
    QVector2D b_id;//底面纹理
    QVector2D u0v0;//纹理的四个角的坐标
    QVector2D u0v1;
    QVector2D u1v1;
    QVector2D u1v0;

    QVector2D tu0v0;//为了方便处理，顶上纹理的四个角的坐标
    QVector2D tu0v1;
    QVector2D tu1v1;
    QVector2D tu1v0;

    QVector2D bu0v0;//底面纹理的四个角的坐标
    QVector2D bu0v1;
    QVector2D bu1v1;
    QVector2D bu1v0;

    QVector3D pos;
    BlockType currentType;
    bool showUpface = false;
public:
    Block(QOpenGLFunctions*,BlockType,bool isShow = false,const QString& path = ":/textures/texture/blocks.png");
    void draw(const QMatrix4x4& model,const QMatrix4x4& view,const QMatrix4x4& projection);
    void setPos(const QVector3D&);
    QVector3D getPos()const;
    void setIsTop(bool state);
private:
    void updateVertexData(float* , float*,float* );
    void initTexcoord(BlockType);
    void initVertexPos();
    void initShader();
};

#endif // MYTEXTURE_H
