#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;


//give this a better name
struct NodeID {
    std::string idName;
    int idIndex;

public:
    NodeID(std::string name, int index){
        idName = name;
        idIndex = index;
    }

    // friend std::ostream& operator << (std::ostream& os, NodeID const& p) {
    //     return os << ((p.idName != "") ? p.idName : std::to_string(p.idIndex));
    // }
    std::string to_string() {
        return (idName != "") ? idName : std::format("num{}",idIndex);
    }
    
    bool operator< (const NodeID& other) const {        // why tf is this here 
        if (idName != other.idName){
            return idName < other.idName;
        }

        return idIndex < other.idIndex;
    }
};

struct NodePath {
    std::list<NodeID> path;
    std::string topLayerID;

    static NodePath create(CCNode* curr){
        NodePath result = NodePath();
        // std::vector<NodeID> path;
        //struct NodeID test = {curr->getID(),getNumID(curr)};
        result.path.push_front({curr->getID(),getNumID(curr)});
        while(curr && !typeinfo_cast<CCScene*>(curr->getParent())){ // while parent exists and parent is not CCScene*
            curr = curr->getParent();
            result.path.push_front({curr->getID(),getNumID(curr)});

        }
        // NodePath result{path,curr->getID}};
        result.topLayerID = curr->getID();
        return result;
    }


    static int getNumID(CCNode* node) {
        auto parent = node->getParent();
        auto siblings = parent->getChildren();
        return siblings->indexOfObject(node);
    }

    std::string to_string() {
        std::ostringstream ss;
        for (auto id : path) {
            ss << id.to_string() << "::";
        }
        std::string output = ss.str();
        if (!output.empty()) {
            output.resize(output.size() - 2); // Remove the last "::"
        }
        return output;
    }
    
};

