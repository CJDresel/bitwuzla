[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![CI](https://github.com/bitwuzla/bitwuzla/workflows/CI/badge.svg)

# Bitwuzla

Bitwuzla is a Satisfiability Modulo Theories (SMT) solvers for the theories
of fixed-size bit-vectors, floating-point arithmetic, arrays, uninterpreted
functions and their combinations.

If you are using Bitwuzla in your work, or if you incorporate it into software
of your own, we invite you to send us a description and link to your
project/software, so that we can link it as third party
application on [bitwuzla.github.io](https://bitwuzla.github.io).

## Website

More information about Bitwuzla is available at: https://bitwuzla.github.io

## Download

The latest version of Bitwuzla is available on GitHub:
https://github.com/bitwuzla/bitwuzla

## Required Dependencies

- [Python >= 3.7](https://www.python.org)
- [Meson >= 0.64](https://mesonbuild.com)
- [Ninja](https://ninja-build.org)
- [GMP v6.1 (GNU Multi-Precision arithmetic library)](https://gmplib.org)
- [CaDiCaL](https://github.com/arminbiere/cadical)
- [SymFPU](https://github.com/martin-cs/symfpu)

## Optional Dependencies

Bitwuzla can be optionally built with support for the following SAT solvers:
- [CryptoMiniSat](https://github.com/msoos/cryptominisat)
- [Kissat](https://github.com/arminbiere/kissat)

**Note**
- If the build system does not find SymFPU or one of the supported SAT solvers
  installed on your system, it can download and build a suitable version
  itself.
- Bitwuzla can be built with support for multiple SAT solvers.


# Build

Bitwuzla can be built on Linux, macOS and Windows.

## Linux and Unix-like OS

Assume that we build Bitwuzla with CaDiCaL:
```
# Download and build Bitwuzla
git clone https://github.com/bitwuzla/bitwuzla
cd bitwuzla

# Configure Bitwuzla
./configure.py

# Build
cd build && meson compile
```

The `bitwuzla` binary can be found in directory `build/src/main`,
the built libraries (`libbitwuzla.*`) in `build/src`.

For more build configuration options of Bitwuzla, see `./configure.py -h`.

## Windows (TODO)

For instructions on how to build Bitwuzla on Windows, see [here](
  https://github.com/bitwuzla/bitwuzla/blob/main/docs/building_on_windows.rst).


## Python Bindings

Bitwuzla Python bindings can be installed via `pip` as follows:

```
pip install bitwuzla
```

If you want to install a local version of the Bitwuzla Python bindings

```
git clone https://github.com/bitwuzla/bitwuzla
cd bitwuzla
pip install .
```

## API documentation (TODO)

To build the API documentation of Bitwuzla, it is required to install
* [Doxygen](https://www.doxygen.nl)
* [Sphinx](https://www.sphinx-doc.org) (>= version 1.2),
* [sphinxcontrib-bibtex](https://sphinxcontrib-bibtex.readthedocs.io)
* [Breathe](https://breathe.readthedocs.io)

Then build Bitwuzla, configure with option `--docs` and generate the
documentation as follows:
```
cd build
make docs
```
The documentation is generated into `build/docs/sphinx`.

**Note**
Make sure to build Bitwuzla with Python bindings, else the documentation of
its Python API will not be included.

### Linking against Bitwuzla in CMake projects (TODO)

Bitwuzla's build system provides a CMake package configuration, which can be
used by the `find_package()` command to provide information about Bitwuzla's
include directories, libraries and it's dependencies.

After installing Bitwuzla you can issue the following commands in your CMake
project to link against Bitwuzla.
```
find_package(Bitwuzla)
target_link_libraries(<your_target> Bitwuzla::bitwuzla)
```

## Contributing

Bitwuzla is distributed under the MIT license
(see [COPYING](https://github.com/bitwuzla/bitwuzla/blob/main/COPYING)
file).
By submitting a contribution you automatically accept the conditions described
in [COPYING](https://github.com/bitwuzla/bitwuzla/blob/main/COPYING).
Additionally, we ask you to certify that you have the right to submit such
contributions.
To manage this process we use a mechanism known as
[Developer Certificate of Origin](https://developercertificate.org), which
can be acknowledged by signing-off your commits with `git commit -s`.
We require all pull requests to be squashed into a single commit and
signed-off.


```
Developer Certificate of Origin
Version 1.1

Copyright (C) 2004, 2006 The Linux Foundation and its contributors.
1 Letterman Drive
Suite D4700
San Francisco, CA, 94129

Everyone is permitted to copy and distribute verbatim copies of this
license document, but changing it is not allowed.


Developer's Certificate of Origin 1.1

By making a contribution to this project, I certify that:

(a) The contribution was created in whole or in part by me and I
    have the right to submit it under the open source license
    indicated in the file; or

(b) The contribution is based upon previous work that, to the best
    of my knowledge, is covered under an appropriate open source
    license and I have the right under that license to submit that
    work with modifications, whether created in whole or in part
    by me, under the same open source license (unless I am
    permitted to submit under a different license), as indicated
    in the file; or

(c) The contribution was provided directly to me by some other
    person who certified (a), (b) or (c) and I have not modified
    it.

(d) I understand and agree that this project and the contribution
    are public and that a record of the contribution (including all
    personal information I submit with it, including my sign-off) is
    maintained indefinitely and may be redistributed consistent with
    this project or the open source license(s) involved.
```
