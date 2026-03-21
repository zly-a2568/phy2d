#pragma once
#include <glm/glm.hpp>
#include "../Object/Object.hpp"
#include "../Object/Circle.hpp"
#include "../Object/Line.hpp"
#include "../Object/Box.hpp"
using namespace glm;

#include <iostream>

namespace phy2d{
    class Collision{
    public:

        static float cross2d(vec2 a, vec2 b){
            return a.x * b.y - a.y * b.x;
        }

        static bool Line1OnLine2(vec2 a1, vec2 a2, vec2 b1, vec2 b2){
            vec2 ab1=a1 - b1;
            vec2 ab2=a2 - b1;
            vec2 b=b2 - b1;
            float lb=length(b2-b1);
            float an1=dot(ab1,b)/lb;
            float an2=dot(ab2,b)/lb;
            if (glm::max(an1,an2) <= 0.0f || glm::min(an1,an2) >= lb)return false;
            return true;
        }

        static std::pair<float,float> getPolygonProjection(std::vector<vec2>& vertices, vec2 p1, vec2 p2){
            std::pair<float,float> result;
            for(auto v:vertices){
                float x=dot(v-p1,p2-p1)/length(p2-p1);
                if ((!result.first) ||(!result.second)){
                    result.first=x;
                    result.second=x;
                }
                else{
                    result.first=glm::min(result.first,x);
                    result.second=glm::max(result.second,x);
                }
            }
            return result;
        }

        static bool PointOnLine(vec2 p, vec2 a, vec2 b){
            vec2 ab = b - a;
            float t = dot(p - a, ab) / dot(ab, ab);
            if(t < 0.0f||t > 1.0f)return false;
            return true;
        }

        static vec2 ClosestPointOnLineSegment(vec2 p, vec2 a, vec2 b){
            vec2 ab = b - a;
            float t = dot(p - a, ab) / dot(ab, ab);
            if(t < 0.0f) t = 0.0f;
            if(t > 1.0f) t = 1.0f;
            return a + t * ab;
        }
        //AABB
        static bool AABBvsAABB(vec2 aMin, vec2 aMax, vec2 bMin, vec2 bMax);
        //Circle
        static bool CirclevsCircle(vec2 aPos, float aRadius, vec2 bPos, float bRadius){
            return length(aPos - bPos) < aRadius + bRadius;
        }
        //Polugon vs Line
        static bool PolygonsvsLine(vec2* aPoints, vec2 bPos, vec2 bDir, int aSize){
            for(int i = 0; i < aSize; i++){
                if(LinevsLine(aPoints[i], aPoints[(i + 1) % aSize], bPos, bPos + bDir)) return true;
            }
            return false;
        }
        //Line vs Line
        static bool LinevsLine(vec2 aStart, vec2 aEnd, vec2 bStart, vec2 bEnd){
            float denominator = (bEnd.y - bStart.y) * (aEnd.x - aStart.x) - (bEnd.x - bStart.x) * (aEnd.y - aStart.y);
            if(denominator == 0) return false;
            float ua = ((bEnd.x - bStart.x) * (aStart.y - bStart.y) - (bEnd.y - bStart.y) * (aStart.x - bStart.x)) / denominator;
            float ub = ((aEnd.x - aStart.x) * (aStart.y - bStart.y) - (aEnd.y - aStart.y) * (aStart.x - bStart.x)) / denominator;
            return ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1;
        }

        //Circle vs Line
        static int CirclevsLine(vec2 aPos, float aRadius, vec2 bStart, vec2 bEnd){
            vec2 ab = bEnd - bStart;
            vec2 ac = aPos - bStart;
            float c_proj=dot(ac, ab)/length(ab);
            if (c_proj < -aRadius || c_proj > length(ab) + aRadius) return -1;
            if (c_proj < 0){
                if(length(ac) < aRadius) return 0;
            }
            else if (c_proj > length(ab)){
                vec2 ad = bEnd - aPos;
                if (length(ad) < aRadius) return 2;
            }
            else{
                vec2 cd = ac - ab * (c_proj / length(ab));
                if (length(cd) < aRadius) return 1;
            }
            return -1;
        }

        //AABB vs Circle
        static bool AABBvsCircle(vec2 aMin, vec2 aMax, vec2 bPos, float bRadius){
            float x = std::max(aMin.x, std::min(bPos.x, aMax.x));
            float y = std::max(aMin.y, std::min(bPos.y, aMax.y));
            float dx = bPos.x - x;
            float dy = bPos.y - y;
            return dx * dx + dy * dy <= bRadius * bRadius;
        }
        static std::vector<float> collideDynamicWithDynamic(float depth,vec2 normal,vec2 cpoint,vec2 aPos, vec2 bPos, vec2 aVel, vec2 bVel,float aMass,float bMass,float aAngleVel,float bAngleVel,float aInertia,float bInertia,float restitution,float friction){
            std::vector<float> res;
            vec2 fixed_aPos = aPos + normal * depth * 0.5f;
            vec2 fixed_bPos = bPos - normal * depth * 0.5f;

            vec2 t=vec2(normal.y, -normal.x);
            std::cout<<t.x<<" "<<t.y<<std::endl;

            vec3 r1 = vec3(cpoint.x,cpoint.y,0.0f) - vec3(aPos.x, aPos.y, 0.0f);
            vec3 r2 = vec3(cpoint.x,cpoint.y,0.0f) - vec3(bPos.x, bPos.y, 0.0f);
            
            vec3 vc1= vec3(aVel.x, aVel.y, 0.0f) + cross(vec3(0,0,aAngleVel), r1);
            vec3 vc2= vec3(bVel.x, bVel.y, 0.0f) + cross(vec3(0,0,bAngleVel), r2);

            vec3 vc_rel= vc1 - vc2;
            std::cout << vc_rel.x << " " << vc_rel.y << " " << vc_rel.z << std::endl;


            float vc_rel_n = dot(vc_rel, vec3(normal.x, normal.y, 0.0f));
            
            float vc_rel_t = dot(vec2(vc_rel.x, vc_rel.y), t);
            std::cout << vc_rel_n << " " << vc_rel_t << std::endl;

            float tdenom = (1/aMass + 1/bMass + (length(r1)*length(r1))/aInertia +(length(r2)*length(r2))/bInertia);
            float ndenom = (1/aMass + 1/bMass);

            float j_n = -(1+restitution) * vc_rel_n / ndenom;

            float j = -vc_rel_t / tdenom;

            if(abs(j) >abs(j_n)*friction){
                j=sign(j)*abs(j_n)*friction;
            }
            std::cout << j << std::endl;

            vec3 impulsen= j_n * vec3(normal.x, normal.y, 0.0f);
            vec3 impulset= j * vec3(t.x, t.y, 0.0f);
            std::cout << impulsen.x << " " << impulsen.y << " " << impulsen.z << std::endl;
            std::cout << impulset.x << " " << impulset.y << " " << impulset.z << std::endl;

            vec3 newaVel =  impulsen / aMass;
            vec3 newbVel = - impulsen / bMass;
            res.push_back(newaVel.x);
            res.push_back(newaVel.y);
            res.push_back(newbVel.x);
            res.push_back(newbVel.y);

            vec3 newaAngleVel = cross(r1, impulset) / aInertia;
            vec3 newbAngleVel = cross(r2, -impulset) / bInertia;

            res.push_back(newaAngleVel.z);
            res.push_back(newbAngleVel.z);
            /*vec2 nv1= dot(normal, aVel)/length(normal)*normal;
            vec2 tv1= dot(t, aVel)/length(t)*t;
            vec2 nv2= dot(normal, bVel)/length(normal)*normal;
            vec2 tv2= dot(t, bVel)/length(t)*t;
            vec2 newaVel= ((aMass-bMass)*nv1+ 2*bMass*nv2) / (aMass+bMass)+ tv1;
            vec2 newbVel= ((bMass-aMass)*nv2+ 2*aMass*nv1) / (aMass+bMass)+ tv2;
            res.push_back(newaVel.x);
            res.push_back(newaVel.y);
            res.push_back(newbVel.x);
            res.push_back(newbVel.y);
            vec3 r1 =vec3(cpoint.x, cpoint.y, 0.0f)-vec3(aPos.x, aPos.y, 0.0f);
            vec3 r2 =vec3(cpoint.x, cpoint.y, 0.0f)-vec3(bPos.x, bPos.y, 0.0f);
            vec3 v1 = vec3(aVel.x, aVel.y, 0.0f) + cross(vec3(0,0,aAngleVel), r1);
            vec3 v2 = vec3(bVel.x, bVel.y, 0.0f) + cross(vec3(0,0,bAngleVel), r2);
            vec3 vRel = v1 - v2;
            float vc_rel_t = dot(vRel, vec3(t.x, t.y, 0.0f));
            float j_t = -vc_rel_t / (1/aMass + 1/bMass + pow(length(r1), 2)/aInertia + pow(length(r2), 2)/bInertia)*friction;
            vec3 impulse_t = j_t * vec3(t.x, t.y, 0.0f);
            vec3 newaAngleVel = vec3(0,0,aAngleVel) + cross(r1, impulse_t) / aInertia;
            vec3 newbAngleVel = vec3(0,0,bAngleVel) + cross(r2, impulse_t) / bInertia;
            res.push_back(newaAngleVel.z);
            res.push_back(newbAngleVel.z);*/




            res.push_back(fixed_aPos.x);
            res.push_back(fixed_aPos.y);
            res.push_back(fixed_bPos.x);
            res.push_back(fixed_bPos.y);


            return res;
        }
        
        static std::vector<float> collideDynamicWithStatic(float depth,vec2 normal,vec2 cpoint,vec2 aPos,vec2 bPos, vec2 aVel,float aMass,float bMass,float aAngleVel,float bAngleVel,float aInertia,float bInertia,float restitution,float friction){
            std::vector<float> res;
            vec2 fixed_aPos = aPos + normal * depth;
            vec2 t = vec2(normal.y, -normal.x);
            vec2 nv1=-dot(normal, aVel)/length(normal)*restitution*normal;
            vec2 tv1=dot(t, aVel)/length(t)*t;
            vec2 newaVel = nv1+tv1;
            res.push_back(newaVel.x);
            res.push_back(newaVel.y);

            vec2 r1 = cpoint-aPos;

            vec3 rt = cross(vec3(r1.x, r1.y, 0), vec3(0, 0, aAngleVel))*friction;
            vec3 newaAngleVel = vec3(0,0,aAngleVel)+cross(rt,vec3(r1.x,r1.y, 0))/aInertia;

            res.push_back(newaAngleVel.z);
            res.push_back(fixed_aPos.x);
            res.push_back(fixed_aPos.y);


            return res;
        }

        static std::vector<float> collideCircleWithLine(vec2 cpoint , vec2 aPos, float aRadius,float aMass, vec2 aVel,float aAngleVel, vec2 bStart, vec2 bEnd) {
            vec2 normal = normalize(vec2(cpoint.x, cpoint.y) - aPos);
            float depth = length(vec2(cpoint - aPos)) - aRadius;
            vec2 fixed_delta = depth * normal;
            float nv1=dot(aVel, normal)/length(normal);
            vec2 t=vec2(normal.y, -normal.x);
            vec2 tv1= dot(aVel, t)/length(t)*t;
            vec2 newaVel=tv1+nv1*(-normal)*0.9f;
            std::vector<float> res;
            res.push_back(newaVel.x);
            res.push_back(newaVel.y);
            res.push_back(aAngleVel);
            res.push_back(aPos.x + fixed_delta.x);
            res.push_back(aPos.y + fixed_delta.y);
            
            return res;

        }
        static void collide(Object*a,Object*b){
        
            if (a->type == CIRCLE && b->type == CIRCLE) {
                Circle* ac = dynamic_cast<Circle*>(a);
                Circle* bc = dynamic_cast<Circle*>(b);
                if (!CirclevsCircle(ac->position, ac->radius, bc->position, bc->radius)) return;
                vec2 aPos= ac->getPosition();
                vec2 aVel= ac->getVelocity();
                vec2 bPos= bc->getPosition();
                vec2 bVel= bc->getVelocity();
                float aRadius= ac->getRadius();
                float bRadius= bc->getRadius();
                float aMass= ac->getMass();
                float bMass= bc->getMass();
                float aAngleVel= ac->getAngleVelocity();
                float bAngleVel= bc->getAngleVelocity();
                auto newVel = collideDynamicWithDynamic(length(aPos-bPos)-(ac->getRadius()+bc->getRadius()),normalize(bPos-aPos),
                        aPos + normalize(bPos-aPos) * aRadius,
                        aPos,
                        bPos,
                        aVel,
                        bVel,
                        aMass,
                        bMass,
                        aAngleVel,
                        bAngleVel,
                        ac->getRotationInertia(),
                        bc->getRotationInertia(),
                        0.99f,
                        0.5f);
                ac->setPosition(newVel[6], newVel[7]);
                bc->setPosition(newVel[8], newVel[9]);
                ac->addVelocity(newVel[0], newVel[1]);
                bc->addVelocity(newVel[2], newVel[3]);
                ac->addAngleVelocity(newVel[4]);
                bc->addAngleVelocity(newVel[5]);
            }
            if ((a->type == CIRCLE && b->type == LINE)||(a->type == LINE && b->type == CIRCLE)) {
                Line* l;
                Circle* c;
                if (a->type == LINE) {
                    l = dynamic_cast<Line*>(a);
                    c = dynamic_cast<Circle*>(b);
                }
                else{
                    l = dynamic_cast<Line*>(b);
                    c = dynamic_cast<Circle*>(a);
                }

                int result=CirclevsLine(c->getPosition(), c->getRadius(), l->getPoint1(),l->getPoint2());
                if(result==-1)return;


                vec2 cpoint = vec3();
                if (result==0) {
                    cpoint = l->getPoint1();
                }
                else if (result==2) {
                    cpoint = l->getPoint2();
                }
                else{
                    //cpoint
                    auto cp = ClosestPointOnLineSegment(c->getPosition(),l->getPoint1(),l->getPoint2());
                    cpoint = vec3(cp.x,cp.y,0);
                }
                vec2 aPos = c->getPosition();
                vec2 bPos = l->getPosition();
                float aRadius = c->getRadius();
                float aMass = c->getMass();
                float bMass = l->getMass();
                vec2 aVel = c->getVelocity();
                float aAngleVel = c->getAngleVelocity();
                float rb= length(bPos-cpoint);

                //auto newVel = collideCircleWithLine(cpoint,c->getPosition(),c->getRadius(),c->getMass(),c->getVelocity(),c->getAngleVelocity(),l->getPoint1(),l->getPoint2());
                auto newVel = collideDynamicWithStatic(length(aPos-cpoint)-aRadius,
                normalize(cpoint-aPos),
                cpoint,
                aPos,
                bPos,
                aVel,
                aMass,
                bMass,
                aAngleVel,
                0,
                c->getRotationInertia(),
                bMass*rb*rb/2,
                1.0f,
                1.0f);
                c->setPosition(newVel[3],newVel[4]);
                //c->setAngleVelocity(newVel[2]);
                c->setVelocity(newVel[0],newVel[1]);

            }
            if ((a->getType() == BodyType::CIRCLE && b->getType() == BodyType::BOX)||(b->getType() == BodyType::CIRCLE && a->getType() == BodyType::BOX)) {
                Box* bx;
                Circle* c;
                if (a->type == BodyType::BOX) {
                    bx = dynamic_cast<Box*>(a);
                    c = dynamic_cast<Circle*>(b);
                }
                else{
                    bx = dynamic_cast<Box*>(b);
                    c = dynamic_cast<Circle*>(a);
                }
                vec2 cpos= c->getPosition();
                float cRadius = c->getRadius();
                vec2 cpoint;
                vec2 normal;
                auto b_vertices = bx->getVertices();
                vec2 cp1=ClosestPointOnLineSegment(cpos,b_vertices[0],b_vertices[1]);
                vec2 cp2=ClosestPointOnLineSegment(cpos,b_vertices[1],b_vertices[2]);
                vec2 cp3=ClosestPointOnLineSegment(cpos,b_vertices[2],b_vertices[3]);
                vec2 cp4=ClosestPointOnLineSegment(cpos,b_vertices[3],b_vertices[0]);
                float d1 = length(cpos-cp1);
                float d2 = length(cpos-cp2);
                float d3 = length(cpos-cp3);
                float d4 = length(cpos-cp4);
                float m1 = glm::min(d1,d2);
                float m2 = glm::min(d3,d4);
                float m = glm::min(m1,m2);
                if(m>cRadius)return;
                if(m==d1){
                    cpoint=cp1;
                    if (PointOnLine(cpoint,b_vertices[0],b_vertices[1])){
                        vec2 t = normalize(b_vertices[1]-b_vertices[0]);
                        normal = vec2(-t.y,t.x);
                    }
                    else{
                        normal=normalize(cp1-cpos);
                    }
                }
                else if(m==d2){
                    cpoint=cp2;
                    if (PointOnLine(cpoint,b_vertices[1],b_vertices[2])){
                        vec2 t = normalize(b_vertices[2]-b_vertices[1]);
                        normal = vec2(-t.y,t.x);
                    }
                    else{
                        normal=normalize(cp2-cpos);
                    }
                }
                else if(m==d3){
                    cpoint=cp3;
                    if (PointOnLine(cpoint,b_vertices[2],b_vertices[3])){
                        vec2 t = normalize(b_vertices[3]-b_vertices[2]);
                        normal = vec2(-t.y,t.x);
                    }
                    else{
                        normal=normalize(cp3-cpos);
                    }
                }
                else{
                    cpoint=cp4;
                    if (PointOnLine(cpoint,b_vertices[3],b_vertices[0])){
                        vec2 t = normalize(b_vertices[0]-b_vertices[3]);
                        normal = vec2(-t.y,t.x);
                    }
                    else{
                        normal=normalize(cp4-cpos);
                    }
                }
                float depth = m-cRadius;
                auto newVel = collideDynamicWithDynamic(depth,normal,cpoint,cpos,bx->getPosition(),c->getVelocity(),bx->getVelocity(),c->getMass(),
                bx->getMass(),c->getAngleVelocity(),bx->getAngleVelocity(),c->getRotationInertia(),bx->getRotationInertia(),0.5f,1.0f);
                c->setPosition(newVel[6], newVel[7]);
                bx->setPosition(newVel[8], newVel[9]);
                c->setVelocity(newVel[0], newVel[1]);
                bx->setVelocity(newVel[2], newVel[3]);
                c->setAngleVelocity(newVel[4]);
                bx->setAngleVelocity(newVel[5]);
            }
            if ((a->getType() == BodyType::LINE && b->getType() == BodyType::BOX)||(b->getType() == BodyType::LINE && a->getType() == BodyType::BOX)){
                return;
                Box* bx;
                Line* l;
                if (a->type == BodyType::BOX) {
                    bx = dynamic_cast<Box*>(a);
                    l = dynamic_cast<Line*>(b);
                }
                else{
                    bx = dynamic_cast<Box*>(b);
                    l = dynamic_cast<Line*>(a);
                }
                auto b_vertices= bx->getVertices();
                std::vector<std::pair<vec2,vec2>> edges;
                vec2 l_n=normalize(l->getPoint1()-l->getPoint2());
                vec2 l_t=vec2(-l_n.y, l_n.x);
                std::vector<vec2> l_p={l->getPoint1(), l->getPoint2()};
                auto bx_proj=getPolygonProjection(b_vertices,vec2(0,0),l_t);
                auto l_proj=getPolygonProjection(l_p,vec2(0,0),l_t);
                if (bx_proj.first > l_proj.second || bx_proj.second < l_proj.first) return;
                else if (bx_proj.first == l_proj.first|| bx_proj.second == l_proj.second){
                    vec2 normal=l_t;
                    vec2 cpoint;
                    std::vector<vec2> cpoints;
                    float k1=l_n.y/l_n.x;
                    for(auto& v: b_vertices){
                        vec2 ab=l->getPoint1()-v;
                        float k2=ab.y/ab.x;
                        if (k1-k2<0.001) cpoints.push_back(v);
                    }
                    if (cpoints.size()==0) return;
                    else if (cpoints.size()==1) cpoint=cpoints[0];
                    else cpoint = (cpoints[0]+cpoints[1])*0.5f;
                    auto closet = ClosestPointOnLineSegment(cpoint,l->getPoint1(),l->getPoint2());
                    auto newVel = collideDynamicWithStatic(-length(cpoint-closet),normal,cpoint,bx->getPosition(),l->getPosition(),bx->getVelocity(),bx->getMass(),
                    l->getMass(),bx->getAngleVelocity(),l->getAngleVelocity(),bx->getRotationInertia(),l->getRotationInertia(),0.2f,0.8f);
                    bx->setPosition(newVel[3],newVel[4]);
                    bx->setAngleVelocity(newVel[2]);
                    bx->setVelocity(newVel[0],newVel[1]);
                }
            }
        
        }
    };
}