from ubuntu:19.10

RUN apt-get update && apt-get install -y \
  build-essential \
  scons \
  curl \
  strace

RUN mkdir /app
RUN mkdir /app/src
RUN mkdir /app/public
VOLUME /app/public
WORKDIR /app/src

COPY ./SimpleServ/main.cpp /app/src/
COPY ./SimpleServ/app/*.h /app/src/app/
COPY ./SimpleServ/app/*.cpp /app/src/app/
COPY ./SimpleServ/html/*.h /app/src/html/
COPY ./SimpleServ/html/*.cpp /app/src/html/
COPY ./SimpleServ/http/*.h /app/src/http/
COPY ./SimpleServ/http/*.cpp /app/src/http/
COPY ./SimpleServ/tcp/*.h /app/src/tcp/
COPY ./SimpleServ/tcp/*.cpp /app/src/tcp/
COPY ./SimpleServ/SConstruct /app/src

RUN scons -Q && scons -c

RUN cp /app/src/build/simple-serv /app
RUN rm -rf /app/src

WORKDIR /app

EXPOSE 3000
ARG ARG_VERSION
ENV APP_VERSION=$ARG_VERSION

COPY ./SimpleServ/public /app/public
COPY ./version /app/public/version
RUN chmod -R +r /app/public

CMD [ "/app/simple-serv", "3000" ]
