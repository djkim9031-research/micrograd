#pragma once

#include <set>
#include <memory>

namespace micrograd{

    class Value{
        public:
            double data;
            std::set<std::shared_ptr<Value>> _prev;
            std::string _op;

            Value(double data, std::set<std::shared_ptr<Value>> children = {}, std::string op = "")
                : data(data), _prev(children), _op(op) {}
            
            std::string repr() const {
                return "Value(data = "+std::to_string(data)+")";
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