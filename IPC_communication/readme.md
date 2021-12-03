# Add LUSB as a Static Libaray to QT Project

## Introduction
I want to show here how to compile a project with libusb as a static under windows.

## Table Of Contents

<!-- toc -->

- [Precautions](#Precautions)
- [Installation](#Installation)
- [Examples](#Examples)

<!-- tocstop -->


## Precautions 
Working under linux or mac is easy because the library would be installed by a package manger. 
But working under Windows is relatively hard because there are different compilers and adding 
a static libaray needs to have it compiled by the same compiler that you will use to compile the project.

There are different MinGW compilers, 64 and 32 bits, with _64 and without. There are two approaches
- 1. compile the static library on your PC.

Disadvantage:
you may find the project available for Visual Studio, and this means that you need to rewrite 
your project under visual studio. There will be difficulties compiling an open source project with Qmake.
Most of them are written with Cmake. This will lead to having both VS and Qt Creator on your PC and may
cause mix difficulties.

- 2. A better approach is install MSys2 under your windows PC, and here you would be able to install 
make as a compiler and install the static library you need and you will be sure it uses the same compiler.




## Installation

- Install MSys2

I used the following https://repo.msys2.org/distrib/x86_64/msys2-x86_64-20210228.exe file and installed in 
the following location  D:\msys2\

- Run MSys2 and install the following packages as follows
 - 1  pacman -S mingw-w64-x86_64-libusb  #this is the libusb package i need 
 - 2  pacman -Su
 - 3 pacman -S mingw-w64-x86_64-gcc
 - 4  gcc -v #to make sure gcc is installed 
 - 5  g++ -v # check g++ compiler 
 - 6  pacman -S make
 - 7  pacman --needed -S mingw-w64-x86_64-toolchain 
 - 8  pacman --needed -S mingw-w64-x86_64-toolchain (choose 9 for mingw-w64-x86_64-make  )
 - 9  pacman -Sy --needed
 - 10  pacman --needed -S bash
 - 11  make -h #test make is available
 - Install the gdb debugger
 -     pacman -S mingw-w64-x86_64-gdb 

- Add the compiler to Qt Creator
	Inside Tools->Options->Compilers Click on Add Manual GCC Compier
	for the C Language, the compiler path is: D:\msys2\mingw64\bin\gcc.exe
	for the C++ Language, use the compiler g++ located at D:\msys2\mingw64\bin\g++.exe
	
	Now go to Kits: Clone any available kit and 
	  Set compiler to use Msys2 Gcc and G++ that were defined last step
	  
- Now create a new project, lets call it cmakecompile that uses cmakecompile
	
	In the CMakeList.txt add the static library by adding the following lines 

	add_executable(cmakecompile main.c)
	target_link_libraries(cmakecompile "D:/msys2/mingw64/lib/libusb-1.0.a")


	In the main.c add the link to the header file and use the following code in the main to test
		#include "D:\msys2\mingw64\include\libusb-1.0\libusb.h"
		
		int main(int argc, char *argv[])
		{
			libusb_context *ctx;
			libusb_init( &ctx );
			libusb_exit(NULL);
			return 0;
		}

- Now compile as a release because the libusb added is not a debug file, here is the output i got
	-	12:28:16: Starting C:\Users\SHERIF\Desktop\USBProject\build-cmakecompile-Desktop_Qt_5_15_2_MSYS2_G_64_bit-Release\cmakecompile.exe ...
	-	12:28:16: C:\Users\SHERIF\Desktop\USBProject\build-cmakecompile-Desktop_Qt_5_15_2_MSYS2_G_64_bit-Release\cmakecompile.exe exited with code 0
	
	-	12:29:30: Starting C:\Users\SHERIF\Desktop\USBProject\build-cmakecompile-Desktop_Qt_5_15_2_MSYS2_G_64_bit-Release\cmakecompile.exe ...
	-	12:29:30: C:\Users\SHERIF\Desktop\USBProject\build-cmakecompile-Desktop_Qt_5_15_2_MSYS2_G_64_bit-Release\cmakecompile.exe exited with code 0

## Examples

To have some examples, use the following link https://libusb.info, download the windows binary which has a folder called examples

Note that this is the same library compiled and installed by MSys2. There are different libraries for lusb and some have very similar names.There is another library called  libusb-32 which has a different initialization method and differnt functions. But this one is the widely used one.
