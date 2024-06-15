#pragma once

#include "block.h"
#include <vector>
#include <random>
#include <cassert>

namespace micrograd{

    class Neuron{
        public:
            std::vector<std::shared_ptr<Value>> ws;
            std::shared_ptr<Value> b;
            std::vector<std::shared_ptr<Value>> params;

            Neuron(int num_in){
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(-1.0, 1.0);
                std::set<std::shared_ptr<Value>> init = {};

                for(size_t i=0; i<num_in; ++i){
                    auto val = std::make_shared<Value>(dis(gen), init, "", "w"+std::to_string(i));
                    ws.push_back(val);
                }
                b = std::make_shared<Value>(dis(gen), init, "", "b0");
            }

            std::shared_ptr<Value> operator()(const std::vector<std::shared_ptr<Value>> &x){
                assert(("input size must match the number of neurons", ws.size() == x.size()));
                auto output = b;
                for(size_t i=0; i<ws.size(); ++i){
                    auto mult = (*ws[i]) * (*x[i]);
                    output = (*output) + (*mult);
                }   

                return output->tanh();
            }

            std::vector<std::shared_ptr<Value>> parameters() {
                params = ws;
                params.push_back(b);
                return params;
            }
    };

    class Layer{
        public:
            int num_in;
            int num_out;
            std::string layer_name;
            std::vector<Neuron> neurons;
            std::vector<std::shared_ptr<Value>> params;

            Layer(int num_in, int num_out, std::string layer_name = "connected")
                : num_in(num_in), num_out(num_out), layer_name(layer_name){
                for(size_t i=0; i<num_out; ++i){
                    Neuron n(num_in);
                    neurons.push_back(n);
                }
            }

            std::vector<std::shared_ptr<Value>> operator()(const std::vector<std::shared_ptr<Value>> &x){
                assert(("input size must match the num_in", num_in == x.size()));
                std::vector<std::shared_ptr<Value>> outputs;
                for(size_t i=0; i<num_out; ++i){
                    outputs.push_back(neurons[i](x));
                }
                return outputs;
            }

            std::vector<std::shared_ptr<Value>> parameters(){
                for(size_t i=0; i<neurons.size(); ++i){
                    auto curr_params = neurons[i].parameters();
                    params.insert(params.end(), curr_params.begin(), curr_params.end());
                }
                return params;
            }
    };

    class MLP{
        public:
            std::vector<Layer> layers;
            std::vector<std::shared_ptr<Value>> params;

            MLP(const std::vector<double> &num_neurons_per_layer){
                for(size_t i=0; i<num_neurons_per_layer.size()-1; ++i){
                    Layer layer(num_neurons_per_layer[i], num_neurons_per_layer[i+1], "connected"+std::to_string(i+1));
                    layers.push_back(layer);
                }
            }

            std::vector<std::shared_ptr<Value>> operator()(const std::vector<std::shared_ptr<Value>> &x){
                assert(("input size must match the num_in of the first layer", layers[0].num_in == x.size()));
                std::vector<std::shared_ptr<Value>> outputs;
                outputs = x;
                for(size_t i=0; i<layers.size(); ++i){
                    outputs = layers[i](outputs); 
                }
                return outputs;
            }

            std::vector<std::shared_ptr<Value>> parameters(){
                for(size_t i=0; i<layers.size(); ++i){
                    auto curr_params = layers[i].parameters();
                    params.insert(params.end(), curr_params.begin(), curr_params.end());
                }
                return params;
            }
    };
}