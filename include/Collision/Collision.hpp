#pragma once
#include <glm/glm.hpp>
#include "../Object/Object.hpp"
#include "../Object/Circle.hpp"
#include "../Object/Line.hpp"
#include "../Object/Box.hpp"
#include "CollideInfo.hpp"
using namespace glm;

#include <iostream>
#include <algorithm>

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

        static std::pair<float,float> getPolygonProjection(std::vector<vec2> vertices, vec2 p){
            std::pair<float,float> result;
            std::vector<float> points;
            for(auto v:vertices){
                float x=dot(v,p)/length(p);
                points.push_back(x);
            }
            std::sort(points.begin(),points.end());
            result.first=*points.begin();
            result.second=*points.rbegin();
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
            vec3 impulse= impulsen + impulset;

            vec3 newaVel =  impulse / aMass;
            vec3 newbVel = - impulse / bMass;
            res.push_back(newaVel.x);
            res.push_back(newaVel.y);
            res.push_back(newbVel.x);
            res.push_back(newbVel.y);

            vec3 newaAngleVel = cross(r1, impulse) / aInertia;
            vec3 newbAngleVel = cross(r2, -impulse) / bInertia;

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
        
        static std::vector<float> collideDynamicWithStatic(float depth,vec2 normal,vec2 cpoint,vec2 aPos,vec2 bPos, vec2 aVel,float aMass,float aAngleVel,float aInertia,float restitution,float friction){
            std::vector<float> res;
            vec2 fixed_aPos = aPos + normal * depth;

            vec2 t=vec2(normal.y, -normal.x);

            vec3 r1 = vec3(cpoint.x,cpoint.y,0.0f) - vec3(aPos.x, aPos.y, 0.0f);
            
            vec3 vc1= vec3(aVel.x, aVel.y, 0.0f) + cross(vec3(0,0,aAngleVel), r1);

            vec3 vc_rel= vc1;


            float vc_rel_n = dot(vc_rel, vec3(normal.x, normal.y, 0.0f));
            
            float vc_rel_t = dot(vec2(vc_rel.x, vc_rel.y), t);

            float tdenom = (1/aMass +(length(r1)*length(r1))/aInertia);
            float ndenom = 1/aMass;

            float j_n = -(1+restitution) * vc_rel_n / ndenom;

            float j = -vc_rel_t / tdenom;

            if(abs(j) >abs(j_n)*friction){
                j=sign(j)*abs(j_n)*friction;
            }

            vec3 impulsen= j_n * vec3(normal.x, normal.y, 0.0f);
            vec3 impulset= j * vec3(t.x, t.y, 0.0f);
            vec3 newaVel = (impulsen+impulset) / aMass;
            res.push_back(newaVel.x);
            res.push_back(newaVel.y);

            vec3 newaAngleVel = cross(r1, (impulset+impulsen) / aInertia);

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
        static CollideInfo collide(Object*a,Object*b){
        
            if (a->type == CIRCLE && b->type == CIRCLE) {
                Circle* ac = dynamic_cast<Circle*>(a);
                Circle* bc = dynamic_cast<Circle*>(b);
                if (!CirclevsCircle(ac->position, ac->radius, bc->position, bc->radius)) return CollideInfo(nullptr,nullptr,vec2(0,0),0,vec2(0,0));
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
                auto c_info= CollideInfo(ac,bc,normalize(bPos-aPos),length(bPos-aPos)-(aRadius+bRadius),aPos + normalize(bPos-aPos) * aRadius);
                return c_info;
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
                if(result==-1)return CollideInfo(nullptr,nullptr,vec2(0,0),0,vec2(0,0));


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
                auto c_info= CollideInfo(c,l,normalize(cpoint-aPos),length(cpoint-aPos)-aRadius,cpoint);
                return c_info;

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
                std::vector<std::pair<vec2,vec2>> b_edges = bx->getEdges();
                for(auto& edge:b_edges){
                    vec2 closet=ClosestPointOnLineSegment(c->getPosition(),edge.first,edge.second);
                    if (length(closet-c->getPosition())<=c->getRadius()){
                        return CollideInfo(c,bx,normalize(closet-c->getPosition()),length(closet-c->getPosition())-c->getRadius(),closet);
                    }
                }
                return CollideInfo(nullptr,nullptr,vec2(0,0),0,vec2(0,0));
                
            }
            if ((a->getType() == BodyType::LINE && b->getType() == BodyType::BOX)||(b->getType() == BodyType::LINE && a->getType() == BodyType::BOX)){
                return CollideInfo(nullptr,nullptr,vec2(0,0),0,vec2(0,0));
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
                auto edges= bx->getEdges();
                for (auto edge : edges){
                    vec2 edge_normal= normalize(edge.second-edge.first);
                    vec2 edge_tangent= normalize(vec2(-edge_normal.y,edge_normal.x));
                    auto bx_proj= getPolygonProjection(bx->getVertices(),edge_normal);
                    if(bx_proj.first > length(l->getPoint2()-l->getPoint1())||bx_proj.second < 0){
                        return CollideInfo(nullptr,nullptr,vec2(0,0),0,vec2(0,0));
                    }
                    else if(bx_proj.first < 0||bx_proj.second > length(l->getPoint2()-l->getPoint1())){
                        if(PointOnLine(l->getPoint1(),edge.first,edge.second)){
                            vec2 normal= normalize(edge.second-bx->getPosition()+edge.first-bx->getPosition());
                            float depth = -length(l->getPoint1()-ClosestPointOnLineSegment(l->getPoint1(),edge.first,edge.second));
                            return CollideInfo(bx,l,normal,depth,l->getPoint1());
                        }
                        if(PointOnLine(l->getPoint2(),edge.first,edge.second)){
                            vec2 normal= normalize(edge.second-bx->getPosition()+edge.first-bx->getPosition());
                            float depth = -length(l->getPoint2()-ClosestPointOnLineSegment(l->getPoint2(),edge.first,edge.second));
                            return CollideInfo(bx,l,normal,depth,l->getPoint2());
                        }
                    }
                    else{
                        if(PointOnLine(edge.first,l->getPoint1(),l->getPoint2())&&PointOnLine(edge.second,l->getPoint1(),l->getPoint2())){
                            vec2 normal= normalize(l->getPoint2()-l->getPoint1());
                            float depth1 = -length(edge.first-ClosestPointOnLineSegment(edge.first,l->getPoint1(),l->getPoint2()));
                            float depth2 = -length(edge.second-ClosestPointOnLineSegment(edge.second,l->getPoint1(),l->getPoint2()));
                            return CollideInfo(bx,l,normal,(depth1+depth2)/2,edge.first*0.5f+edge.second*0.5f);
                        }
                        else{
                            if(PointOnLine(edge.first,l->getPoint1(),l->getPoint2())){
                                vec2 normal= normalize(l->getPoint2()-l->getPoint1());
                                float depth = -length(edge.first-ClosestPointOnLineSegment(edge.first,l->getPoint1(),l->getPoint2()));
                                return CollideInfo(bx,l,normal,depth,edge.first);
                            }
                            else{
                                if(PointOnLine(edge.second,l->getPoint1(),l->getPoint2())){
                                    vec2 normal= normalize(l->getPoint2()-l->getPoint1());
                                    float depth = -length(edge.second-ClosestPointOnLineSegment(edge.second,l->getPoint1(),l->getPoint2()));
                                    return CollideInfo(bx,l,normal,depth,edge.second);
                                }
                            }
                        }

                    }

                    
                }
            }
            return CollideInfo(nullptr,nullptr,vec2(0,0),0,vec2(0,0));
        }
    };



}