#ifndef GSIGNDISTANCEFUNCTION_H
#define GSIGNDISTANCEFUNCTION_H

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
};

#endif // GSIGNDISTANCEFUNCTION_H
