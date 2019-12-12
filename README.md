# Setting up VSCode for STM32 projects
Goal is to build STM32 projects, using VSCode, CMake, Openocd and cmsis

## Workflow
Setup a project by cloning this skeleton project. This sets up a fully configured VSCode project.  
For quick and dirty debugging semihosting can be used.

## Getting Started

### Prerequisites
Windows 10  
Visual Studio Code  
Git for Windows  

### Preparing the toolchain
There are several tools needed for building projects. Most tools do not need to be installed with an installer, but can simply be unzipped.  

When done, the toolchain tree should look like this:  
```
D:\Dev-Tools
| - gcc-arm-8
| - MinGW-W64
| - openocd-0.10.0
```

Ninja (1 exe) somewhere in the path.  
STM32CubeMx can come in handy to get the clock config, linkscript and assembler startup code.  


### gcc-arm-8
Download the zip from [developer.arm.com](https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2019q3/RC1.1/gcc-arm-none-eabi-8-2019-q3-update-win32.exe?revision=b3eb9c4d-f49f-4694-8928-2084c9f090ac?product=GNU%20Arm%20Embedded%20Toolchain,32-bit,,Windows,8-2019-q3-update)  
Unpack under the root of the toolchain, and rename the folder to gcc-arm-8.  
Edit your path (windows r --> env) and add the bin dir of the toolchain.  

### MingGw-X64
Download the zip from [sourceforge](https://sourceforge.net/projects/mingw-w64/)  
Unpack, and move the folder under the dev tools.
Edit your path (windows r --> env) and add the bin dir of the MingGw-X64.  
MingGW-X64 is used for make.  For convenience, just copy mingw32-make.exe to make.exe (220kB).    

### openocd-0.10.0
Download prebuilt windows binaries from [here](http://www.freddiechopin.info/en/download/category/4-openocd)  
Unpack, and move the folder under the dev tools.  

### Preparing Visual Studio Code
VSCode needs three extensions:  
- C/C++ for Visual Studio Code  
- Cortex Debug  
- CMake

For intellisense to work, ```c_cpp_properties.json``` in the .vscode folder needs to be configured with the right settings. Use the same settings for family, proc and board as in the makefile.  
```settings.json``` needs to be configured with the path to stutil.

## Starting a new project from scratch

### Preparation

```sh
$ mkdir my_awesome_project
$ chdir my_awesome_project
$ git init
$ mkdir cmake
$ mkdir doc
$ mkdir build
$ mkdir src
$ mkdir .vscode
$ code .
```
Open a new bash terminal in VSCode.  
Create a README.md 

### Copy standard files
- in .vscode: ..
- from CubeMx generated files: the ld (link script and the startup_xx.s code  
- tbd: get the svd file
- copy a main.c in src, from a skelleton project
- copy a CMakeLists.txt, from a skelleton project
- copy the arm-none-eabi.cmake and the cmsis.cmake from a skelleton project
- copy a .gitignore in the build folder

### documentation
- make a screenshot from the clock config (CubeMx), save in doc
- get the datasheet and reference manual, save them in doc

### adding cmsis
```sh
$ git submodule add  https://github.com/JBerg60/stm32-cmsis cmsis
```

When cloning the repo, the submodule must be initialized again:  
```sh
$ git submodule update --init --recursive
```


### preparing for build
Edit the CMakeLists.txt and make the folling adjustments:  
- in add_definitions(-DSTM32F030x6) set the correct processor  
- set the correct MCU => set(MCU "cortex-m0")  
- set the linker file => CMAKE_EXE_LINKER_FLAGS
- in cmake/cmsis.make set the correct processor family

Then make the build system, using Ninja
```sh
$ cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.cmake -G Ninja ..
```

Edit the sketch, the LED on this F030 board is on PA4.  
Run the build task(or use ```make``` in a bash terminal), build should be without any errors.  

note: for intellisense to work, these settings also need to be configured in ```c_cpp_properties.json```.

### Preparing the debugger
Edit the .vscode/launch.json, and set the device, svdFile and target.

### Semihosting
Semihosting can send printf commands (and some others) directly over the ST-Link interface to the Output window of visual Studio Code.  
There is no need to setup a UART and a serial monitor. This is a huge advantage. On the other hand, semihosting is not veri fast.
To use semihosting following needs to be done:  
- add ```LDFLAGS += --specs=lrdimon.specs -lrdimon``` to the linker flags.
- add ```extern "C" void initialise_monitor_handles(void);``` to the beginning of the sketch.
- add a call to ```initialise_monitor_handles();```  to the setup function.
Launch.json is already configured to use semihosting. Only the ```target``` and the ```device``` need to be set.


### Useful commands
```
make -> build the system (shortcut ctrl-shift b)  

make clean -> clean the system (shortcut ctrl-shift c)  

make info -> get info on the connected device (shortcut ctrl-shift i).  

make flash -> flash the firmware to the device (shortcut ctrl-shift f).  
                If necessary, the firmware is build before flashing. 

F5 starts the debugger in Visual Studio Code  

Ctrl-F5 stops the debugger.  

```

### Useful links  

Stefan frings (German) [STM32F0](http://stefanfrings.de/stm32/stm32l0.html)

## Authors

* **John Berg** - *Maintainer* - [NetbaseNext](https://netbasenext.nl)

See also the list of [contributors](https://github.com/JBerg60/Arduino-VSCode/graphs/contributors) who participated in this project.

## License

MIT License.

## Acknowledgments

* Stackoverflow
