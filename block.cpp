#include "block.h"




Block::Block(QOpenGLFunctions *fa,BlockType bt,bool isTop,const QString& path):
    core(fa),
    QOpenGLTexture(QImage(path).mirrored(),QOpenGLTexture::GenerateMipMaps),
    showUpface(isTop),
    currentType(bt)
{

    pos = QVector3D(0.0f,0.0f,0.0f);
    initShader();//配置着色器
    initTexcoord(currentType);//配置纹理
    initVertexPos();//配置顶点
}

void Block::draw(const QMatrix4x4 &model, const QMatrix4x4& view, const QMatrix4x4 &projection)
{

    blockShader->bind();
    blockShader->setUniformValue("model",model);
    blockShader->setUniformValue("view",view);
    blockShader->setUniformValue("projection",projection);
    VAO->bind();
    this->bind();
    core->glActiveTexture(GL_TEXTURE0);
    core->glDrawArrays(GL_TRIANGLES,0,36);
    VAO->release();
    blockShader->release();
}

void Block::setPos(const QVector3D & p)
{
    this->pos = p;
    initVertexPos();
}

QVector3D Block::getPos() const
{
    return this->pos;
}

void Block::setIsTop(bool state)
{
    Q_UNUSED(state);
    this->showUpface=true;
    initTexcoord(currentType);
    initVertexPos();
}

void Block::updateVertexData(float *vertices, float *upface,float* bottomface)
{
    int cnt = 0;
    for(int i = 0;i<120;++i){
        if(cnt == 3||cnt == 4){
            if(cnt == 4)cnt = 0;
            else ++cnt;
            continue;
        }
        else{
            ++cnt;
            if(cnt == 1){
                vertices[i]+=pos.x();
            }
            else if(cnt == 2){
                vertices[i]+=pos.y();
            }
            else if(cnt == 3){
                vertices[i]+=pos.z();
            }
        }
    }
    cnt = 0;
    for(int i = 0;i<30;++i){
        if(cnt == 3||cnt == 4){
            if(cnt == 4)cnt = 0;
            else ++cnt;
            continue;
        }
        else{
            ++cnt;
            if(cnt == 1){
                upface[i]+=pos.x();
            }
            else if(cnt == 2){
                upface[i]+=pos.y();
            }
            else if(cnt == 3){
                upface[i]+=pos.z();
            }
        }
    }
    cnt = 0;
    for(int i = 0;i<30;++i){
        if(cnt == 3||cnt == 4){
            if(cnt == 4)cnt = 0;
            else ++cnt;
            continue;
        }
        else{
            ++cnt;
            if(cnt == 1){
                bottomface[i]+=pos.x();
            }
            else if(cnt == 2){
                bottomface[i]+=pos.y();
            }
            else if(cnt == 3){
                bottomface[i]+=pos.z();
            }
        }
    }
}

void Block::initTexcoord(Block::BlockType bt)
{
    this->setMinMagFilters(QOpenGLTexture::Nearest,QOpenGLTexture::Nearest);
    this->setWrapMode(QOpenGLTexture::Repeat);
    blockShader->bind();
    blockShader->setUniformValue("block_face",0);
    switch (bt) {
        case Block::Dirt:{
            id = QVector2D(2,15);
            t_id = QVector2D(2,15);
            b_id = QVector2D(2,15);
            break;
        }
        case Block::DirtWithGrass:{
            id = QVector2D(3,15);
            if(this->showUpface){
                t_id = QVector2D(8,11);//如果与外面接触，就要展示草的那一面
            }
            else t_id = QVector2D(2,15);
            b_id = QVector2D(2,15);
            break;
        }
    }
    //因为纹理映射是0-1，所以把数据进行转换
    this->u0v0 = QVector2D((id.x()*CUBE_SIZE)/TEXTURE_SIZE,(id.y()*CUBE_SIZE)/TEXTURE_SIZE);
    this->u0v1 = QVector2D((id.x()*CUBE_SIZE)/TEXTURE_SIZE,((id.y()+1)*CUBE_SIZE)/TEXTURE_SIZE);
    this->u1v0 = QVector2D(((id.x()+1)*CUBE_SIZE)/TEXTURE_SIZE,(id.y()*CUBE_SIZE)/TEXTURE_SIZE);
    this->u1v1 = QVector2D(((id.x()+1)*CUBE_SIZE)/TEXTURE_SIZE,((id.y()+1)*CUBE_SIZE)/TEXTURE_SIZE);

    this->tu0v0 = QVector2D((t_id.x()*CUBE_SIZE)/TEXTURE_SIZE,(t_id.y()*CUBE_SIZE)/TEXTURE_SIZE);
    this->tu0v1 = QVector2D((t_id.x()*CUBE_SIZE)/TEXTURE_SIZE,((t_id.y()+1)*CUBE_SIZE)/TEXTURE_SIZE);
    this->tu1v0 = QVector2D(((t_id.x()+1)*CUBE_SIZE)/TEXTURE_SIZE,(t_id.y()*CUBE_SIZE)/TEXTURE_SIZE);
    this->tu1v1 = QVector2D(((t_id.x()+1)*CUBE_SIZE)/TEXTURE_SIZE,((t_id.y()+1)*CUBE_SIZE)/TEXTURE_SIZE);


    this->bu0v0 = QVector2D((b_id.x()*CUBE_SIZE)/TEXTURE_SIZE,(b_id.y()*CUBE_SIZE)/TEXTURE_SIZE);
    this->bu0v1 = QVector2D((b_id.x()*CUBE_SIZE)/TEXTURE_SIZE,((b_id.y()+1)*CUBE_SIZE)/TEXTURE_SIZE);
    this->bu1v0 = QVector2D(((b_id.x()+1)*CUBE_SIZE)/TEXTURE_SIZE,(b_id.y()*CUBE_SIZE)/TEXTURE_SIZE);
    this->bu1v1 = QVector2D(((b_id.x()+1)*CUBE_SIZE)/TEXTURE_SIZE,((id.y()+1)*CUBE_SIZE)/TEXTURE_SIZE);

}

void Block::initVertexPos()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  u0v0.x(),u0v0.y(),
        -0.5f,  0.5f, -0.5f,  u0v1.x(),u0v1.y(),
         0.5f,  0.5f, -0.5f,  u1v1.x(),u1v1.y(),
         0.5f,  0.5f, -0.5f,  u1v1.x(),u1v1.y(),
         0.5f, -0.5f, -0.5f,  u1v0.x(),u1v0.y(),
        -0.5f, -0.5f, -0.5f,  u0v0.x(),u0v0.y(),

        -0.5f, -0.5f,  0.5f,  u0v0.x(),u0v0.y(),
         0.5f, -0.5f,  0.5f,  u1v0.x(),u1v0.y(),
         0.5f,  0.5f,  0.5f,  u1v1.x(),u1v1.y(),
         0.5f,  0.5f,  0.5f,  u1v1.x(),u1v1.y(),
        -0.5f,  0.5f,  0.5f,  u0v1.x(),u0v1.y(),
        -0.5f, -0.5f,  0.5f,  u0v0.x(),u0v0.y(),

        -0.5f,  0.5f,  0.5f,  u1v1.x(),u1v1.y(),
        -0.5f,  0.5f, -0.5f,  u0v1.x(),u0v1.y(),
        -0.5f, -0.5f, -0.5f,  u0v0.x(),u0v0.y(),
        -0.5f, -0.5f, -0.5f,  u0v0.x(),u0v0.y(),
        -0.5f, -0.5f,  0.5f,  u1v0.x(),u1v0.y(),
        -0.5f,  0.5f,  0.5f,  u1v1.x(),u1v1.y(),

        0.5f,  0.5f,  0.5f,  u1v1.x(),u1v1.y(),
        0.5f, -0.5f,  0.5f,  u1v0.x(),u1v0.y(),
        0.5f, -0.5f, -0.5f,  u0v0.x(),u0v0.y(),
        0.5f, -0.5f, -0.5f,  u0v0.x(),u0v0.y(),
        0.5f,  0.5f, -0.5f,  u0v1.x(),u0v1.y(),
        0.5f,  0.5f,  0.5f,  u1v1.x(),u1v1.y(),
    };
    float upface[] = {
        -0.5f,  0.5f, -0.5f,  tu0v0.x(),tu0v0.y(),//此面为上面
        -0.5f,  0.5f,  0.5f,  tu0v1.x(),tu0v1.y(),
         0.5f,  0.5f,  0.5f,  tu1v1.x(),tu1v1.y(),
         0.5f,  0.5f,  0.5f,  tu1v1.x(),tu1v1.y(),
         0.5f,  0.5f, -0.5f,  tu1v0.x(),tu1v0.y(),
        -0.5f,  0.5f, -0.5f,  tu0v0.x(),tu0v0.y(),
    };
    float bottomface[] = {
        -0.5f, -0.5f, -0.5f,  bu0v0.x(),bu0v0.y(),
         0.5f, -0.5f, -0.5f,  bu0v1.x(),bu0v1.y(),
         0.5f, -0.5f,  0.5f,  bu1v1.x(),bu1v1.y(),
         0.5f, -0.5f,  0.5f,  bu1v1.x(),bu1v1.y(),
        -0.5f, -0.5f,  0.5f,  bu1v0.x(),bu1v0.y(),
        -0.5f, -0.5f, -0.5f,  bu0v0.x(),bu0v0.y()
    };
    updateVertexData(vertices,upface,bottomface);
    VAO = new QOpenGLVertexArrayObject();
    VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    VAO->create();
    VBO->create();
    VAO->bind();
    VBO->bind();
    VBO->allocate(sizeof (vertices)+sizeof (upface)+sizeof (bottomface));
    VBO->write(0,vertices,sizeof (vertices));
    VBO->write(sizeof (vertices),upface,sizeof (upface));
    VBO->write(sizeof (vertices)+sizeof (upface),bottomface,sizeof (bottomface));
    blockShader->setAttributeBuffer(0,GL_FLOAT,0,3,5*sizeof(float));
    blockShader->enableAttributeArray(0);
    blockShader->setAttributeBuffer(1,GL_FLOAT,3*sizeof (float),2,5*sizeof(float));
    blockShader->enableAttributeArray(1);
    VAO->release();
}

void Block::initShader()
{
    blockShader = new QOpenGLShaderProgram();
    blockShader->addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/shaders/blockvs.glsl");
    blockShader->addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/shaders/blockfs.glsl");
    blockShader->link();
}
