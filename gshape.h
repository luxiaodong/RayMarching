#ifndef GSHAPE_H
#define GSHAPE_H


class GShape
{
public:
    GShape(float sdf, float emissive = 1.0, float reflectivity = 0.0f);
    static GShape unionOp(GShape one, GShape other);
    static GShape intersectOp(GShape one, GShape other);
    static GShape subtractOp(GShape one, GShape other);

public:
    float m_sdf;
    float m_emissive;
    float m_reflectivity;
};

#endif // GSHAPE_H
