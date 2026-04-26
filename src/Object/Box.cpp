#include "../../include/Object/Box.hpp"

namespace phy2d{
Box::Box(float x ,float y, float width, float height, float mass){
    this->position = vec2(x, y);
    this->velocity = vec2(0, 0);
    this->size = vec2(width, height);
    this->angle=0;
    this->angle_velocity=0;
    this->torque=0;
    this->force=vec2(0, 0);
    this->mass = mass;
    this->RotationInertia = (1.0f / 12.0f) * mass * (size.x * size.x + size.y * size.y);
    this->type = BOX;
    this->is_static=false;
}

void Box::update(float dt){
    angle_velocity+=torque/RotationInertia;
    velocity+=force/mass;
    position+=velocity*dt;
    angle+=angle_velocity*dt;
    force = vec2(0,0);
    torque = 0;
    auto vertices=getVertices();
    float l=glm::min(glm::min(vertices[0].x,vertices[1].x),glm::min(vertices[2].x,vertices[3].x));
    float r=glm::max(glm::max(vertices[0].x,vertices[1].x),glm::max(vertices[2].x,vertices[3].x));
    float b=glm::min(glm::min(vertices[0].y,vertices[1].y),glm::min(vertices[2].y,vertices[3].y));
    float t=glm::max(glm::max(vertices[0].y,vertices[1].y),glm::max(vertices[2].y,vertices[3].y));
    bounding_box=AABB(l,r,b,t);
}

vec2 Box::getSize()const{
    return size;
}

std::vector<vec2> Box::getVertices(){
    std::vector<vec2> vertices;
    vec2 ne = size*0.5f;
    vec2 nw = vec2(-ne.x, ne.y);
    vec2 sw = vec2(-ne.x, -ne.y);
    vec2 se = vec2(ne.x, -ne.y);
    vec2 new_ne = vec2(ne.x*cos(angle)-ne.y*sin(angle),ne.x*sin(angle)+ne.y*cos(angle));
    vec2 new_nw = vec2(nw.x*cos(angle)-nw.y*sin(angle),nw.x*sin(angle)+nw.y*cos(angle));
    vec2 new_sw = vec2(sw.x*cos(angle)-sw.y*sin(angle),sw.x*sin(angle)+sw.y*cos(angle));
    vec2 new_se = vec2(se.x*cos(angle)-se.y*sin(angle),se.x*sin(angle)+se.y*cos(angle));
    vertices.push_back(position+new_ne);
    vertices.push_back(position+new_nw);
    vertices.push_back(position+new_sw);
    vertices.push_back(position+new_se);
    return vertices;

}

std::vector<std::pair<vec2, vec2>> phy2d::Box::getEdges()
{
    std::vector<std::pair<vec2, vec2>> edges;
    std::vector<vec2> vertices = getVertices();
    edges.push_back(std::make_pair(vertices[0], vertices[1]));
    edges.push_back(std::make_pair(vertices[1], vertices[2]));
    edges.push_back(std::make_pair(vertices[2], vertices[3]));
    edges.push_back(std::make_pair(vertices[3], vertices[0]));
    return edges;
}


}


