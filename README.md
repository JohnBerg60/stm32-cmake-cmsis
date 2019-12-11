# How to build


## Compiling from scratch
Build libopncm3 (only once):  
```sh
$ git submodule update --init --recursive
$ cd libopencm3
$ make
$ cd ..
```

Prepare the project:  
```sh
$ cd build/
$ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.cmake -G Ninja ..
```

