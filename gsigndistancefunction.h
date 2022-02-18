#ifndef GSIGNDISTANCEFUNCTION_H
#define GSIGNDISTANCEFUNCTION_H

#include <QSize>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

class GSignDistanceFunction
{
public:
    GSignDistanceFunction();

    // c0 圆心
    static float circle(QVector2D pt, QVector2D c0, float r);
    static float sphere(QVector3D pt, QVector3D c0, float r);

    static float plane2D(QVector2D pt, QVector2D p0, QVector2D dir);
    static float segment2D(QVector2D pt, QVector2D a, QVector2D b);
    static float capsule2D(QVector2D pt, QVector2D a, QVector2D b, float r);
    static float box2D(QVector2D pt, QVector2D c0, QVector2D halfSize, double sita);
    static float triangle2D(QVector2D pt, QVector2D a, QVector2D b, QVector2D c);
};

#endif // GSIGNDISTANCEFUNCTION_H
