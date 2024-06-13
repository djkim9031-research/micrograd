#pragma once

#include <set>
#include <memory>
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

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

            //=====================================================================================================================
            // (+) operation
            //
            // 1. Value + Value
            // 2. Value + double
            // 3. double + Value
            //=====================================================================================================================

            std::shared_ptr<Value> operator+(Value& other) {
                auto other_ptr = other.shared_from_this();
                auto output = std::make_shared<Value>(data + other.data, std::set<std::shared_ptr<Value>>{shared_from_this(), other_ptr}, "+", "("+label+"+"+other.label+")");
                output->back_prop = [this, other_ptr, output](){
                    this->grad += 1.0 * output->grad;
                    other_ptr->grad += 1.0 * output->grad;
                };
                return output;
            }

            std::shared_ptr<Value> operator+(double other) {
                std::set<std::shared_ptr<Value>> init = {};
                std::ostringstream label_stream;
                label_stream << std::fixed << std::setprecision(2) <<other;
                std::string dlabel = label_stream.str();
                auto other_ptr = std::make_shared<Value>(other, init, "", dlabel);
                auto output = std::make_shared<Value>(data + other, std::set<std::shared_ptr<Value>>{shared_from_this(), other_ptr}, "+", "("+label+"+"+other_ptr->label+")");
                output->back_prop = [this, other_ptr, output](){
                    this->grad += 1.0 * output->grad;
                    other_ptr->grad += 1.0 * output->grad;
                };
                return output;
            }

            friend inline std::shared_ptr<Value> operator+(double lhs, Value& rhs);

            //=====================================================================================================================
            // (-) operation
            //
            // 1. Value - Value
            // 2. Value - double
            // 3. double - Value
            //=====================================================================================================================

            std::shared_ptr<Value> operator-(Value& other) {
                auto negated = -1.0 * other;
                return ((*this) + (*negated));
            }

            std::shared_ptr<Value> operator-(double other) {
                auto negated = -1.0 * other;
                return ((*this) + (negated));
            }

            friend inline std::shared_ptr<Value> operator-(double lhs, Value& rhs);

            //=====================================================================================================================
            // (*) operation
            //
            // 1. Value * Value
            // 2. Value * double
            // 3. double * Value
            //=====================================================================================================================

            std::shared_ptr<Value> operator*(Value& other) {
                auto other_ptr = other.shared_from_this();
                auto output = std::make_shared<Value>(data * other_ptr->data, std::set<std::shared_ptr<Value>>{shared_from_this(), other_ptr}, "*", "("+label+"*"+other.label+")");
                output->back_prop = [this, other_ptr, output](){
                    this->grad += other_ptr->data * output->grad;
                    other_ptr->grad += this->data * output->grad;
                };
                return output;
            }

            std::shared_ptr<Value> operator*(double other) {
                std::set<std::shared_ptr<Value>> init = {};
                std::ostringstream label_stream;
                label_stream << std::fixed << std::setprecision(2) <<other;
                std::string dlabel = label_stream.str();
                auto other_ptr = std::make_shared<Value>(other, init, "", dlabel);
                auto output = std::make_shared<Value>(data * other, std::set<std::shared_ptr<Value>>{shared_from_this(), other_ptr}, "*", "("+label+"*"+other_ptr->label+")");
                output->back_prop = [this, other_ptr, output](){
                    this->grad += other_ptr->data * output->grad;
                    other_ptr->grad += this->data * output->grad;
                };
                return output;
            }

            friend inline std::shared_ptr<Value> operator*(double lhs, Value& rhs);

            //=====================================================================================================================
            // (/) operation
            //
            // 1. Value / Value
            // 2. Value / double
            // 3. double / Value
            //=====================================================================================================================

            std::shared_ptr<Value> operator/(Value& other) {
                auto div = other.pow(-1.0);
                return ((*this) * (*div));
            }

            std::shared_ptr<Value> operator/(double other) {
                auto div = std::pow(other, -1.0);
                return ((*this) * (div));
            }

            friend inline std::shared_ptr<Value> operator/(double lhs, Value& rhs);

            //=====================================================================================================================
            // (pow) operation
            //
            // 1. Value.Pow(double) => (expression)^(double value)
            // 2. double.Pow(Value) => (double value)^(expression)
            //=====================================================================================================================
            
            // y = x^a, where x is the variable currently being handled. a is the double value.
            std::shared_ptr<Value> pow(double other){
                std::set<std::shared_ptr<Value>> init = {};
                std::ostringstream label_stream;
                label_stream << std::fixed << std::setprecision(2) <<other;
                std::string dlabel = label_stream.str();
                auto other_ptr = std::make_shared<Value>(other, init, "", dlabel);
                auto output = std::make_shared<Value>(std::pow(data, other), std::set<std::shared_ptr<Value>>{shared_from_this(), other_ptr}, "^", "("+label+"^"+other_ptr->label+")");
                output->back_prop = [this, other_ptr, output](){
                    this->grad += (other_ptr->data * std::pow(this->data, other_ptr->data - 1)) * output->grad;
                };
                return output;
            }

            // y = a^x, where a is the double value. x is the variable currently being handled.
            std::shared_ptr<Value> pow_with_base(double other){
                std::set<std::shared_ptr<Value>> init = {};
                std::ostringstream label_stream;
                label_stream << std::fixed << std::setprecision(2) <<other;
                std::string dlabel = label_stream.str();
                auto other_ptr = std::make_shared<Value>(other, init, "", dlabel);
                auto output = std::make_shared<Value>(std::pow(other, data), std::set<std::shared_ptr<Value>>{other_ptr, shared_from_this()}, "^", "("+other_ptr->label+"^"+label+")");
                output->back_prop = [this, other_ptr, output](){
                    // y = a^x => y' = a^x * ln(a)
                    this->grad += (std::pow(other_ptr->data, this->data) * std::log(other_ptr->data)) * output->grad;
                };
                return output;
            }

            //=====================================================================================================================
            // (exp) operation
            //
            // 1. exp(Value)
            //=====================================================================================================================

            std::shared_ptr<Value> exp(){
                double t = std::exp(data);
                auto output = std::make_shared<Value>(t, std::set<std::shared_ptr<Value>>{shared_from_this()}, "exp", "exp_("+label+")");
                output->back_prop = [this, t, output](){
                    this->grad += t * output->grad;
                };
                return output;
            }

            //=====================================================================================================================
            // (tanh) operation
            //
            // 1. tanh(Value)
            //=====================================================================================================================

            std::shared_ptr<Value> tanh(){
                double t = (std::exp(2*data) - 1) / (std::exp(2*data) + 1);
                auto output = std::make_shared<Value>(t, std::set<std::shared_ptr<Value>>{shared_from_this()}, "tanh", "tanh_("+label+")");
                output->back_prop = [this, t, output](){
                    this->grad += (1 - t*t) * output->grad;
                };
                return output;
            }

            //=====================================================================================================================
            // Backpropagation logic
            // 
            // How to use:
            // First build the expressions in order
            // e.g. std::set<std::shared_ptr<Value>> init = {};
            //      auto x1 = std::make_shared<Value>(2.0, init, "", "x1");
            //      auto w1 = std::make_shared<Value>(-3.0, init, "", "w1");
            //      auto y = (*x1) * (*w1)
            // Then, call backward() at the last value of the expression tree.
            // e.g. y.backward()
            //=====================================================================================================================

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


    std::shared_ptr<Value> operator+(double lhs, Value& rhs){
        std::set<std::shared_ptr<Value>> init = {};
        std::ostringstream label_stream;
        label_stream << std::fixed << std::setprecision(2) <<lhs;
        std::string dlabel = label_stream.str();
        auto lhs_ptr = std::make_shared<Value>(lhs, init, "", dlabel);
        auto output = std::make_shared<Value>(lhs + rhs.data, std::set<std::shared_ptr<Value>>{lhs_ptr, rhs.shared_from_this()}, "+", "("+lhs_ptr->label+"+"+rhs.label+")");
        output->back_prop = [lhs_ptr, &rhs, output](){
            lhs_ptr->grad += 1.0 * output->grad;
            rhs.grad += 1.0 * output->grad;
        };
        return output;
    }

    std::shared_ptr<Value> operator-(double lhs, Value& rhs){
        auto negated = -1.0 * rhs;
        return (lhs + (*negated));
    }

    std::shared_ptr<Value> operator*(double lhs, Value& rhs){
        std::set<std::shared_ptr<Value>> init = {};
        std::ostringstream label_stream;
        label_stream << std::fixed << std::setprecision(2) <<lhs;
        std::string dlabel = label_stream.str();
        auto lhs_ptr = std::make_shared<Value>(lhs, init, "", dlabel);
        auto output = std::make_shared<Value>(lhs * rhs.data, std::set<std::shared_ptr<Value>>{lhs_ptr, rhs.shared_from_this()}, "*", "("+lhs_ptr->label+"*"+rhs.label+")");
        output->back_prop = [lhs_ptr, &rhs, output](){
            lhs_ptr->grad += rhs.data * output->grad;
            rhs.grad += lhs_ptr->data * output->grad;
        };
        return output;
    }

    std::shared_ptr<Value> operator/(double lhs, Value& rhs){
        auto div = rhs.pow(-1.0);
        return (lhs * (*div));
    }
}