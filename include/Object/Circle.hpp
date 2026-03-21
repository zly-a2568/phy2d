#ifndef PHY2D_CIRCLE_HPP
#define PHY2D_CIRCLE_HPP

#include "Object.hpp"
namespace phy2d{

class Circle : public Object {

public:
    Circle(float x, float y,float m,float radius);
    float radius;
    void update(float dt) override;
    float getRadius() const;
};
}
#endif //PHY2D_CIRCLE_HPP