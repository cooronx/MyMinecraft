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
    camera = new Camera(QVector3D(0.0f, 0.0f, 3.0f));
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Widget::paintGL()//记得观察，别把初始化放在渲染里面了
{
    cursor->draw();
    projection.setToIdentity();
    projection.perspective(camera->zoom,this->width()/this->height(),0.1f,100.0f);//
    QMatrix4x4 model{};
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
    float yoffset = rect().center().y() - event->position().y();//y坐标是相反的
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

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        ray = std::make_shared <QVector3D> (getCurrentRay(event->position().x(),event->position().y(),this->width(),this->height()));
        qDebug()<<*ray;
    }
}

QVector3D Widget::getCurrentRay(float mx, float my,int width,int height)
{
    float x = (2.0f * mx) / width - 1.0f;//从(w,h)映射到[-1,1]
    float y = 1.0f - (2.0f * my) / height;
    float z = 1.0f;//只需要远平面的那个点就行了
    QVector3D ray_nds(x, y, z);
    QVector4D ray_clip = QVector4D(ray_nds,1.0f);
    QVector4D ray_eye = camera->getViewMatrix().inverted() * ray_clip;//乘以观察矩阵的逆矩阵
    QVector4D ray_world = projection.inverted() * ray_eye;//乘以投影矩阵的逆矩阵,得到世界坐标
    if(ray_world.w() != 0.0f){
        ray_world.setX(ray_world.x()/ray_world.w());
        ray_world.setY(ray_world.y()/ray_world.w());
        ray_world.setZ(ray_world.z()/ray_world.w());
    }
    QVector3D ray_dir(ray_world.toVector3D() - camera->position);//减去相机的位置，就得到了所指的向量
    ray_dir.normalize();
    return ray_dir;
}

