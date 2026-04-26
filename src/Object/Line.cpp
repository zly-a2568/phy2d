#include "../../include/Object/Line.hpp"

namespace phy2d{
    Line::Line(float x1, float y1, float x2, float y2,float m){
        this->p1.x=x1;
        this->p1.y=y1;
        this->p2.x=x2;
        this->p2.y=y2;
        this->mass=m;
        this->type=LINE;
        this->position.x=(x1+x2)/2;
        this->position.y=(y1+y2)/2;
        this->RotationInertia=mass*length(p1-p2)*length(p1-p2)/12;
        this->is_static=true;
    }
    void Line::update(float dt){
        this->position.x=(p1.x+p2.x)/2;
        this->position.y=(p1.y+p2.y)/2;
        float l=glm::min(p1.x,p2.x)-10.0f;
        float r=glm::max(p1.x,p2.x)+10.0f;
        float b=glm::min(p1.y,p2.y)-10.0f;
        float t=glm::max(p1.y,p2.y)+10.0f;
        bounding_box=AABB(l,r,b,t);
    }
    vec2 Line::getPoint1() const{
        return this->p1;
    }
    vec2 Line::getPoint2() const{
        return this->p2;
    }

}