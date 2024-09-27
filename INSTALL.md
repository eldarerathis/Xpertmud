## Building

### Dependencies

Xpertmud depend these libs:
 * Qt5 => 5.11 is the current minimum
 * Perl5 => 5.4 - 5.8 tested on MacOS 10.10 and ActivePerl 5.24.0 on Windows.
 * Python 3.x => 3.9 and 3.12 tested on Windows
 * ZLib - tested system zlib on MacOs 10.10

Python needed for Python interpretator plugin, Perl - for Perl plugin and zlib - for MCCP support.
All this is optional and can be enabled separately.

All libs are 32bit.

Strawberry Perl can be used to link, but it don't load, because libstdc++-6.dll is differ between Qt and Strawberry.

These CMake vars can be used:

* QTDIR - set Qt libs root dir on Windows and MacOS.
* PYTHON_SUPPORT - enable build Python plugin (default ON)
* PERL_SUPPORT - enable build Perl plugin (default ON)
* MCCP_SUPPORT - enable support MCCP (default OFF)

### Windows

The following works (for me) to build on Windows 11 with VS 2022:

* Configure: `cmake .\CMakeLists.txt -A Win32 -DCMAKE_INSTALL_PREFIX=<where-to-install> -DQTDIR=<path-to-Qt>`
    * Alternatively, set the `QTDIR` environment variable and omit `-DQTDIR`
    * Building with ClangCL works as well by adding `-T ClangCL`
* Build: `cmake --build . --config Release`
    * Manually build xpertmud: `& '<your-VS-install-Path>\MSBuild\Current\Bin\MSBuild.exe' .\xpertmud.vcxproj /p:Configuration=Release`
    * Manually build a scripting plugin (python, for e.g.): `& '<your-VS-install-path>\MSBuild\Current\Bin\MSBuild.exe' .\xpertmud.src\scripting\python\libxmpythoninterpreter.vcxproj /p:Configuration=Release`
* Install: `cmake --install .`

### Non-Windows

Building steps all as any CMake-powered project:
 
 * `cmake . [-DCMAKE_INSTALL_PREFIX=<install-path>]`
 * `make`
 * `make install`

## Installation

Installation is very simple.
Destination path can be defined by CMAKE_INSTALL_PREFIX variable.
For execute installation process run 'make install' in cmake build dir. It will copy all necessary files to
defined directory. On Windows and MacOS Qt libs will be included.
