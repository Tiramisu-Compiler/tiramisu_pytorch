#ifndef HALIDE_PYTHON_BINDINGS_PYHALIDE_H
#define HALIDE_PYTHON_BINDINGS_PYHALIDE_H

#include <pybind11/pybind11.h>
#include "operators/relu_layer_generator_tiramisu.o.h"

//#include <torch/csrc/jit/custom_operator.h>

// Register a pass to convert the IR into one with our operator
#include <torch/csrc/jit/pass_manager.h>
// CustomFuseGraph is a helper to use simple whitelisting
#include <torch/csrc/jit/passes/graph_fuser.h>
#include <torch/csrc/autograd/variable.h>
#include <torch/csrc/autograd/function.h>
//#include <ATen/Tensor.h>
//#include <ATen/ATen.h>
#include <torch/extension.h>
#include <tiramisu/utils.h>
#include <torch/torch.h>
#include "Halide.h"
#include <cstdlib>
#include <chrono>
#include <string>
#include <time.h>
#include <iostream>

//#include "compiler.h"
namespace py = pybind11;

using namespace torch::jit;

PYBIND11_MODULE(tiramisu_compiler, m) {
  
  const auto tiramisu_compiler_symbol =
      Symbol::fromQualString("pw::CompilationGroup");

  /* Enable Function 
  Role : Define Tiramisu Compiler Configuration. 
  Mode : 
        - Mode 1 : Use Tiramisu prebuilt operators. (No installation required)
        - Mode 2 : PyTorch IR --> Tiramisu Computation Graph --> Create the computations
  Fusion : Boolean to define whether or not we fuse function when it accelerate the code.
  */        
  m.def(
      "enable",
      [](int mode, 
          bool fusion) {
        tiramisu::set_config(mode, fusion);
      },
      py::arg("mode") = 1,
      py::arg("fusion") = false
  );  

  m.def(
      "relu",
      [](torch::Tensor  mode) {
          
          //std::cout << parameters(0); 
          //std::cout << mode.sizes(); 
          int N = mode.size(0); 
          Halide::Buffer<float> input(N, N, 1, 1);
          Halide::Buffer<float> output(N, N, 1, 1);
          Halide::Buffer<float> parameters(4);
        
          parameters(0) = N;
          parameters(1) = 1;
          parameters(2) = 1;
          parameters(3) = 0;

          auto foo_a = mode.accessor<float,2>();
          for (int n = 0; n < 1; ++n)
           for (int z = 0; z < 1; ++z)
            for (int y = 0; y < 500; ++y)
                for (int x = 0; x < 500; ++x)
                    input(x, y, z, n) = foo_a[x][y];

          relu_tiramisu(input.raw_buffer(), parameters.raw_buffer(), output.raw_buffer());
          //std::cout << output; 
        
      }
  ); 

}



#endif  // HALIDE_PYTHON_BINDINGS_PYHALIDE_H


    