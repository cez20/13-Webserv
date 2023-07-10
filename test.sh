#!/bin/bash

# Creates a separate build folder with its own  
mkdir build
cd build
cmake ..
make


# Launch the differents tests.
../webservTests