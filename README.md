# Micrograd
Micrograd c++ implementation

C++ implementation of gradient graph, similar to TensorFlow or PyTorch.
Initial python implementation is shown by Andrej Karpathy. 

Each scalar value class contains a value and a gradient field.
An arithmatic operation connects operands in the gradient graph and at the final stage (e.g., loss value in nn), backpropagation propagates the gradients from the loss sequentially using the gradient chain rule.
Using this building block, a simple multi-layer perceptron (MLP) is constructed and convergence of predicted outputs of MLP to the ground truths is demonstrated.

This code implements the following:
1. Value class that builds/connects each operand to a gradient graph.
2. Backpropagation of the gradient graph.
3. A neuron (weights and biases as operands)
4. An neural network layer (multiple neurons)
5. Multi-layer perceptron
6. A simple regression case with inputs and ground truth values, to demonstrate the convergence of NN model to the GT.
7. Visualizer for value gradient graph.
8. Visualizer for MLP layer connection graph.

   

![nn_graph](https://github.com/djkim9031-research/micrograd/assets/172340336/47e00ada-2fa7-46d9-870e-2cfa1fb90698)

![image](https://github.com/djkim9031-research/micrograd/assets/172340336/9b2ce6a2-88f6-4002-84a1-24e0d94f9139)



![value_graph](https://github.com/djkim9031-research/micrograd/assets/172340336/e4f24341-4af9-4ecc-80b6-b314524846b3)
