# pytorch_tiramisu 

pytorch_tiramisu is a python package that adds [Tiramisu Compiler](http://tiramisu-compiler.org/) as a compiler backend to [PyTorch Deep Learning Framework](https://pytorch.org/). 

pytorch_tiramisu offers two mode of utilization: 
<img src="https://zupimages.net/up/20/22/fx6v.png" />

1. Mode 1: The first mode is dedicated to non expert users that want to benefit from directly laveraging the compiler stack. The user will be using pre-compiled operators. (This mode is only available for CPUs). **This mode does not require the installation of Tiramisu.** 

2. Mode 2: If you want to register more operators or apply some optimizations that are not yet supported by the package, you can install [tiramisu](https://github.com/Tiramisu-Compiler/tiramisu#building-tiramisu-from-sources) and fully pass all the compiler stack, as the figure above illustrates. 

## Build & Install 
1. Install the latest Nightly build of PyTorch. You can choose to install it from source for more efficient acceleration [follow these instructions](https://github.com/pytorch/pytorch#from-source).
2. Install pytorch_tiramisu: We recommend to install the package from source, since it  is still a research project in its infancy. 

### Using pypi 
```
pip install pytorch_tiramisu
```

### Install from Source 

* Building pytorch_tiramisu from source requires the following packages : 
  * [cmake](https://cmake.org/install/) 
  * automake 
  * libtool 

* You can check the [Dockerfile](https://github.com/IHIaadj/tiramisu_pytorch/blob/master/Dockerfile) which shows the environment we used for testing and building pytorch_tiramisu. 
* Clone this repository:
```
git clone --recursive https://github.com/IHIaadj/tiramisu_pytorch.git 
cd tiramisu_pytorch
```

* Install the package: This file sets the paths to both PyTorch and [pybind11](https://github.com/pybind/pybind11).   
``` 
# If mode 2 uncomment the fourth line from build.sh
./build.sh
``` 

## Tests & Tutorials 

You can test the installation by running the following code: 
```
import torch 
import pytorch_tiramisu as pt 
pt.enable(jit=True) 
```

Take a look at one of our Jupyter notebooks to quickly try different features and deep learning models:
* [Getting Started with pytorch_tiramisu](https://github.com/Tiramisu-Compiler/tiramisu_pytorch/blob/master/notebooks/getting_started.ipynb)
* [ResNet in Tiramisu](https://github.com/Tiramisu-Compiler/tiramisu_pytorch/blob/master/notebooks/resnet.ipynb)
* [XLNet](https://github.com/Tiramisu-Compiler/tiramisu_pytorch/blob/master/notebooks/xlnet.ipynb)

## Usage 
* pytorch_tiramisu can hook into PyTorch JIT and compile the model operators that it supports. (if jit parameter is set to True) 
```
import torch 
import torch.nn.functional as F
import pytorch_tiramisu as pt 
pt.enable(jit=True) 

# The following function will be compiled with Tiramisu
@torch.jit.script
def relu_(a):
    return F.relu(a) 
```
* Otherwise, pytorch_tiramisu.compile(model) can be used to perform the compilation of the deep learning model prior to running the final graph execution. 
```
import torch 
import torch.nn.functional as F
import pytorch_tiramisu as pt 
pt.enable(jit=False) 

class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.fc = nn.Linear(256, 10)

    def forward(self, x):
        x = self.fc3(x)
        return x
        
model = Net() 
a = torch.randn(1, 256)
generated = pt.compile(model(a)) # Execute an optimization pass and generate the operators.
pt.execute(generated) 
```



