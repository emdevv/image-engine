This document aims to provide all the necessary information to get picceler to build on your system.

We recommend building on Linux, and the following commands assume you are running Linux, but it is likely for picceler to build on Windows as well.


# Dependencies

We currently have the following dependencies: 
* **SFML** -- most versions should work
* **SYCL** -- most versions should work
* **googletest** - most versions should work
* **CLI11** - most versions should work


**googletest**
```bash
git clone https://github.com/google/googletest.git && cd googletest
mkdir build && cd build
cmake .. && cmake --build . -j
sudo cmake --install .
```

**CLI11**
```bash
git clone https://github.com/CLIUtils/CLI11.git && cd CLI11
mkdir build && cd build
cmake .. && cmake --build . -j
sudo cmake --install .
```

**SFML**
```bash
sudo dnf install SFML-devel
```

**SYCL**
```bash
sudo dnf install intel-oneapi-compiler-dpcpp-cpp
# If you have Intel GPU :
# if you have intel gpu -- sudo dnf install intel-level-zero-gpu intel-opencl
echo "alias init_sycl='source /opt/intel/oneapi/setvars.sh'" >> ~/.bashrc
source ~/.bashrc
init_sycl
```

### You are done! Now let's compile image-engine!

```bash
git clone https://github.com/emdevv/image-engine.git && cd image-engine
mkdir build && cd build
cmake -DCMAKE_CXX_COMPILER=icpx ..
cmake --build . -j
