mkdir -p build
cmake -S . -B build
cd build
cmake --build .
./build/CTSBuild