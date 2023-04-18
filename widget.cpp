#include "widget.h"


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
    glEnable(GL_PROGRAM_POINT_SIZE);
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);//加入面剔除
    skybox = new SkyBox(faces,":/shaders/shaders/skyboxvs.glsl",":/shaders/shaders/skyboxfs.glsl",this);
    chunk_test = new Chunk(this->context()->functions());
    cursor.reset(new CrossCursor(this->context()->functions()));
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Widget::paintGL()//记得观察，别把初始化放在渲染里面了
{
    cursor->draw();
    QMatrix4x4 projection,model{};
    projection.setToIdentity();
    projection.perspective(camera->zoom,this->width()/this->height(),0.1f,100.0f);
    model.setToIdentity();
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
    float xoffset = event->position().x() - rect().center().x();
    float yoffset = rect().center().y() - event->position().y();
    if(firstMouse){
        xoffset = event->position().x();
        yoffset = event->position().y();
        firstMouse = false;
    }
    camera->processMouseMovement(xoffset,yoffset);//camera process
    QPoint glob = mapToGlobal(QPoint(width()/2,height()/2));
    QCursor::setPos(glob);
    QOpenGLWidget::mouseMoveEvent(event);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    camera->keys[event->key()] = true;
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    camera->keys[event->key()] = false;
}

