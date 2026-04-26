#ifndef WORLD_HPP
#define WORLD_HPP

#include "../Object/Object.hpp"
#include "../Collision/Collision.hpp"

#include <algorithm>


namespace phy2d{
    class World {
    public:
        World();
        ~World();

        void add_object(Object* obj);
        void remove_object(Object* obj);
        std::vector<Object*>& get_objects();
        void update(float dt);
        virtual AABB getAABB(){};
        void render();

    private:
        std::vector<Object*> objects;
        float gravity= 10.0f;
    };
}

#endif