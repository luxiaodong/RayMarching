#include "graymarching.h"
#include "gsigndistancefunction.h"
#include "gshape.h"
#include <QtMath>
#include <QtDebug>

GRayMarching::GRayMarching(QSize& size)
{
    m_image = QImage(size, QImage::Format_RGBA8888);
    m_image.fill(Qt::black);

    m_sampleCount = 64;
    m_maxStepCount = 64;
    m_maxDistance = 2.0f;
    m_maxEpsilon = 1e-6f;
}

void GRayMarching::draw()
{
    this->draw_2d();
//    this->test();
}

void GRayMarching::test()
{
    QVector2D p1 = QVector2D(0.9f, 0.1f);
    float value = GSignDistanceFunction::capsule2D(p1, QVector2D(0.4f,0.4f), QVector2D(0.6f,0.6f), 0.1f);
qDebug()<<value;
}

void GRayMarching::draw_2d()
{
    QSize size = m_image.size();
    for(int y = 0; y < size.height(); ++y)
    {
        for(int x = 0; x < size.width(); ++x)
        {
            QVector2D p0(1.0f*x/size.width(), 1.0f*y/size.height());
//            float value = this->sample_2d(p0);
//            if(value > 1.0f) value = 1.0f;
//            if(value < 0.0f) value = 0.0f;
//            int c = static_cast<int>(value*255);
//            m_image.setPixelColor(x, y, QColor(c,c,c));

            QVector2D n = this->normal_2d(p0);
            int r = (n.x()*0.5f + 0.5f) * 255;
            int g = (n.y()*0.5f + 0.5f) * 255;
            if(r > 255) r = 255;
            if(g > 255) g = 255;
            r = qMax(r, 0);
            g = qMax(g, 0);
            m_image.setPixelColor(x, y, QColor(r,g,0));
        }
    }
}

// 以平面上的pt为采样点,向各个方向采样
float GRayMarching::sample_2d(QVector2D& p0)
{
    float sum = 0.0f;
    for(int i = 0; i < m_sampleCount; ++i)
    {
//        double sita = 2.0*M_PI*i/m_sampleCount; //分层采样
//        double sita = 2.0*M_PI*qrand()/RAND_MAX; //均匀采样
        double sita = 2.0*M_PI*(i+ static_cast<double>(qrand())/RAND_MAX)/m_sampleCount; //抖动采样
        QVector2D dir = QVector2D( static_cast<float>(qCos(sita)), static_cast<float>(qSin(sita)));
        sum += this->rayMarching_2d(p0, dir);
    }
    return sum/m_sampleCount;
}

float GRayMarching::rayMarching_2d(QVector2D& p0, QVector2D& dir)
{
    float t = 0.0f;
    for(int i = 0; i < m_maxStepCount; ++i)
    {
        QVector2D p1 = p0 + dir*t;
        GShape shape = scene_2d(p1);
        if(shape.m_sdf < m_maxEpsilon) return shape.m_emissive;
        if(t > m_maxDistance) break;
        t += shape.m_sdf;
    }

    return 0.0f;
}

QVector2D GRayMarching::normal_2d(QVector2D& p0)
{
    float epsilon = m_maxEpsilon;
    QVector2D pxa = QVector2D(p0.x()+epsilon, p0.y());
    QVector2D pxn = QVector2D(p0.x()-epsilon, p0.y());
    QVector2D pya = QVector2D(p0.x(), p0.y()+epsilon);
    QVector2D pyn = QVector2D(p0.x(), p0.y()-epsilon);
    float dx = (scene_2d(pxa).m_sdf - scene_2d(pxn).m_sdf)/(2*epsilon);
    float dy = (scene_2d(pya).m_sdf - scene_2d(pyn).m_sdf)/(2*epsilon);
    return QVector2D(dx, dy).normalized();
}

GShape GRayMarching::scene_2d(QVector2D& p1)
{
//    return GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f, 0.5f), 0.1f), 1.0f);

//    GShape r1 = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.3f, 0.3f), 0.1f), 2.0f);
//    GShape r2 = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.3f, 0.7f), 0.05f), 0.8f);
//    GShape r3 = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.7f, 0.5f), 0.1f), 0.0f);
//    return GShape::unionOp(GShape::unionOp(r1, r2), r3);

//    GShape r4 = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.4f, 0.5f), 0.2f), 1.0f);
//    GShape r5 = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.6f, 0.5f), 0.2f), 0.8f);
//    return GShape::unionOp(r4, r5);
//    return GShape::intersectOp(r4, r5);
//    return GShape::subtractOp(r4, r5);

//    return GShape(GSignDistanceFunction::plane2D(p1,QVector2D(0,0.5f),QVector2D(0,1)), 0.8f);

//    GShape c6 = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f, 0.5f), 0.2f), 1.0f);
//    GShape p2 = GShape(GSignDistanceFunction::plane2D(p1,QVector2D(0,0.5f),QVector2D(0,1)), 0.8f);
//    return GShape::intersectOp(c6, p2);

//    return GShape(GSignDistanceFunction::segment2D(p1, QVector2D(0.4f,0.4f), QVector2D(0.6f,0.6f)), 1.0f);
//    return GShape(GSignDistanceFunction::capsule2D(p1, QVector2D(0.4f,0.4f), QVector2D(0.6f,0.6f), 0.1f), 1.0f);

//    return GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.5f,0.5f), QVector2D(0.3f,0.1f), M_PI/16), 1.0f);
//    return GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.5f,0.5f), QVector2D(0.3f,0.1f), M_PI/16) - 0.1f, 1.0f);

//    return GShape(GSignDistanceFunction::triangle2D(p1,QVector2D(0.5f,0.2f),QVector2D(0.8f,0.8f),QVector2D(0.3f,0.6f)),1.0f);

    GShape c7 = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.4f, 0.2f), 0.1f), 2.0f);
    GShape b1 = GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.5f,0.8f), QVector2D(0.1f,0.1f), M_PI/16), 0.0f);
    GShape b2 = GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.8f,0.5f), QVector2D(0.1f,0.1f), M_PI/16), 0.0f);
    return GShape::unionOp(c7, GShape::unionOp(b1, b2));

//    (QVector2D pt, QVector2D c0, QSize halfSize, double sita)
}

