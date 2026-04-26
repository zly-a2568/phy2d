#pragma once

#include "../Object/Object.hpp"
#include <vector>
#include <unordered_map>
#include "../Collision/CollideInfo.hpp"

namespace  phy2d {
    class Island {
    public:
        std::vector<Object*> objs;
        std::vector<CollideInfo*> contacts;
        bool awake=true;
        Island(){}
        ~Island(){}
        static void DFSGenerate(std::vector<Island>& islands,std::vector<Object*>& objects,std::vector<CollideInfo>& contacts);
    };
}
