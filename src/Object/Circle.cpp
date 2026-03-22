#include "../../include/Object/Circle.hpp"

namespace phy2d {

Circle::Circle(float x,float y,float m,float r){
    this->velocity = vec2(0,0);
    this->position = vec2(x,y);
    this->angle = 0;
    this->radius = r;
    this->mass = m;
    this->RotationInertia = 0.5f * mass * radius * radius;
    this->type = CIRCLE;
    this->torque = 0;
    this->angle_velocity = 0;
    this->is_static=false;
}

void Circle::update(float dt){
    angle_velocity += torque/RotationInertia;
    velocity += force/mass;
    velocity =clamp(velocity,-radius/dt,radius/dt);
    position += velocity * dt;
    angle += angle_velocity * dt;
    force = vec2(0,0);
    torque = 0;

}


float Circle::getRadius() const{
    return radius;
}

}// namespace phy2d