# Open Addressing Hash


## Getting Started

### Prerequisites

To build and run OpenAddressingHash, you will need:

- **C++20 compatible compiler**
- **CMake** (version 3.20 or higher)
- **Google Test** (automatically fetched via CMake if not present)

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

4. **Run Tests**:
   TradeZest uses Google Test for unit testing. To run the tests:
   ```bash
   ctest
   ```

### Running the Application

Once built, you can run the application from the `build` directory:

```bash
./OpenAddressingHash
```

## Contact

For any questions or suggestions, please reach out [Ariel Berardi](https://www.linkedin.com/in/aberardi95/).
