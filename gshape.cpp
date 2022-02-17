#include "gshape.h"
#include <QtMath>

GShape::GShape(float sdf, float emissive)
{
    m_sdf = sdf;
    m_emissive = emissive;
}

GShape& GShape::unionOp(GShape& one, GShape& other)
{
    if(one.m_sdf < other.m_sdf)
    {
        return one;
    }

    return other;
}

GShape GShape::intersectOp(GShape& one, GShape& other)
{
    GShape s = one;
    s.m_sdf = qMax(one.m_sdf, other.m_sdf);
    s.m_emissive = one.m_sdf > other.m_sdf ? other.m_emissive : one.m_emissive;
    return s;
}

GShape GShape::subtractOp(GShape& one, GShape& other)
{
    GShape s = one;
    s.m_sdf = (one.m_sdf > -other.m_sdf) ? one.m_sdf : -other.m_sdf;
    return s;
}