#ifndef GSHAPE_H
#define GSHAPE_H


class GShape
{
public:
    GShape(float sdf, float emissive);
    static GShape& unionOp(GShape& one, GShape& other);
    static GShape intersectOp(GShape& one, GShape& other);
    static GShape subtractOp(GShape& one, GShape& other);

public:
    float m_sdf;
    float m_emissive;
};

#endif // GSHAPE_H
