# Open Addressing Hash (WIP)

Study case of an Hash Table implementation using Modern C++

Expectation is for this class to be compatible with STL Algorithms and also Ranges

I'm taking the time on testing and having next questions answered:
   - Performance & Memory Managment: Vector vs Array
   - Performance: Raw Loops vs STL Algorithms
   - Performance: Differneces between algorithms for Open Addressing Collision resolution
   - Memory managment: Avoid creation and copy of objects when possible

## Getting Started

### Prerequisites

To build and run OpenAddressingHash, you will need:

- **C++20 compatible compiler**
- **CMake** (version 3.20 or higher)

### Setup

1. **Clone the repository**:
   ```bash
   git clone https://github.com/arielberardi/OpenAddressingHash.git
   cd OpenAddressingHash
   ```

2. **Configure the project with CMake**:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. **Build the project**:
   ```bash
   cmake --build .
   ```

### Running the Application

Once built, you can run the application from the `build` directory:

```bash
./OpenAddressingHash
```

## Contact

For any questions or suggestions, please reach out [Ariel Berardi](https://www.linkedin.com/in/aberardi95/).
