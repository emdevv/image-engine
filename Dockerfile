FROM fedora:latest

# Install system tools, CMake, SFML, and procps (for 'ps')
RUN dnf update -y && dnf install -y \
    gcc-c++ \
    cmake \
    git \
    ninja-build \
    gtest-devel \
    SFML-devel \
    procps-ng \
    wget \
    dnf-plugins-core \
    && dnf clean all

# Add Intel oneAPI repo
RUN tee /etc/yum.repos.d/oneAPI.repo <<EOF
[oneAPI]
name=Intel oneAPI repository
baseurl=https://yum.repos.intel.com/oneapi
enabled=1
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://yum.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB
EOF

# Install Intel oneAPI compiler
RUN dnf install -y \
    intel-basekit-getting-started \
    intel-oneapi-dpcpp-cpp-compiler \
    && dnf clean all

WORKDIR /workspace