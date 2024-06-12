#pragma once

#include <set>
#include <memory>
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>

#include <iostream>

namespace micrograd{

    class Value : public std::enable_shared_from_this<Value>{
        public:
            static int nVals;
            double data;
            double grad;
            std::set<std::shared_ptr<Value>> prevs;
            std::string op;
            std::string label;
            std::function<void()> back_prop;

            Value(double data, std::set<std::shared_ptr<Value>> children = {}, std::string op = "", std::string label="")
                : data(data), prevs(children), op(op), grad(0.0), label(label) {
                back_prop = [](){};
            }
            
            std::string repr() const {
                return "Value(data = "+std::to_string(data)+" | grad = " + std::to_string(grad) + " label: "+ label+ ")";
            }

            std::shared_ptr<Value> operator+(Value& other) {
                auto other_ptr = other.shared_from_this();
                auto output = std::make_shared<Value>(data + other.data, std::set<std::shared_ptr<Value>>{shared_from_this(), other_ptr}, "+", label+"+"+other.label);
                output->back_prop = [this, other_ptr, output](){
                    this->grad = 1.0 * output->grad;
                    other_ptr->grad = 1.0 * output->grad;
                };
                return output;
            }

            std::shared_ptr<Value> operator*(Value& other) {
                auto other_ptr = other.shared_from_this();
                auto output = std::make_shared<Value>(data * other_ptr->data, std::set<std::shared_ptr<Value>>{shared_from_this(), other_ptr}, "*", label+"*"+other.label);
                output->back_prop = [this, other_ptr, output](){
                    this->grad = other_ptr->data * output->grad;
                    other_ptr->grad = this->data * output->grad;
                };
                return output;
            }

            std::shared_ptr<Value> tanh(){
                double t = (std::exp(2*data) - 1) / (std::exp(2*data) + 1);
                auto output = std::make_shared<Value>(t, std::set<std::shared_ptr<Value>>{shared_from_this()}, "tanh", "tanh_("+label+")");
                output->back_prop = [this, t, output](){
                    this->grad = (1 - t*t) * output->grad;
                };
                return output;
            }

            void backward(){
                std::vector<std::shared_ptr<Value>> topo;
                std::set<std::shared_ptr<Value>> visited;

                build_topo(shared_from_this(), topo, visited);

                this->grad = 1.0;
                for(auto it = topo.rbegin(); it!=topo.rend(); ++it){
                    (*it)->back_prop();
                }
            }


        private:
            void build_topo(const std::shared_ptr<Value>& node, 
                            std::vector<std::shared_ptr<Value>>& topo, 
                            std::set<std::shared_ptr<Value>> &visited){
                if(visited.find(node) == visited.end()){
                    visited.insert(node);
                    for(const auto& child : node->prevs){
                        build_topo(child, topo, visited);
                    }
                    topo.push_back(node);
                }
            }
    };
}