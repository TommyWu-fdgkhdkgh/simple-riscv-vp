# for `source`
SHELL := /bin/bash

NPROC ?= 32
CPU_TYPE ?=atomic
TERM_PORT ?= 3456

#####################
####  compiler  #####
#####################
.PHONY: install/riscv64-unknown-elf/gcc
install/riscv64-unknown-elf/gcc:
	@ sudo apt-get install gcc-riscv64-unknown-elf

#################
####  gem5  #####
#################
.PHONY: gem5/apt-install
gem5/apt-install:
	sudo apt install -y build-essential git m4 scons zlib1g zlib1g-dev libprotobuf-dev protobuf-compiler libprotoc-dev libgoogle-perftools-dev python3-dev python3-six python-is-python3 libboost-all-dev pkg-config

.PHONY: gem5/clone
gem5/clone:
	@ git clone git@github.com:TommyWu-fdgkhdkgh/gem5.git
	@ cd gem5 && git checkout simple-riscv

.PHONY: gem5/venv
gem5/venv:
	@ python -m venv gem5_venv
	@ source gem5_venv/bin/activate && \
		cd gem5 && \
		pip install -r requirements.txt

.PHONY: gem5/build-opt
gem5/build-opt:
	@ source gem5_venv/bin/activate && \
		cd gem5 && \
		scons build/RISCV/gem5.opt -j$(NPROC)

.PHONY: gem5/build-debug
gem5/build-debug:
	@ source gem5_venv/bin/activate && \
		cd gem5 && \
		scons build/RISCV/gem5.debug -j$(NPROC)

.PHONY: gem5/run-opt/hello-world
gem5/run-opt/hello-world:
	@ gem5/build/RISCV/gem5.opt gem5/configs/learning_gem5/part1/simple-riscv.py

.PHONY: gem5/run-debug/hello-world
gem5/run-debug/hello-world:
	@ gdb --args gem5/build/RISCV/gem5.debug gem5/configs/learning_gem5/part1/simple-riscv.py

