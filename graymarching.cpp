#include "graymarching.h"
#include "gsigndistancefunction.h"
#include "gshape.h"
#include <QtMath>
#include <QtDebug>
#include <QDir>

GRayMarching::GRayMarching(QSize& size)
{
    m_filePath = "test.png";
    m_image = QImage(size, QImage::Format_RGBA8888);
    m_image.fill(Qt::black);

    m_sampleCount = 256;
    m_stepCount = 64;
    m_maxDistance = 5.0f;
    m_epsilon = 1e-6f;
    m_maxDepth = 5;
    m_normalBais = 1e-4f;
}

void GRayMarching::draw()
{
    this->draw_2d();
//    this->test();
}

void GRayMarching::test()
{
    m_filePath = "test.png";
    QString fullPath = QDir::currentPath() + QString("/../../../../RayMarching/image/") + m_filePath;
qDebug()<<fullPath;
    qDebug()<<m_image.save(fullPath);
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
            int c = static_cast<int>(value*255);
            m_image.setPixelColor(x, y, QColor(c,c,c));

//            QVector2D n = this->normal_2d(p0);
//            int r = (n.x()*0.5f + 0.5f) * 255;
//            int g = (n.y()*0.5f + 0.5f) * 255;
//            if(r > 255) r = 255;
//            if(g > 255) g = 255;
//            r = qMax(r, 0);
//            g = qMax(g, 0);
//            m_image.setPixelColor(x, y, QColor(r,g,0));
        }

qDebug()<<1.0f*y/size.height();
    }

    QString fullPath = QDir::currentPath() + QString("/../../../../RayMarching/image/") + m_filePath;
qDebug()<<fullPath;
    m_image.save(fullPath);
qDebug()<<1.0f;
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
        sum += this->trace_2d(p0, dir, 0);
    }
    return sum/m_sampleCount;
}

float GRayMarching::trace_2d(QVector2D& p0, QVector2D& dir, int depth)
{
    float t = 1e-3f;
    float sign = scene_2d(p0).m_sdf > 0.0f ? 1.0f : -1.0f;
    for(int i = 0; i < m_stepCount; ++i)
    {
        QVector2D p1 = p0 + dir*t;
        GShape shape = scene_2d(p1);
        if(shape.m_sdf * sign < m_epsilon)
        {
            float sum = shape.m_emissive;
            if( depth < m_maxDepth && (shape.m_reflectivity > 0.0f || shape.m_ratio > 0.0f))
            {
                float refl = shape.m_reflectivity;
                QVector2D nDir = this->normal_2d(p1);
                nDir = nDir*sign;
                if(shape.m_ratio > 0.0f)
                {
                    bool isOk = true;
                    float ratio = sign < 0.0f ? shape.m_ratio : 1.0f/shape.m_ratio;
                    QVector2D rDir = this->refract_2D(dir, nDir, ratio, isOk);
                    if(isOk)
                    {
                        float cosi = - QVector2D::dotProduct(dir, nDir);
                        float cost = - QVector2D::dotProduct(rDir, nDir);
                        refl = sign < 0.0f ? fresnel_2D(cosi, cost, shape.m_ratio, 1.0f) : fresnel_2D(cosi, cost, 1.0f, shape.m_ratio);
                        QVector2D p2 = p1 - nDir*m_normalBais;
                        sum += (1.0f - refl) * trace_2d(p2, rDir, depth + 1);
                    }
                    else
                    {
                        refl = 1.0f;
                    }
                }

                if(refl > 0.0f)
                {
                    QVector2D rDir = this->reflect_2D(dir, nDir);
                    QVector2D p2 = p1 + nDir*m_normalBais;
                    sum += refl * this->trace_2d(p2, rDir, depth + 1);
                }
            }
            return sum;
        }
        t += shape.m_sdf * sign;
        if(t > m_maxDistance) break;
    }

    return 0.0f;
}

// https://zhuanlan.zhihu.com/p/31534769
float GRayMarching::fresnel_2D(float cosi, float cost, float etai, float etat)
{
    float rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
    float rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
    return (rs * rs + rp * rp) * 0.5f;
}

float GRayMarching::schlick_2D(float cosi, float cost, float etai, float etat)
{
    float r1 = (etai - etat) / (etai + etat);
    float r2 = r1*r1;
    float a = 1.0f - (etai < etat ? cosi : cost);
    return r2 + (1.0f-r2)*static_cast<float>(qPow(static_cast<double>(a), 5));
}

QVector2D GRayMarching::reflect_2D(QVector2D in, QVector2D n)
{
    return in - 2*QVector2D::dotProduct(in, n)*n;
}

// ratio 空气与水的折射率比值
// https://zhuanlan.zhihu.com/p/31127076
QVector2D GRayMarching::refract_2D(QVector2D in, QVector2D n, float ratio, bool &isOk)
{
    float idotn = QVector2D::dotProduct(in,n);
    float k = 1.0f - ratio*ratio*(1.0f - idotn*idotn);
    if(k < 0.0f)
    {
        isOk = false;
        return QVector2D(0,0);
    }

    isOk = true;
    float a = ratio*idotn + static_cast<float>(qSqrt(static_cast<double>(k)));
    return (ratio*in - a*n).normalized();
}

QVector2D GRayMarching::normal_2d(QVector2D& p0)
{
    QVector2D pxa = QVector2D(p0.x()+m_epsilon, p0.y());
    QVector2D pxn = QVector2D(p0.x()-m_epsilon, p0.y());
    QVector2D pya = QVector2D(p0.x(), p0.y()+m_epsilon);
    QVector2D pyn = QVector2D(p0.x(), p0.y()-m_epsilon);
    float dx = (scene_2d(pxa).m_sdf - scene_2d(pxn).m_sdf)/(2*m_epsilon);
    float dy = (scene_2d(pya).m_sdf - scene_2d(pyn).m_sdf)/(2*m_epsilon);
    return QVector2D(dx, dy).normalized();
}

GShape GRayMarching::scene_2d(QVector2D& p1)
{
//    m_filePath = "basic_b.png";
//    return GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f, 0.5f), 0.1f), 1.0f);

//    m_filePath = "csg_scene.png";
//    GShape a = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.3f, 0.3f), 0.1f), 2.0f);
//    GShape b = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.3f, 0.7f), 0.05f), 0.8f);
//    GShape c = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.7f, 0.5f), 0.1f), 0.0f);
//    return GShape::unionOp(GShape::unionOp(a, b), c);

//    m_filePath = "csg_subtractOp2.png";
//    GShape a = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.4f, 0.5f), 0.2f), 1.0f);
//    GShape b = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.6f, 0.5f), 0.2f), 0.8f);
//    return GShape::unionOp(a, b);
//    return GShape::intersectOp(a, b);
//    return GShape::subtractOp(a, b);
//    return GShape::subtractOp(b, a);

//    m_filePath = "shapes_plane.png";
//    return GShape(GSignDistanceFunction::plane2D(p1,QVector2D(0,0.5f),QVector2D(0,1)), 0.8f);

//    m_filePath = "shapes_semicircle.png";
//    GShape a = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f, 0.5f), 0.2f), 1.0f);
//    GShape b = GShape(GSignDistanceFunction::plane2D(p1,QVector2D(0,0.5f),QVector2D(0,1)), 0.8f);
//    return GShape::intersectOp(a, b);

//    return GShape(GSignDistanceFunction::segment2D(p1, QVector2D(0.4f,0.4f), QVector2D(0.6f,0.6f)), 1.0f);

//    m_filePath = "shapes_capsule.png";
//    return GShape(GSignDistanceFunction::capsule2D(p1, QVector2D(0.4f,0.4f), QVector2D(0.6f,0.6f), 0.1f), 1.0f);

//    m_filePath = "shapes_box.png";
//    return GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.5f,0.5f), QVector2D(0.3f,0.1f), M_PI/16), 1.0f);
//    return GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.5f,0.5f), QVector2D(0.3f,0.1f), M_PI/16) - 0.1f, 1.0f);

//    m_filePath = "shape_triangle.png";
//    return GShape(GSignDistanceFunction::triangle2D(p1,QVector2D(0.5f,0.2f),QVector2D(0.8f,0.8f),QVector2D(0.3f,0.6f)),1.0f);

//    m_filePath = "scene_normal.png";
//    GShape a = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.4f, 0.2f), 0.1f), 2.0f);
//    GShape b = GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.5f,0.8f), QVector2D(0.1f,0.1f), M_PI/16), 0.0f);
//    GShape c = GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.8f,0.5f), QVector2D(0.1f,0.1f), M_PI/16), 0.0f);
//    return GShape::unionOp(a, GShape::unionOp(b, c));

//    m_filePath = "reflection_scene.png";
//    GShape a = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.4f, 0.2f), 0.1f), 2.0f, 0.0f);
//    GShape b = GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.5f,0.8f), QVector2D(0.1f,0.1f), M_PI/16), 0.0f, 0.9f);
//    GShape c = GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.8f,0.5f), QVector2D(0.1f,0.1f), M_PI/16), 0.0f, 0.7f);
//    return GShape::unionOp(a, GShape::unionOp(b, c));

//    m_filePath = "reflection_concavemirror.png";
//    GShape a = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.4f, 0.2f), 0.1f), 2.0f, 0.0f);
//    GShape b = GShape(GSignDistanceFunction::plane2D(p1,QVector2D(0,0.5f),QVector2D(0,-1)), 0.0f, 0.9f);
//    GShape c = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f, 0.5f), 0.4f), 0.0f, 0.9f);
//    return GShape::unionOp(a, GShape::subtractOp(b, c));

//    m_filePath = "m.png";
//    QVector2D p2 = QVector2D( qAbs(p1.x() - 0.5f) + 0.5f, p1.y());
//    GShape a = GShape(GSignDistanceFunction::capsule2D(p2, QVector2D(0.75f,0.25f), QVector2D(0.75f,0.75f), 0.05f), 1.0f);
//    GShape b = GShape(GSignDistanceFunction::capsule2D(p2, QVector2D(0.75f,0.25f), QVector2D(0.5,0.75f), 0.05f), 1.0f);
//    return a.m_sdf < b.m_sdf ? a : b;

//    m_filePath = "refraction_box.png";
//    GShape a = GShape(GSignDistanceFunction::circle(p1, QVector2D(-0.2f,-0.2f), 0.1f), 10.0f, 0.0f);
//    GShape b = GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.5f,0.5f), QVector2D(0.3f,0.2f), 0.0), 0.2f, 0.2f, 1.5f);
//    return GShape::unionOp(a, b);
//    return b;

    m_filePath = "refraction_convexlens.png";
    GShape a = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f,-0.5f), 0.05f),20.0f);
    GShape b = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f,0.2f), 0.35f),0.0f, 0.2f, 1.5f);
    GShape c = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f,0.8f), 0.35f),0.0f, 0.2f, 1.5f);
    return GShape::unionOp(a, GShape::intersectOp(b, c));

//    GShape a = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f,-0.5f), 0.05f),20.0f);
//    GShape b = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f,0.2f), 0.2f), 0.0f, 0.2f, 1.5f);
//    GShape c = GShape(GSignDistanceFunction::plane2D(p1,QVector2D(0.5f,0.5f),QVector2D(0,-1)), 0.0f, 0.2f, 1.5f);
//    return GShape::unionOp(a, GShape::intersectOp(b, c));

//    GShape a = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f,-0.5f), 0.05f),20.0f);
//    GShape b = GShape(GSignDistanceFunction::box2D(p1, QVector2D(0.5f,0.5f), QVector2D(0.0f,0.2f), 0.1), 0.0f, 0.2f, 1.5f);
//    GShape c = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f,0.12f), 0.35f), 0.0f, 0.2f, 1.5f);
//    GShape d = GShape(GSignDistanceFunction::circle(p1, QVector2D(0.5f,0.87f), 0.35f), 0.0f, 0.2f, 1.5f);
//    return GShape::unionOp(a, GShape::subtractOp(b, GShape::unionOp(c, d)) );
}

