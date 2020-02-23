from ubuntu:19.04


RUN apt-get update && apt-get install -y \
  build-essential \
  scons \
  curl

RUN mkdir /app
RUN mkdir /app/src
RUN mkdir /app/public
VOLUME /app/public
WORKDIR /app/src

COPY ./SimpleServ/*.h /app/src/
COPY ./SimpleServ/*.cpp /app/src/
COPY ./SimpleServ/SConstruct /app/src

RUN scons -Q && scons -c

RUN cp /app/src/build/SimpleServ /app
RUN rm -rf /app/src

WORKDIR /app

EXPOSE 3000

COPY ./SimpleServ/public /app/public
RUN chmod -R +r /app/public

CMD [ "/app/SimpleServ", "3000" ]
