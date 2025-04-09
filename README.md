# Thalia
> A fast, statically typed, general-purpose, procedural programming language

[![Version: v0.0.0](https://img.shields.io/badge/version-v0.0.0-red)](https://github.com/thalia-lang/thalia/blob/master/README.md)
[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](http://www.gnu.org/licenses/lgpl-3.0)

## Contents
- [Building and Installation](#building-and-installation)
  - [Dependencies](#dependencies)
  - [Building the project](#building-the-project)
  - [Running tests](#running-tests)
  - [Running the program](#running-the-program)
  - [Installing](#installing)
- [License](#license)
- [Contributing](#contributing)

## Building and Installation

### Dependencies
- C++ compiler (gcc, clang, ...)
- Build system generator: cmake
- Package manager: vcpkg 

### Building the project
To set the compiler that will be used, you can use:
```sh
# for clang
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

# for gcc
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
```

To set the path to the directory where vcpkg is installed, you can use:
```sh
export VCPKG_ROOT=/path/to/vcpkg
```

To compile the project, we first need to make 'build.sh' script to be executable with:
```sh
chmod +x ./build.sh
```

Now, we can use 'build.sh' script to compile the project:
```sh
./build.sh
```

### Running the program
If everything went well with the compilation we can run the executable:
```sh
./build/thalia
```

### Installing
To install the app run:
```sh
sudo make install
```
By default the app will be installed in `/usr/local/bin/` directory.

## License
Thalia is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
For more details, see [LICENSE](https://github.com/thalia-lang/thalia/blob/master/LICENSE) file.

## Contributing
Contributions are welcome.
If you have a feature request, or have found a bug, feel free to open a [new issue](https://github.com/thalia-lang/thalia/issues/new).
If you wish to contribute code, see [CONTRIBUTING.md](https://github.com/thalia-lang/thalia/blob/master/CONTRIBUTING.md) for more details.

