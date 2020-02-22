from ubuntu:19.04


RUN apt-get update && apt-get install -y \
  build-essential \
  scons \
  curl

WORKDIR /app

COPY ./SimpleServ/*.h /app/
COPY ./SimpleServ/*.cpp /app/
COPY ./SimpleServ/SConstruct /app
COPY ./SimpleServ/public /app/public

RUN scons

EXPOSE 3000

CMD [ "/app/build/SimpleServ", "3000" ]
