#include "gsigndistancefunction.h"
#include <QtMath>

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

float GSignDistanceFunction::plane2D(QVector2D pt, QVector2D p0, QVector2D dir)
{
    return QVector2D::dotProduct(pt - p0, dir);
}

float GSignDistanceFunction::segment2D(QVector2D pt, QVector2D a, QVector2D b)
{
    QVector2D dir = b - a;
    QVector2D nor = QVector2D(-dir.y(), dir.x());
    float d2ll = GSignDistanceFunction::plane2D(pt, a, nor.normalized());
    float d2lr = GSignDistanceFunction::plane2D(pt, a, -nor.normalized());
    float d2l = qMax(d2ll, d2lr);
    float dot = QVector2D::dotProduct(pt - a, b - a);
    QVector2D c = a + dot/QVector2D(b - a).length() * dir.normalized();
    float lamda = 0;
    if(b.x() - a.x() > b.y() - a.y())
    {
        lamda = (c.x() - a.x())/(b.x() - a.x());
    }
    else
    {
        lamda = (c.y() - a.y())/(b.y() - a.y());
    }
    if(lamda < 0) return QVector2D(pt - a).length();
    if(lamda < 1) return d2l;
    return QVector2D(pt - b).length();
}

float GSignDistanceFunction::capsule2D(QVector2D pt, QVector2D a, QVector2D b, float r)
{
    return GSignDistanceFunction::segment2D(pt, a, b) - r;
}

float GSignDistanceFunction::box2D(QVector2D pt, QVector2D c0, QVector2D halfSize, double sita)
{
    QVector2D delt = pt - c0;
    float cosV = static_cast<float>(qCos(sita));
    float sinV = static_cast<float>(qSin(sita));
    float dx = qAbs(delt.x()*cosV + delt.y()*sinV) - halfSize.x();
    float dy = qAbs(-delt.x()*sinV + delt.y()*cosV) - halfSize.y();
    float ax = qMax(dx, 0.0f);
    float ay = qMax(dy, 0.0f);
    return QVector2D(ax,ay).length() + qMin(qMax(dx,dy),0.0f);
}

float GSignDistanceFunction::triangle2D(QVector2D pt, QVector2D a, QVector2D b, QVector2D c)
{
    float dab = GSignDistanceFunction::segment2D(pt, a, b);
    float dac = GSignDistanceFunction::segment2D(pt, a, c);
    float dbc = GSignDistanceFunction::segment2D(pt, b, c);
    float d = qMin(qMin(dab,dac),dbc);
    bool s1 = (b.x()-a.x())*(pt.y()-a.y()) > (b.y()-a.y())*(pt.x()-a.x());
    bool s2 = (c.x()-b.x())*(pt.y()-b.y()) > (c.y()-b.y())*(pt.x()-b.x());
    bool s3 = (a.x()-c.x())*(pt.y()-c.y()) > (a.y()-c.y())*(pt.x()-c.x());
    if(s1 && s2 && s3) return -d;
    return d;
}
