# resume
My resume, in HTML format, with a homemade web server to host it

https://github.com/daniel-bytes/resume/blob/master/DanielBattaglia.pdf

The server is a homemade HTTP server written in C++.

The C++ server is most definitely NOT production ready (no event loop, uses blocking IO, uses low-level socket APIs instead of a more versatile framework).  It exists to show an example of some real working C++ code and design, using the standard library and OS networking APIs, as well as abstracting OS-specific code.

The server takes an optional argument for the port it runs on.  By default it runs on port 3000.

To build on Windows in Visual Studio, just open the solution and build.

To build on OSX/Linux, make sure you install scons and run the "scons" command from the terminal. To build and run, just use: scons && ./build/SimpleServe 3000

Note that the unit tests are currently only building in Visual C++ on Windows.
