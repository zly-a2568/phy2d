#ifndef PHY2D_BOX_HPP
#define PHY2D_BOX_HPP

#include "Object.hpp"

namespace phy2d
{
class Box : public Object
{
public:
    Box(float x, float y, float width, float height, float mass);
    vec2 size;
    void update(float dt) override;
    vec2 getSize() const;
    std::vector<vec2> getVertices();
    std::vector<std::pair<vec2, vec2>> getEdges();
};
}

#endif // PHY2D_BOX_HPP