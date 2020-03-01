# Resume
My resume, in HTML format, with a homemade web server to host it.

- *Website*: https://resume.daniel-battaglia.com
- *PDF*: https://github.com/daniel-bytes/resume/blob/master/DanielBattaglia.pdf

The server is a home rolled HTTP server written in C++, using Unix `poll` for multi-client support.  While not as robust as most production-grade web servers, it exists as an example of building a working high-level application server on top of a low-level TCP and HTTP server.

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
