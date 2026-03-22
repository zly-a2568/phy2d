#pragma once

#include <glm/glm.hpp>
#include "../Object/Object.hpp"
using namespace glm;

namespace phy2d{
    class CollideInfo {
public:
    float cross2d(vec2 a, vec2 b) {
        return a.x * b.y - a.y * b.x;
    }
    CollideInfo(){
        this->obj1 = nullptr;
        this->obj2 = nullptr;
        this->normal = vec2(0, 0);
        this->depth = 0;
        this->contact = vec2(0, 0);
    }
    CollideInfo(Object* obj1, Object* obj2, vec2 normal, float depth, vec2 contact) {
        this->obj1 = obj1;
        this->obj2 = obj2;
        this->normal = normal;
        this->depth = depth;
        this->contact = contact;
        this->restitution = 0.5f;
        this->friction = 0.5f;
    }
    void generate(){
        if(!obj1||!obj2) return;
        if(obj1->is_static&&obj2->is_static)return;
        if(obj1->is_static||obj2->is_static){
            generate_static();
        }else{
            generate_dynamic();
        }
    }
    void generate_dynamic(){
            if(!obj1||!obj2) return;
            vec2 t=vec2(normal.y, -normal.x);
            

            vec3 r1 = vec3(contact.x,contact.y,0.0f) - vec3(obj1->position.x, obj1->position.y, 0.0f);
            vec3 r2 = vec3(contact.x,contact.y,0.0f) - vec3(obj2->position.x, obj2->position.y, 0.0f);
            
            vec3 vc1= vec3(obj1->getVelocity().x, obj1->getVelocity().y, 0.0f) + cross(vec3(0,0,obj1->getAngleVelocity()), r1);
            vec3 vc2= vec3(obj2->getVelocity().x, obj2->getVelocity().y, 0.0f) + cross(vec3(0,0,obj2->getAngleVelocity()), r2);

            vec3 vc_rel= vc1 - vc2;

            

            float vc_rel_n = dot(vc_rel, vec3(normal.x, normal.y, 0.0f));
            
            float vc_rel_t = dot(vec2(vc_rel.x, vc_rel.y), t);
            float i_a = obj1->getRotationInertia();
            float i_b = obj2->getRotationInertia();
            float invm1 = 1/obj1->getMass();
            float invm2 = 1/obj2->getMass();
            float inv_i_a = 1/i_a;
            float inv_i_b = 1/i_b;
            if(obj1->is_static){
                invm1 = 0.0f;
                inv_i_a = 0.0f;
            }
            if(obj2->is_static){
                invm2 = 0.0f;
                inv_i_b = 0.0f;
            }

            float tdenom = (invm1 + invm2 + (length(r1)*length(r1))*inv_i_a +(length(r2)*length(r2))*inv_i_b);
            float ndenom = (invm1 + invm2);

            float j_n = -(1+restitution) * vc_rel_n / ndenom;

            float j = -vc_rel_t / tdenom;

            if(abs(j) >abs(j_n)*friction){
                j=sign(j)*abs(j_n)*friction;
            }

            vec3 impulsen= j_n * vec3(normal.x, normal.y, 0.0f);
            vec3 impulset= j * vec3(t.x, t.y, 0.0f);
            vec3 impulse= impulsen + impulset;

            vec3 newaVel =  impulse / obj1->getMass();
            vec3 newbVel = - impulse / obj2->getMass();
            vec3 newaAngleVel = cross(r1, impulse) / i_a;
            vec3 newbAngleVel = cross(r2, -impulse) / i_b;
            obj1->velocity+= vec2(newaVel);
            obj2->velocity+= vec2(newbVel);
            obj1->angle_velocity+= newaAngleVel.z;
            obj2->angle_velocity+= newbAngleVel.z;
            vec2 fixed_p1= obj1->position+normal*depth*0.5f;
            vec2 fixed_p2= obj2->position-normal*depth*0.5f;
            obj1->position = fixed_p1;
            obj2->position = fixed_p2;

            
    }
    void generate_static(){
            if(!obj1||!obj2) return;
            vec2 t=vec2(normal.y, -normal.x);
            if (obj1->is_static&&obj2->is_static){
                return;
            }
            if(obj1->is_static){
                std::swap(obj1, obj2);
            }

            vec2 fixed_p1= obj1->position + normal*depth;
            obj1->position = fixed_p1;
            vec3 r1 = vec3(contact.x,contact.y,0.0f) - vec3(obj1->position.x, obj1->position.y, 0.0f);
            
            vec3 vc1= vec3(obj1->getVelocity().x, obj1->getVelocity().y, 0.0f) + cross(vec3(0,0,obj1->getAngleVelocity()), r1);

            vec3 vc_rel= vc1;

            

            float vc_rel_n = dot(vc_rel, vec3(normal.x, normal.y, 0.0f));
            
            float vc_rel_t = dot(vec2(vc_rel.x, vc_rel.y), t);
            float i_a = obj1->getRotationInertia();
            float invm1 = 1/obj1->getMass();
            float inv_i_a = 1/i_a;

            float tdenom = (invm1+ (length(r1)*length(r1))*inv_i_a);
            float ndenom = invm1;

            float j_n = -(1+restitution) * vc_rel_n / ndenom;

            float j = -vc_rel_t / tdenom;

            if(abs(j) >abs(j_n)*friction){
                j=sign(j)*abs(j_n)*friction;
            }

            vec3 impulsen= j_n * vec3(normal.x, normal.y, 0.0f);
            vec3 impulset= j * vec3(t.x, t.y, 0.0f);
            vec3 impulse= impulsen + impulset;

            vec3 newaVel =  impulse / obj1->getMass();
            vec3 newaAngleVel = cross(r1, impulse) / i_a;
            
            obj1->velocity += vec2(newaVel);    
            obj1->angle_velocity += newaAngleVel.z;
            
            
            
    }
    
    Object* obj1;
    Object* obj2;
protected:
    vec2 normal;
    float depth;
    vec2 contact;
    float restitution;
    float friction;
};

}