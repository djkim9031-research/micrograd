#include "visualizer.h"

#include <string>

namespace micrograd{
    void trace(const std::shared_ptr<Value>& last_node, 
               std::set<std::shared_ptr<Value>>& nodes, 
               std::set<std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>>>& edges){
        
        if(nodes.find(last_node) == nodes.end()){
            nodes.insert(last_node);
            for(const auto& child : last_node->prevs){
                edges.insert({child, last_node});
                trace(child, nodes, edges);
            }
        }
    }

    void draw_plot(const std::shared_ptr<Value>& last_node){
        std::set<std::shared_ptr<Value>> nodes;
        std::set<std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>>> edges;
        trace(last_node, nodes, edges);

        GVC_t* gvc = gvContext();
        Agraph_t* g = agopen(const_cast<char*>("g"), Agdirected, nullptr);
        agattr(g, AGNODE, const_cast<char*>("shape"), const_cast<char*>("record"));

        for(const auto& n : nodes){
            std::string uid = std::to_string(reinterpret_cast<std::uintptr_t>(n.get()));
            Agnode_t* node = agnode(g, const_cast<char*>(uid.c_str()), 1);
            
            std::ostringstream label_stream;
            label_stream << "{data " << std::fixed << std::setprecision(2) << n->data << " | grad " << std::fixed << std::setprecision(2) << n->grad << "}";
            std::string label = label_stream.str();
            agset(node, const_cast<char*>("label"), const_cast<char*>(label.c_str()));

            if(!n->op.empty()){
                Agnode_t* opNode = agnode(g, const_cast<char*>((uid + n->op).c_str()), 1);
                agset(opNode, const_cast<char*>("label"), const_cast<char*>(n->op.c_str()));
                agedge(g, opNode, node, nullptr, 1);
            }
        }

        for(const auto& e : edges){
            std::string n1_uid = std::to_string(reinterpret_cast<std::uintptr_t>(e.first.get()));
            std::string n2_uid = std::to_string(reinterpret_cast<std::uintptr_t>(e.second.get())) + e.second->op;
            agedge(g, agnode(g, const_cast<char*>(n1_uid.c_str()), 0), agnode(g, const_cast<char*>(n2_uid.c_str()), 0), nullptr, 1);
        }

        gvLayout(gvc, g, "dot");
        gvRenderFilename(gvc, g, "svg", "graph.svg");
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);
    }
}