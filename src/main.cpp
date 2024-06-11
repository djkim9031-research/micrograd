#include <iostream>

#include "block.h"
#include "visualizer.h"

using namespace micrograd;

int main() {
    auto a = std::make_shared<Value>(2.0);
    auto b = std::make_shared<Value>(-3.0);
    auto c = std::make_shared<Value>(10.0);

    auto d = (*(*a * *b) + *c);

    draw_plot(d);

    return 0;
}
