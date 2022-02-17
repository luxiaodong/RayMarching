#ifndef GRAYMARCHING_H
#define GRAYMARCHING_H

#include <QImage>
#include <QSize>
#include <QColor>

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
    float sdf_2d(QVector2D& p1);

public:
    QImage m_image; //输出图像
};

#endif // GRAYMARCHING_H
