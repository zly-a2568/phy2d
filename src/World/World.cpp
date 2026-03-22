#include "../../include/World/World.hpp"

namespace phy2d{

World::World(){
    
}
World::~World(){
    
}
void World::add_object(Object* obj){
    objects.push_back(obj);
}
void World::remove_object(Object* obj){
    objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}

std::vector<Object *>& World::get_objects()
{
    return this->objects;
}
void World::update(float dt){
    
    for (auto object:objects){
        if(!object->isAwake()) continue;
        object->velocity.y+=gravity;
        object->update(dt);
    }
    std::vector<CollideInfo> collideInfos;
    for(int i=0;i<objects.size();i++){
        for(int j=i+1;j<objects.size();j++){
            auto info = Collision::collide(objects[i],objects[j]);
            if (info.obj1==nullptr||info.obj2==nullptr) continue;
            collideInfos.push_back(info);
        }
    }
    for(auto collideInfo:collideInfos){
        auto obj1=collideInfo.obj1;
        auto obj2=collideInfo.obj2;
        if(!obj1->isAwake()||!obj2->isAwake()){
            obj1->setAwake(true);
            obj2->setAwake(true);
        }
    }
    for(auto collideInfo:collideInfos){
        auto obj1=collideInfo.obj1;
        auto obj2=collideInfo.obj2;
        if(!obj1->isAwake()&&!obj2->isAwake()) continue;
        collideInfo.generate();
    }
    for (auto object:objects){
        object->updateSleepTime(dt);
    }
}
void World::render()
{
    for(auto object:objects){

    }
}
}