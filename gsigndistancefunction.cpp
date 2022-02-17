#include "gsigndistancefunction.h"

GSignDistanceFunction::GSignDistanceFunction()
{

}

float GSignDistanceFunction::circle(QVector2D pt, QVector2D c0, float r)
{
    QVector2D dis = pt - c0;
    return dis.length() - r;
}

float GSignDistanceFunction::sphere(QVector3D pt, QVector3D c0, float r)
{
    QVector3D dis = pt - c0;
    return dis.length() - r;
}
