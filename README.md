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

* **Example (Visual Studio).**
In the example below, the project directory is
"C:\workspace\personal\privilege-check" and Visual Studio 2015 is used,
targeting x86-64.

      > cmake -G "Visual Studio 14 2015 Win64" C:\workspace\personal\privilege-check
      ...

      > cmake --build . --config release
      ...

* **Example (MinGW-w64).**
Another example, this time using Cygwin + MinGW-w64, targeting x86-64
(the project directory is "/cygdrive/c/workspace/personal/privilege-check").

      > cmake -G "Unix Makefiles"                         \
          -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++    \
          -D CMAKE_RC_COMPILER=x86_64-w64-mingw32-windres \
          -D CMAKE_BUILD_TYPE=Release                     \
          /cygdrive/c/workspace/personal/privilege-test
      ...

      > cmake --build . -- -j
      ...

Cross table
-----------

Snippets of information provided by the tool on different Windows versions
are summarized in the table below.

"Administrator" below stands for the built-in Administrator account.
"testadmin" is a user that belongs to the BUILTIN\Administrators group.
"test" is a user that doesn't belong to this group.

### UAC on

| User          |                 | Windows 7 | Windows 8.1 | Windows 2012 R2
| ------------- | --------------- | --------- | ----------- | ---------------
| Administrator | Integrity level | -         | -           | High
|               | Elevated?       | -         | -           | True
| testadmin     | Integrity level | Medium    | Medium      | Medium
|               | Elevated?       | False     | False       | False
| test          | Integrity level | Medium    | Medium      | Medium
|               | Elevated?       | False     | False       | False

### UAC off

| User          |                 | Windows 7 | Windows 8.1 | Windows 2012 R2
| ------------- | --------------- | --------- | ----------- | ---------------
| Administrator | Integrity level | -         | -           | High
|               | Elevated?       | -         | -           | True
| testadmin     | Integrity level | High      | High        | High
|               | Elevated?       | True      | True        | True
| test          | Integrity level | Medium    | Medium      | Medium
|               | Elevated?       | False     | False       | False*

\* True on domain controllers.

License
-------

Distributed under the MIT License.
See [LICENSE.txt] for details.

[LICENSE.txt]: LICENSE.txt
