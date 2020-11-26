FROM ubuntu:20.10

RUN apt-get update && apt-get install -y \
  build-essential \
  openssl \
  libssl-dev \
  scons \
  curl \
  strace

RUN mkdir /app
RUN mkdir /app/src
RUN mkdir /app/private
RUN mkdir /app/public
RUN mkdir /app/public/.well-known
VOLUME /app/private
VOLUME /app/public
WORKDIR /app/src

COPY ./SimpleServ/main.cpp /app/src/
COPY ./SimpleServ/shared/*.h /app/src/shared/
COPY ./SimpleServ/shared/*.cpp /app/src/shared/
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

EXPOSE 3080
EXPOSE 3443
ARG ARG_VERSION
ENV APP_VERSION=$ARG_VERSION

COPY ./SimpleServ/public /app/public
COPY ./version /app/public/version
RUN chmod -R +r /app/private
RUN chmod -R +r /app/public

CMD [ "/app/simple-serv", "3080", "3443" ]
