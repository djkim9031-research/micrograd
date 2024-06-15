#include <iostream>

#include "block.h"
#include "nn.h"
#include "visualizer.h"

using namespace micrograd;

int main() {
    std::set<std::shared_ptr<Value>> init = {};
    std::vector<std::shared_ptr<Value>> input1 =
        {std::make_shared<Value>(2.0, init, "", "x1_0"),
         std::make_shared<Value>(-3.0, init, "", "x1_1"),
         std::make_shared<Value>(-1.0, init, "", "x1_2")};

    std::vector<std::shared_ptr<Value>> input2 =
        {std::make_shared<Value>(3.0, init, "", "x2_0"),
         std::make_shared<Value>(-1.0, init, "", "x2_1"),
         std::make_shared<Value>(0.5, init, "", "x2_2")};

    std::vector<std::shared_ptr<Value>> input3 =
        {std::make_shared<Value>(0.5, init, "", "x3_0"),
         std::make_shared<Value>(1.0, init, "", "x3_1"),
         std::make_shared<Value>(1.0, init, "", "x3_2")};

    std::vector<std::shared_ptr<Value>> input4 =
        {std::make_shared<Value>(1.0, init, "", "x4_0"),
         std::make_shared<Value>(1.0, init, "", "x4_1"),
         std::make_shared<Value>(-1.0, init, "", "x4_2")};

    std::vector<std::shared_ptr<Value>> y_gt =
        {std::make_shared<Value>(1.0, init, "", "y_gt0"),
         std::make_shared<Value>(-1.0, init, "", "y_gt1"),
         std::make_shared<Value>(-1.0, init, "", "y_gt2"),
         std::make_shared<Value>(1.0, init, "", "y_gt3")};

    std::vector<std::vector<std::shared_ptr<Value>>> inputs = {input1, input2, input3, input4};
    MLP mlp({3, 4, 1});
    auto params = mlp.parameters();
    int itr = 1;
    double threshold = 1e-6;
    while(true){
        auto loss = std::make_shared<Value>(0.0, init, "", "");
        for(int i=0; i<4; ++i){
            // single output regression
            auto y_pred = mlp(inputs[i])[0];
            auto diff = (*y_pred) - (*y_gt[i]);
            diff = (*diff) * (*diff);
            loss = (*loss) + (*diff);
        }
        std::cout<<"[Iteration "<<itr<<"]: Loss = "<<loss->data<<std::endl;

        if(loss->data < threshold){
            loss->label = "loss";
            micrograd::draw_value_graph(loss);
            break;
        }

        loss->backward();
        
        for(int i=0; i<params.size(); ++i){
            params[i]->data += -0.01*params[i]->grad;
        }

        itr += 1;
    }
    
    for(int i=0; i<4; ++i){
        std::cout<<"________________________________________"<<std::endl;
        std::cout<<"Ground truth: "<<y_gt[i]->data<<std::endl;
        std::cout<<"Predicted val: "<<mlp(inputs[i])[0]->data<<std::endl;
    }
    std::cout<<"________________________________________"<<std::endl;
    
    micrograd::draw_nn_graph(mlp);

    return 0;
}
