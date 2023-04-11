#include "widget.h"

static float plantVertices[] = {//这里y轴是反着的，相当于从上到下了
     // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
     0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
     0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
     1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

     0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
     1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
     1.0f,  0.5f,  0.0f,  1.0f,  0.0f
 };
static QVector<QVector3D> glassPosition
{
    QVector3D(-1.5f, 0.0f, -0.48f),
    QVector3D(1.5f, 0.0f, 0.51f),
    QVector3D(0.0f, 0.0f, 0.7f),
    QVector3D(-0.3f, 0.0f, -2.3f),
    QVector3D(0.5f, 0.0f, -0.6f)
};


Widget::Widget()
{
    setMouseTracking(true);
    QSurfaceFormat x;
    x.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    setFormat(x);
    setCursor(Qt::BlankCursor);
    this->setFixedSize(1000,800);
    faces =
    {
        ":/skybox/skybox/right.jpg",
        ":/skybox/skybox/left.jpg",
        ":/skybox/skybox/top.jpg",
        ":/skybox/skybox/bottom.jpg",
        ":/skybox/skybox/front.jpg",
        ":/skybox/skybox/back.jpg"
    };
    camera = new Camera(QVector3D(0.0f, 0.0f, 3.0f));
}

Widget::~Widget()
{

}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    skybox = new SkyBox(faces,":/shaders/shaders/skyboxvs.glsl",":/shaders/shaders/skyboxfs.glsl",this);
    m_block1 = new Block(this->context()->functions(),Block::DirtWithGrass,true);
    m_block2 = new Block(this->context()->functions(),Block::DirtWithGrass,true);
    chunk_test = new Chunk(this->context()->functions());
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Widget::paintGL()//记得观察，别把初始化放在渲染里面了
{
    QMatrix4x4 projection,model{};
    projection.setToIdentity();
    projection.perspective(camera->zoom,this->width()/this->height(),0.1f,100.0f);
    model.setToIdentity();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_block1->draw(model,camera->getViewMatrix(),projection);
    //m_block2->setPos(QVector3D(4.0f,4.0f,4.0f));
    //m_block2->draw(model,camera->getViewMatrix(),projection);
    chunk_test->draw(model,camera->getViewMatrix(),projection);
    skybox->draw(camera,projection);//画天空盒
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    deltaTime = currentTime - lastDrawTime;
    lastDrawTime = currentTime;//计算帧时间差
    camera->processInput(deltaTime);
    update();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    float xoffset = event->x() - rect().center().x();
    float yoffset = rect().center().y() - event->y();//计算偏离中心的值
    if(firstMouse){
        xoffset = event->x();
        yoffset = event->y();
        firstMouse = false;
    }
    camera->processMouseMovement(xoffset,yoffset);
    QCursor::setPos(geometry().center());//这里是保证鼠标指针不会动，保持中心位置
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    camera->keys[event->key()] = true;
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    camera->keys[event->key()] = false;
}

