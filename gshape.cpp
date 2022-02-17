#include "gshape.h"

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
