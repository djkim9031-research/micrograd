#include <iostream>
#include <set>
#include <memory>
#include <graphviz/gvc.h>

#include "block.h"

using namespace micrograd;


int main() {
    auto a = std::make_shared<Value>(2.0);
    auto b = std::make_shared<Value>(-3.0);
    auto c = std::make_shared<Value>(10.0);

    auto d = (*(*a * *b) + *c);

    std::cout<<"Value of d: "<<d->data<<std::endl;
    
    return 0;
}
