
#include <map>
#include <string.h>
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <types.h>
#include "../../../benchmarks/DNN/layers/relu/cpu/configure.h"


namespace tiramisu_torch 
{
    bool isSupported(Node* node);
    void execute_operator(Node* node); 
 
}