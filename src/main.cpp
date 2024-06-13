#include <iostream>

#include "block.h"
#include "visualizer.h"

using namespace micrograd;

int main() {
    std::set<std::shared_ptr<Value>> init = {};
   
    auto x1 = std::make_shared<Value>(2.0, init, "", "x1");
    auto x2 = std::make_shared<Value>(0.0, init, "", "x2");
    auto w1 = std::make_shared<Value>(-3.0, init, "", "w1");
    auto w2 = std::make_shared<Value>(1.0, init, "", "w2");

    auto x1w1 = (*x1)* (*w1);
    auto x2w2 = (*x2)* (*w2);
    auto temp = (*x1w1) + (*x2w2);
    auto bias = std::make_shared<Value>(6.8814, init, "", "b");

    auto n = (*temp) + (*bias);
    //auto o = n->tanh();
    auto e = (2* (*n));
    e = e->exp();
    auto numer = (*e-1);
    auto denom = (*e+1);
    auto o = (*numer)/(*denom);

    o->backward();
 
    /*

    auto a = std::make_shared<Value>(-2.0, init, "", "a");
    auto b = std::make_shared<Value>(3.0, init, "", "b");
    auto d = (*a)/(*b);
    auto e = d->pow_with_base(10);
    e->backward();
    */

    draw_plot(o);

    return 0;
}
