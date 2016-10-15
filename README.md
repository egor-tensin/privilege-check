Privilege check
===============

Check your privilege. On Windows.

Displays a window, showing

* if the current user belongs to the BUILTIN\Administrators group,
* whether the process is "run as Administrator",
* if it has elevated privileges,
* process's "integrity level".

If it's not "run as Administrator", a button allows to actually re-run the
executable as Administrator, utilizing Windows' User Account Control feature.

Building
--------

Create the build files using CMake and build using your native build tools
(Visual Studio/make/etc.).

For example, using Visual Studio Community 2015 Update 3 (targetting x86-64):

    > cd
    D:\workspace\build\privilege-check\msvc

    > cmake -G "Visual Studio 14 2015 Win64" D:\workspace\personal\privilege-check
    ...

    > cmake --build . --config release
    ...

Another example, using Cygwin + MinGW-w64 + `make` (again, targetting x86-64):

    > pwd
    /cygdrive/d/workspace/build/privilege-check/mingw/release

    > cmake -G "Unix Makefiles"                         \
        -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++    \
        -D CMAKE_RC_COMPILER=x86_64-w64-mingw32-windres \
        -D CMAKE_BUILD_TYPE=Release                     \
        /cygdrive/d/workspace/personal/privilege-test
    ...

    > cmake --build . -- -j
    ...

License
-------

Distributed under the MIT License.
See [LICENSE.txt] for details.

[LICENSE.txt]: LICENSE.txt
