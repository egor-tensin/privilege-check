Privilege check
===============

[![CI](https://github.com/egor-tensin/privilege-check/workflows/CI/badge.svg)](https://github.com/egor-tensin/privilege-check/actions?query=workflow%3ACI)

Check your privilege. On Windows.

Displays a window, showing

* if the current user belongs to the BUILTIN\Administrators group,
* whether the process is "run as Administrator",
* if it has elevated privileges,
* process's "integrity level".

If it's not "run as Administrator", a button allows to actually re-run the
executable as Administrator, utilizing Windows' User Account Control feature.

Development
-----------

Build using CMake.
The project is Windows-only, so building with either MSVC or MinGW-64 is
required.

There's a Makefile with useful shortcuts to build the project in the .build/
directory (defaults to building with MinGW-w64):

    make build

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
