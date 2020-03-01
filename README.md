# Resume
My resume, in HTML format, with a homemade web server to host it.

- *Website*: https://resume.daniel-battaglia.com
- *PDF*: https://github.com/daniel-bytes/resume/blob/master/DanielBattaglia.pdf

The server is a home rolled HTTP server written in C++.

The C++ server is most definitely NOT production ready (no event loop, uses blocking IO, uses low-level socket APIs instead of a more versatile framework).  It exists to show an example of some real working C++ code and design, using the standard library and OS networking APIs, as well as abstracting OS-specific code.

The server takes an optional argument for the port it runs on.  By default it runs on port 3000.

## Scripts

To build locally, install `scons` and run:
```
./local-build.sh
```

To run locally after building:
```
./local-run.sh
```

To build as a Docker image:
```
./docker-build.sh
```

To run a built Docker image as a container:
```
./docker-run.sh
```

to publish the Docker image to Dockerhub:
```
./docker-publish.sh
```
