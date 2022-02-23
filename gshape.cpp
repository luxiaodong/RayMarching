#include "gshape.h"
#include <QtMath>

GShape::GShape(float sdf, float emissive, float reflectivity, float ratio, float absorption)
{
    m_sdf = sdf;
    m_emissive = emissive;
    m_reflectivity = reflectivity;
    m_ratio = ratio;
    m_absorption = absorption;
}

GShape GShape::unionOp(GShape one, GShape other)
{
    if(one.m_sdf < other.m_sdf)
    {
        return one;
    }

    return other;
}

GShape GShape::intersectOp(GShape one, GShape other)
{
    GShape s = one;
    s.m_sdf = qMax(one.m_sdf, other.m_sdf);
    s.m_emissive = one.m_sdf < other.m_sdf ? one.m_emissive : other.m_emissive;
    return s;
}

GShape GShape::subtractOp(GShape one, GShape other)
{
    GShape s = one;
    s.m_sdf = (one.m_sdf > -other.m_sdf) ? one.m_sdf : -other.m_sdf;
    return s;
}
