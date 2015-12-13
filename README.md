node-linkedlist-native
======

A native implemantation of linked list for node.js.

Prerequisites

* node v0.11 or later
* GCC (C++11 Compiler)

## To install

    export CXXFLAGS="-std=c++11"
    npm install git://github.com/paulownia/node-linkedlist-native.git


## To build

install node-gyp

    npm install -g node-gyp

configure & build

    node-gyp configure
    node-gyp build CXXFLAGS="-std=c++11"

or rebuild (clean, configure and build)

    env CXXFLAGS="-std=C++11" node-gyp rebuild
