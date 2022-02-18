#ifndef GRAYMARCHING_H
#define GRAYMARCHING_H

#include <QImage>
#include <QSize>
#include <QColor>
#include "gshape.h"

class GRayMarching
{
public:
    GRayMarching(QSize& size);

public:
    void draw();
    void test();

private:
    void draw_2d();
    float sample_2d(QVector2D& p0);
    float rayMarching_2d(QVector2D& p0, QVector2D& dir);
    QVector2D normal_2d(QVector2D& p0);
    GShape scene_2d(QVector2D& p1);

public:
    QImage m_image; //输出图像

    int m_sampleCount;
    int m_maxStepCount;
    float m_maxDistance;
    float m_maxEpsilon;
};

#endif // GRAYMARCHING_H
