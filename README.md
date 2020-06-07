# althas

*althas* is a header-only C++ library that provides unit test facilities for Verilator. It will help you manage *tracing*, *clocking*, *testing* and *assertion* in a highly-customizable and easy-to-use way. This library is intended to simplify current Verilator-based hardware simulation & testing procedure which is full of boilerplate, by providing a small but compact toolbox that covers many facets, from tracing to assertion.

## Install

[CMake](https://cmake.org/) is used to configure the install process. You should install CMake and Verilator beforehand, then run following commands:

```bash
$ git clone https://github.com/Ray-Eldath/althas
$ cd althas
$ cmake .
$ sudo make install
```

## Usage

When *althas* is installed, the header files and a CMake configuration file will be copied to your device. Add following lines to you `CMakeList.txt` will link *althas* to your target:

```cmake
find_package(althas REQUIRED)
target_link_libraries(<TARGET_NAME> PRIVATE althas::althas)
```

After that, you could include *althas* in your own project by:

```c++
#include <althas/althas.hpp>
```

**This is thoroughly demonstrated in `example/find_package`.  Checking example projects is highly recommended.**

---

*althas* is consisted of some basic components:

- `TESTBENCH`: Basic management facility which have nothing to do with test cases and whatnot. It contains code for *tracing* and wrapped basic logic such as *raise the clock, dump, then lower the clock, dump again*.
- `TESTER`: Testing facility that provides lifecycle-focused, testcase-based style of testing, and assertion. You can control the whole testing procedure by overriding a handful lifecycle functions, `beforeAll`, `afterAll`; `beforeEach`, `onEach` and `afterEach`. Among them the `onEach` is a *pure virtual function*, meaning that `onEach` have to be overwritten cause it control the necessary “changing-state” part of a testcase-based testing process. Generally `afterEach` is for *assertion* and state checking if needed. For assertion, `TASSRERT` and `TASSERT_INFO` is provided for typical assertion and assertion with a message printed first, which is very useful and recommended in most cases.
- `TPRINTER`: Helper class for indented printing. `TPRINTF` and `TPRINTFI` (Testing PRINTF Indented) is defined, use them at your discretion.

There are some configuration macros defined in `althas/config.h`. If you need to overwrite these configuration, you should include *althas* as:

```c++
#include "althas/config.h" // redefine config macros as demonstrated here

#undef CLOCK_WIRE
#define CLOCK_WIRE clock
#undef RESET_WIRE
#define RESET_WIRE reset

#include "althas/althas.hpp"
```

Here we redefine the `CLOCK_WIRE` and `RESET_WIRE`, which respectively stands for *the name of the clock wire* and *the name of the reset wire*. Except these two, there are other configuration macros defined in `althas/config.h`:

```c++
// config macros
#define RESET_LINE_ENABLED // whether the DUT have the RESET line. if not, logic for reset will not be compiled.
// #define FAIL_ASSERTION // whether use a C assert at every althas assertion. if enable, failed assertion will cause the program interrupted.

#define CLOCK_WIRE clk // the name of the clock wire
#define RESET_WIRE rst // the name of the reset wire
```

**Caveat:** If a wire, or logic is prefixed with underscore(`_`), then it will be hidden from Verilator tracing, thus will not display on your gtkwave. You should rename the wire, or add `VERILATOR_ARGS "--trace-underscore" ` to your `verilate` parameters.

## Examples

Some simple but typical examples are located in `example/find_package` and `example/pc`. They both demonstrate the general layout of *althas*-based testing procedure. The former is based on CMake `find_package` and the DUT is simpler as well, while the latter is directly included by `add_subdirectory` with a little more complicated DUT. **Checking these example projects is strongly recommended.**

## And last…

- The CLion license which is necessary for development is licensed with Jetbrains Open Source License with no charge. Thanks to Jetbrains!
  
  [![Jetbrains](https://raw.githubusercontent.com/Ray-Eldath/Avalon/master/img/jetbrains-variant-4.jpg)](https://www.jetbrains.com/?from=sirius)



- I programmed most of the DUT in SystemVerilog using Edaphic Studio’s SystemVerilog plugin. I have to say get rid of Vivado surprisingly unwieldy yet surprisingly incomplete and troublesome built-in editor, which is ironic, is a great pleasure. Their plugin is handy, multifunctional, even provides nice reformatting and renaming for your code. Coding in one IDE with the plugin and Verilator is undoubtedly the best workflow I’ve ever tried for such task, I’m planning to write a blog post about the workflows I’ve explored once my blog is on. They also provides free license for non-commercial project and student work, please check out [their homepage](https://www.edaphic.studio/) for more information.
- Please star the project if it really helps you. And if not, **feel free to open a issue, or send me an email for feature request or bug report**. I’m glad to know how to improve my project. Many thanks!