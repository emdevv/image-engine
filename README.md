# Image Engine

> A lightweight C++ command-line image processing engine built with **SFML** and **CLI11**. Perform common image manipulations such as cropping, blurring, and rotating through a simple and intuitive CLI.

---

## Features

- Load images from disk
- Crop images using custom coordinates
- Apply adjustable blur effects
- Rotate images (90°, 180°, 270°)
- Display original and processed images using SFML
- Fast and lightweight C++ implementation
- Clean command-line interface powered by CLI11

---

## Project Structure

```
image-engine/
├── include/          # Header files
├── src/
│   ├── ops/          # Image operations
│   └── main.cpp
├── external/
│   ├── CLI11/
│   └── stb/
└── README.md
```

---

## Requirements

Before building the project, make sure you have:

- C++17 compatible compiler
- CMake (recommended)
- SFML
- CLI11 (included)
- stb_image (included)

### Supported Platforms

- Windows
- Linux
- macOS (with SFML installed)

---

## Installation

### 1. Clone the repository

```bash
git clone https://github.com/emdevv/image-engine.git
cd image-engine
```

### 2. Build the project

Using CMake:

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

Or compile manually with your preferred compiler and link against SFML.

---

## Usage

General syntax:

```bash
image-engine <image> <command> [arguments]
```

## Technologies Used

- C++17
- SFML
- CLI11
- stb_image

---

## Future Improvements

- 

---

## Contributing

Contributions are welcome!

Feel free to open an issue or submit a pull request to improve the project.

---

## License

This project is available under the MIT License. Feel free to use, modify, and distribute it.

---
