#ifndef PHY2D_LINE_HPP
#define PHY2D_LINE_HPP

#include "Object.hpp"

namespace phy2d
{
    class Line : public Object
    {
    public:
        Line(float x1, float y1, float x2, float y2,float m);
        vec2 p1,p2;
        void update(float dt) override;
        vec2 getPoint1() const;
        vec2 getPoint2() const;

    };
}

#endif //PHY2D_LINE_HPP