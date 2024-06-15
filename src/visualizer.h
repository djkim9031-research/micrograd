#pragma once

#include <set>
#include <sstream>
#include <iomanip>
#include <graphviz/gvc.h>

#include "block.h"
#include "nn.h"

namespace micrograd{

    // Helper function to populate nodes and edges
    void trace(const std::shared_ptr<Value>& last_node, 
               std::set<std::shared_ptr<Value>>& nodes, 
               std::set<std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>>>& edges);
    
    // Function to draw the calculation graph for "Value"
    void draw_value_graph(const std::shared_ptr<Value>& last_node);

    // Function to draw the NN layer graph (not as granular as the value graph).
    // This displays the layer connections and high-level information about each layer
    // in multi-layer-perceptron (MLP).
    void draw_nn_graph(class MLP& mlp);

}