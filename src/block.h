#pragma once

#include <set>
#include <memory>

namespace micrograd{

    class Value{
        public:
            double data;
            double grad;
            std::set<std::shared_ptr<Value>> prevs;
            std::string op;

            Value(double data, std::set<std::shared_ptr<Value>> children = {}, std::string op = "", double grad = 0.0)
                : data(data), prevs(children), op(op), grad(grad) {}
            
            std::string repr() const {
                return "Value(data = "+std::to_string(data)+" | grad = " + std::to_string(grad) + ")";
            }

            std::shared_ptr<Value> operator+(const Value& other) const{
                auto output = std::make_shared<Value>(data + other.data, std::set<std::shared_ptr<Value>>{std::make_shared<Value>(*this), std::make_shared<Value>(other)}, "+");
                return output;
            }

            std::shared_ptr<Value> operator*(const Value& other) const {
                auto output = std::make_shared<Value>(data * other.data, std::set<std::shared_ptr<Value>>{std::make_shared<Value>(*this), std::make_shared<Value>(other)}, "*");
                return output;
            }
            
    };
}