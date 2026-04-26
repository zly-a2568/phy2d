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

void Object::applyImpulse(vec2 p,vec2 n)
{
    
}

float Object::getRotationInertia() const
{
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

bool Object::isStatic() const
{
    return is_static;
}

bool Object::isAwake() const
{
    return awake;
}
void Object::setAwake(bool a)
{
    if (a) {
        awake = true;
        sleep_time = 0.0f;
    } else {
        awake = false;
        velocity.x=0.0f;
        velocity.y=0.0f;
        angle_velocity=0.0f;
    }
}

void Object::updateSleepTime(float dt) {
    float energy=0.5f*length(velocity)*length(velocity)*mass+0.5f*angle_velocity*angle_velocity*RotationInertia;
    if (energy<100.0f) {
        sleep_time+=dt;
        if (sleep_time>=0.1f) {
            setAwake(false);
        }
    }
    else {
        sleep_time=0.0f;
        if (!awake)setAwake(true);
    }
}

void Object::setVelocity(float vx, float vy)
{
    velocity.x=vx;
    velocity.y=vy;
}
}