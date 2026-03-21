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
    for(auto object:objects){
        //vec2 newv=object->getVelocity()+vec2(0.0f,9.8f);
        //object->setVelocity(newv.x,newv.y);
        object->update(dt);
    }
    for(int i=0;i<objects.size();i++){
        for(int j=i+1;j<objects.size();j++){
            Collision::collide(objects[i],objects[j]);
        }
    }
}
void World::render()
{
    for(auto object:objects){

    }
}
}