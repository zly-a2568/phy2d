#include "../../include/World/World.hpp"

#include "../../include/Island/Island.h"



namespace phy2d{

World::World(){
    
}
World::~World(){
    
}
void World::add_object(Object* obj){
    objects.push_back(obj);
    //BroadPhase::getInstance()->addBody(obj);

}
void World::remove_object(Object* obj){
    //BroadPhase::getInstance()->removeBody(obj);
    objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}

std::vector<Object *>& World::get_objects()
{
    return this->objects;
}
void World::update(float dt){

    for (auto object:objects){
        //object->velocity.y+=gravity;
        if(!object->isAwake()) continue;
        object->update(dt);
        //BroadPhase::getInstance()->updateBody(object);
    }
    for (int i=0;i<8;i++) {
        std::vector<CollideInfo> collideInfos;
        for (int a=0;a<objects.size();a++) {
            for (int b=a+1;b<objects.size();b++) {
                auto bodyA=objects[a];
                auto bodyB = objects[b];
                if (!bodyA->bounding_box.intersects(bodyB->bounding_box))continue;
                CollideInfo info=Collision::collide(bodyA,bodyB);
                if (info.obj1==nullptr||info.obj2==nullptr)continue;
                collideInfos.push_back(info);

            }
        }

        for (auto& contact:collideInfos) {
            contact.generate_position();
            contact.generate();
        }
    }
}
void World::render()
{
    for(auto object:objects){

    }
}
}
