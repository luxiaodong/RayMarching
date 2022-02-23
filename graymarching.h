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
    float trace_2d(QVector2D& p0, QVector2D& dir, int depth);
    QVector2D normal_2d(QVector2D& p0);
    GShape scene_2d(QVector2D& p1);
    QVector2D reflect_2D(QVector2D in, QVector2D n);
    QVector2D refract_2D(QVector2D in, QVector2D n, float ratio, bool &isOk); //折射率
    float fresnel_2D(float cosi, float cost, float etai, float etat);
    float schlick_2D(float cosi, float cost, float etai, float etat);
    float beerLambert(float absorption, float distance);

public:
    QImage m_image; //输出图像
    QString m_filePath; //保存路径

    int m_sampleCount;
    int m_stepCount;
    int m_maxDepth;
    float m_maxDistance;
    float m_epsilon;
    float m_normalBais;
};

#endif // GRAYMARCHING_H
