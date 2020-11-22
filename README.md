# Resume
My resume, in HTML format, with a homemade web server to host it.

- *Website*: https://resume.daniel-battaglia.com
- *PDF*: https://github.com/daniel-bytes/resume/blob/master/DanielBattaglia.pdf

The server is a home rolled HTTP server written in C++, using Unix `poll` for multi-client support.  While not as robust as most production-grade web servers, it exists as an example of building a working high-level application server on top of a low-level TCP and HTTP server.

The server takes an optional argument for the port it runs on.  By default it runs on port 3000.

## Server Architecture

The `SimpleServ` HTTP server is built in layers, from TCP at the bottom to the application layer at the top.

Communication between layers is done via `Listener` interfaces, where the layer above will implement the interface and pass a reference to itself down to the lower layer's bootstrapping function.

### TCP layer

The TCP layer is the lowest layer of the server. It houses the TCP socket management and polling code.

The `TcpServer` class is the entrypoint to the TCP layer; the `BlockingListen` method is called which takes a reference to a `TcpMessageListener` to handle incoming TCP data.  Behind the scenes, `TcpServer` listens for connections using an instance of the `Poll` class, which wraps the standard Unix `poll` socket function. The `Poll` class manages the actual listen and accept socket wrapper classes lifetimes.  When `Poll` accepts a socket, it buffers all incoming data and passes to the `TcpMessageListener` for processing, and then sends any return value back to the socket as a response (this means that request / response is currently built into the TCP layer itself, perhaps in the future this can be broken out to happen at the HTTP layer only).

### HTTP layer

The `HttpServer` has a simlar interface to the `TcpServer`, with a `BlockingListen` function for starting the server and blocking the current thread while HTTP requests are processed.  Internally, it houses a `TcpServer` that it forwards the `BlockingListen` call to, as well as implementing the `TcpMessageListener` interface for handling the TCP requests and responses. When a TCP request is received, the `TcpMessageReceived` function is called, which parses the bytes and, if successfully parsed, converts to a typed `HttpRequestMessage` for processing, and returns a `HttpResponseMessage` for converting back to bytes to send back to the TCP socket as a response.  `HttpServer` is meant to be inherited, with the inheriting class overriding the `HttpMessageReceived` for actual business logic.

### HTML layer

The HTML layer handles some typical functions used in a HTML-based web framework:

- serving HTML and asset files from disk (`FileServer`)
- Mustache-style HTML templating (`TemplateParser`)
- MVC style data models to inject into HTML templates (`Model`)

### Application layer

The application layer is the top layer, handling URL routing and error handling.  The `AppServer` class is instantiated directly from the application's `main` function, and handles ownership of the underlying `TcpServer`.

## Building and running the server application

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
