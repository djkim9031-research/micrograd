#pragma once

#include <set>
#include <sstream>
#include <iomanip>
#include <graphviz/gvc.h>

#include "block.h"

namespace micrograd{

    // Helper function to populate nodes and edges
    void trace(const std::shared_ptr<Value>& last_node, 
               std::set<std::shared_ptr<Value>>& nodes, 
               std::set<std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>>>& edges);
    
    // Function to draw the calculation graph.
    void draw_plot(const std::shared_ptr<Value>& last_node);

}