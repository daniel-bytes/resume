# Resume
My resume, in HTML format, with a homemade web server to host it.

- *Website*: https://resume.daniel-battaglia.com
- *PDF*: https://github.com/daniel-bytes/resume/blob/master/DanielBattaglia.pdf

The server is a home rolled HTTP server written in C++, using Unix `poll` for multi-client support.  While not as robust as most production-grade web servers, it exists as an example of building a working high-level application server on top of a low-level TCP and HTTP server.

The server takes an optional argument for the port it runs on.  By default it runs on port 3000.

## Building and running the application

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

To publish the Docker image to Dockerhub:
```
./docker-publish.sh
```

To run the container on a production server (only Docker installed):
```
VERSION=xxx
docker pull danielbytes/resume:$VERSION
docker rm -vf resume 
docker run -d --name resume -p 80:3000 -e CACHE_FILES=true -e CACHE_TEMPLATES=true danielbytes/resume:$VERSION
```

To run directly on a host with Valgrind (assuming an already running container):
```
rm -rf ~/app
docker cp resume:/app ~/app
cd ~/app
docker stop resume
valgrind --log-file=valgrind_out.txt ./simple-serv 80
jobs
$ disown  -h  %1
$ jobs
```
