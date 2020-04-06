UNAME = $(shell uname)
THIS_MAKEFILE = $(realpath $(filter %Makefile, $(MAKEFILE_LIST)))
ROOT_DIR = $(strip $(shell dirname $(THIS_MAKEFILE)))
$(info $$var is [${ROOT_DIR}])
# These are set by Halide's Makefile when built via that path.
HALIDE_PATH ?= $(ROOT_DIR)/../../3rdParty/Halide
HALIDE_DISTRIB_PATH ?= $(HALIDE_PATH)/lib
BIN ?= $(ROOT_DIR)/bin
PYTHON ?= python3
TEST_TMP ?= $(BIN)/tmp
PYBIND11_PATH ?= $(ROOT_DIR)/pybind11

ifeq ($(OS), Windows_NT)
    # assume we are building for the MinGW environment
    FPIC=
    SHARED_EXT=dll
else
    FPIC=-fPIC
ifeq ($(UNAME), Darwin)
    SHARED_EXT=dylib
	# Note that Python on OSX won't load .dylib; it requires .so
    SUFFIX=.a
else 
    SHARED_EXT=a
endif
endif

ifeq ($(UNAME), Linux)
USE_EXPORT_DYNAMIC=-rdynamic
else
ifeq ($(UNAME), Darwin)
USE_EXPORT_DYNAMIC=-undefined dynamic_lookup
else
USE_EXPORT_DYNAMIC=
endif
endif

LIBHALIDE ?= $(HALIDE_DISTRIB_PATH)/libHalide.$(SHARED_EXT)

SUFFIX = $(shell $(PYTHON)-config --extension-suffix)

# Discover PyBind path from `python3 -m pybind11 --includes`
# if it is pip/conda installed, which is a common case.
# Cf. https://github.com/pybind/pybind11/blob/master/docs/compiling.rst#building-manually
PYBIND11_CFLAGS = $(shell $(PYTHON) -m pybind11 --includes)
ifeq ($(PYBIND11_CFLAGS),)
    ifndef PYBIND11_PATH
        $(error PYBIND11_PATH is undefined)
    endif
    PYBIND11_PATH ?= /path/to/pybind11
    PYBIND11_CFLAGS = -I $(PYBIND11_PATH)/include
endif

OPTIMIZE ?= -O3

# defining DEBUG + undefining NDEBUG gives extra debug info in PyBind11
# OPTIMIZE ?= -g -DDEBUG=1 -UNDEBUG

# Compiling with -fvisibility=hidden saves ~80k on optimized x64 builds
CCFLAGS=$(shell $(PYTHON)-config --cflags) $(PYBIND11_CFLAGS) -I $(HALIDE_DISTRIB_PATH)/../include -I $(ROOT_DIR) -I ${PYTORCH_DIR}/include -I ${PYTORCH_DIR}/include/torch/csrc/api/include  -I ../../include  -L${PYTORCH_DIR}/lib/  -std=c++11 $(FPIC)  -fvisibility=hidden -fvisibility-inlines-hidden $(OPTIMIZE) $(CXXFLAGS)
# Filter out a pointless warning present in some Python installs
CCFLAGS := $(filter-out -Wstrict-prototypes,$(CCFLAGS))


# DON'T link libpython* - leave those symbols to lazily resolve at load time
# Cf. https://github.com/pybind/pybind11/blob/master/docs/compiling.rst#building-manually
LDFLAGS=-lz $(USE_EXPORT_DYNAMIC)

PY_SRCS=$(shell ls $(ROOT_DIR)/src/*.cpp)
PY_OBJS=$(PY_SRCS:$(ROOT_DIR)/src/%.cpp=$(BIN)/src/%.o)

MODULE=$(BIN)/tiramisu_compiler$(SUFFIX)

$(MODULE): $(PY_OBJS) $(LIBHALIDE) 
	@mkdir -p $(@D)
	$(CXX) $^ $(LDFLAGS) -shared -o $@ $(ROOT_DIR)/src/operators/*.o

# We don't want any of this auto-deleted
.SECONDARY:

$(BIN)/src/%.o: $(ROOT_DIR)/src/%.cpp 
	@mkdir -p $(@D)
	$(CXX) $(CCFLAGS) -c $< -o $@ 

# Produce a Python extension for the generator by compiling PyStub.cpp
# (with HALIDE_PYSTUB_GENERATOR_NAME defined to the Generator's build name),
# and linking with the generator's .o file, PyStubImpl.o, plus the same libHalide
# being used by halide.so.
#
# You can optionally also define HALIDE_PYSTUB_MODULE_NAME if you want the Python
# module name to be something other than the Generator build name.


# Which target features to use for which test targets.
target_features_addconstant=-legacy_buffer_wrappers-no_runtime
target_features_bit=-no_runtime
target_features_user_context=-user_context-legacy_buffer_wrappers-no_runtime

# TODO(srj): the python bindings need to be put into the distrib folders;
# this is a hopefully-temporary workaround (https://github.com/halide/Halide/issues/4368)
.PHONY: build_python_bindings
build_python_bindings: $(MODULE)
