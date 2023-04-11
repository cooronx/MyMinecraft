#include "skybox.h"



SkyBox::SkyBox(const QVector <QString> f,const QString& vs,const QString& fs,QOpenGLWidget* fa)
{
    core = fa->context()->functions();
    initShader(vs,fs);//初始化shader
    faces = f;
    loadTexture();
}

void SkyBox::initShader(const QString &vs, const QString &fs)
{
    skyboxShader = new QOpenGLShaderProgram();
    skyboxShader->addShaderFromSourceFile(QOpenGLShader::Vertex,vs);
    skyboxShader->addShaderFromSourceFile(QOpenGLShader::Fragment,fs);
    skyboxShader->link();//配置skybox着色器
}

void SkyBox::initVertex()
{
    float skyboxVertices[] = {
            // 天空盒坐标
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
    VAO = new QOpenGLVertexArrayObject();//配置顶点数据
    VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    VAO->create();
    VAO->bind();
    VBO->create();
    VBO->bind();
    VBO->allocate(skyboxVertices,sizeof (skyboxVertices));
    skyboxShader->setAttributeBuffer(0,GL_FLOAT,0,3,3*sizeof (float));
    skyboxShader->enableAttributeArray(0);
    VBO->release();
    VAO->release();
}

void SkyBox::loadTexture()
{
    initVertex();
    skyboxTexture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);//加载skybox
    skyboxTexture->create();
    skyboxTexture->bind();
    for(int i = 0;i<faces.size();++i){
        QImage temp = QImage(faces[i]).convertToFormat(QImage::Format_RGB888);
        //qDebug()<<temp<<endl;
        core->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, temp.width(), temp.height(), 0,
                           GL_RGB, GL_UNSIGNED_BYTE,temp.bits());
    }
    skyboxTexture->setMinificationFilter(QOpenGLTexture::Linear);     //纹理放大或缩小时，像素的取值方法 ，线性或就近抉择
    skyboxTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    skyboxTexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);   //设置纹理边缘的扩展方法
    skyboxTexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);

}

void SkyBox::draw(const Camera* camera,const QMatrix4x4& projection)
{

    skyboxShader->bind();
    QMatrix4x4 view = camera->getViewMatrix();
    view.setColumn(3,QVector4D(0,0,0,1.0f));//去除掉平移分量
    skyboxShader->setUniformValue("skybox",0);
    skyboxShader->setUniformValue("view",view);
    skyboxShader->setUniformValue("projection",projection);
    core->glDepthFunc(GL_LEQUAL);
    //开始绘制
    VAO->bind();
    core->glActiveTexture(GL_TEXTURE0);
    skyboxTexture->bind();
    core->glDrawArrays(GL_TRIANGLES, 0, 36);
    core->glDepthFunc(GL_LESS);
}
