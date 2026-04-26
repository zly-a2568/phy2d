#pragma once

#include <glm/glm.hpp>

using namespace glm;

namespace phy2d {

class AABB {
    public:
    AABB() {

    }
    AABB(float l,float r,float b, float t) {
        left=l;
        right=r;
        bottom=b;
        top=t;
    }
    bool contains(vec2& p) {
        return (p.x>=left&&p.x<=right)&&(p.y>=bottom&&p.y<=top);
    }

    bool contains(AABB& aabb) {
        return (aabb.left>=left&&aabb.right<=right)&&(aabb.bottom>=bottom&&aabb.top<=top);
    }

    bool intersects(AABB& other)const {
        float max_x=glm::min(this->right,other.right);
        float min_x=glm::max(this->left,other.left);
        float max_y=glm::min(this->top,other.top);
        float min_y=glm::max(this->bottom,other.bottom);
        return (max_x>min_x)&&(max_y>min_y);
    }
    AABB fatten() {
        float x_center=(left+right)/2;
        float y_center=(top+bottom)/2;
        AABB a;
        a.left=1.2f*(left-x_center)+x_center;
        a.right=1.2f*(right-x_center)+x_center;
        a.bottom=1.2f*(bottom-y_center)+y_center;
        a.top=1.2f*(top-y_center)+y_center;
        return a;

    }

    float area() {
        return (right-left)*(top-bottom);
    }
    float perimeter() {
        return 2.0f*((right-left)+(top-bottom));
    }
    float left;
    float right;
    float bottom;
    float top;
};
    inline AABB operator+(AABB& a,AABB& b) {
        float max_x=glm::max(a.right,b.right);
        float min_x=glm::min(a.left,b.left);
        float max_y=glm::max(a.top,b.top);
        float min_y=glm::min(a.bottom,b.bottom);
        return AABB(min_x,max_x,min_y,max_y);
    }
    inline AABB merge(AABB& a,AABB& b) {
        float max_x=glm::max(a.right,b.right);
        float min_x=glm::min(a.left,b.left);
        float max_y=glm::max(a.top,b.top);
        float min_y=glm::min(a.bottom,b.bottom);
        return AABB(min_x,max_x,min_y,max_y);
    }
}
