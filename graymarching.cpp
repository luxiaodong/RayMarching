#include "graymarching.h"
#include "gsigndistancefunction.h"
#include <QtMath>
#include <QtDebug>

GRayMarching::GRayMarching(QSize& size)
{
    m_image = QImage(size, QImage::Format_RGBA8888);
    m_image.fill(Qt::black);
}

void GRayMarching::draw()
{
    this->draw_2d();
}

void GRayMarching::test()
{
    QVector2D p0 = QVector2D(0.5f, 0.5f);
    float value = this->sample_2d(p0);
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
            float value = this->sample_2d(p0);
            if(value > 1.0f) value = 1.0f;
            if(value < 0.0f) value = 0.0f;
            int c = (int)value*255;
            m_image.setPixelColor(x, y, QColor(c,c,c));
        }
    }
}

// 以平面上的pt为采样点,向各个方向采样
float GRayMarching::sample_2d(QVector2D& p0)
{
    float sum = 0.0f;
    int count = 10;
    for(int i = 0; i < count; ++i)
    {
        float sita = 2.0f*M_PI*qrand()/RAND_MAX;
        QVector2D dir = QVector2D(qCos(sita), qSin(sita));
        sum += this->rayMarching_2d(p0, dir);
    }
    return sum/count;
}

float GRayMarching::rayMarching_2d(QVector2D& p0, QVector2D& dir)
{
    float t = 0.0f;
    for(int i = 0; i < 10; ++i)
    {
        QVector2D p1 = p0 + dir*t;
        float dis = sdf_2d(p1);
        if(dis < 0.01f) return 1.0;
        t += dis;
    }

    return 0.0f;
}

float GRayMarching::sdf_2d(QVector2D& p1)
{
    QVector2D c0 = QVector2D(0.5f, 0.5f);
    float r = 0.1f;
    return GSignDistanceFunction::circle(p1, c0, r);
}

