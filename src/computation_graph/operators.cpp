
#include <operators.h>
#include <tiramisu/utils.h>
#include "tiramisu/relu_layer_generator_tiramisu.o.h"
#include "tiramisu/generated_conv_layer.o.h"
#include "tiramisu/maxpool_layer_generator_tiramisu.o.h"
#include "tiramisu/bn_layer_generator_tiramisu.o.h"

namespace tiramisu_torch 
{
    bool isSupported(Node* node) {
        auto map = getTiramisuOperatorMap();
        return map.find(node->kind()) != map.end();
    }

    void execute_operator(Node* node, auto parameters){
        auto name = node->kind(); 
        if (isSupported(node)){
            switch(name){
                "relu" : relu_tiramisu(parameters); 
                         break; 

                "maxpool" : maxpool_tiramisu(parameters); 
                            break; 

                "conv2d" : conv_tiramisu(parameters);
                           break;

                "batch_Normalisation" : bn_tiramisu(parameters);
                                        break; 

                default : break; 

			

            }
        }else {
            cout << "ERROR : Operator not supported. "; 
        }
    }
    
}