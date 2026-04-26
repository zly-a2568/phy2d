#include "../../include/Island/Island.h"
#include <stack>
namespace  phy2d {


    void Island::DFSGenerate(std::vector<Island>& islands,std::vector<Object*>& objects,std::vector<CollideInfo>& contacts) {
        std::unordered_map<Object*,bool> visited;
        std::stack<Object*> stack_buf;
        for (auto obj:objects) {
            while (!stack_buf.empty())stack_buf.pop();
            if (visited[obj]==true)continue;
            Island island;
            island.objs.push_back(obj);
            stack_buf.push(obj);
            visited[obj]=true;
            while (!stack_buf.empty()) {
                auto obj=stack_buf.top();
                stack_buf.pop();
                for (auto& contact:contacts) {
                    if (contact.obj1==obj||contact.obj2==obj) {
                        auto neighbor=contact.obj1==obj?contact.obj2:contact.obj1;
                        if (visited[neighbor]==true)continue;
                        island.contacts.push_back(&contact);
                        visited[neighbor]=true;
                        stack_buf.push(neighbor);
                        island.objs.push_back(neighbor);
                    }
                }
            }
            islands.push_back(island);
        }
    }
}
