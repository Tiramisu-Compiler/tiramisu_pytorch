FROM nvidia/cuda:9.0-cudnn7-devel-ubuntu18.04

RUN rm /bin/sh && ln -sf /bin/bash /bin/sh


ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update

RUN apt-get install -y --no-install-recommends make git ssh realpath wget unzip cmake vim libncurses5-dev
RUN apt-get install -y --no-install-recommends libz-dev libgmp3-dev
RUN apt-get install -y --no-install-recommends automake libtool valgrind subversion
RUN apt-get install -y --no-install-recommends ca-certificates software-properties-common

########### Conda Environment #########################
WORKDIR /conda-install
RUN echo 'export PATH=/opt/conda/bin:$PATH' > /etc/profile.d/conda.sh &&\
    wget --quiet https://repo.continuum.io/archive/Anaconda3-5.0.1-Linux-x86_64.sh -O anaconda.sh && \
    chmod +x anaconda.sh && \
    ./anaconda.sh -b -p /opt/conda/anaconda && \
    rm anaconda.sh

RUN . /opt/conda/anaconda/bin/activate && \
    conda create -y --name tiramisu_pytorch python=3.7

RUN . /opt/conda/anaconda/bin/activate && \
    source activate tiramisu_pytorch && \
    conda update -y -n root conda && \
    conda install -y conda-build && \
    conda install -y pyyaml mkl-include pytest && \
    conda install -y -c pytorch pytorch torchvision cuda90 && \
    conda config --add channels anaconda && \
    conda config --add channels pytorch && \
    conda config --add channels conda-forge


############## Set General ENV ####################"
ENV CC /usr/bin/gcc
ENV CXX /usr/bin/g++
ENV LD_LIBRARY_PATH /usr/local/cuda/lib64:/usr/local/cuda/targets/x86_64-linux/lib/stubs/:/usr/lib/x86_64-linux-gnu/:$LD_LIBRARY_PATH
ENV PATH /usr/local/bin:/usr/local/cuda/bin:$PATH

