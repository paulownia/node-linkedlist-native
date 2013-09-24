node-linkedlist
======

A native implemantation of linked list for node.js. 

## To build

Prerequisites

* node v0.8 or later
* GCC (C++11 Compiler)

install node-gyp 
   
    npm install -g node-gyp

build

    node-gyp configure
    node-gyp build CXXFLAGS="-std=c++11"

or

    env CXXFLAGS="-std=C++11" node-gyp rebuild
