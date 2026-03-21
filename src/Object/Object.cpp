#include "../../include/Object/Object.hpp"
namespace phy2d {
vec2 Object::getPosition() const {
    return position;
}

float Object::getMass() const {
    return mass;
}

void Object::setPosition(float x, float y) {
    position.x=x;
    position.y=y;
}

void Object::setMass(float m) {
    mass=m;
}

float Object::getAngleVelocity() const {
    return angle_velocity;
}

void Object::setAngleVelocity(float av) {
    angle_velocity=av;
}

float Object::getAngle() const {
    return angle;
}

void Object::setAngle(float a) {
    angle=a;
}

 void Object::applyTorque(float t)
{
    torque+=t;
}

void Object::applyForce(vec2 f)
{
    force+=f;
}

float Object::getRotationInertia() const{
    return RotationInertia;
}

BodyType Object::getType()
{
    return type;
}

vec2 Object::getVelocity() const
{
    return velocity;
}

void Object::addVelocity(float vx, float vy){
    tmp_v+= vec2(vx,vy);
}

void Object::addAngleVelocity(float av){
    tmp_av+=av;
}

void Object::setVelocity(float vx, float vy) {
    velocity.x=vx;
    velocity.y=vy;
}
}