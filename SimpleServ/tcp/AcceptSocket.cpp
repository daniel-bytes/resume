#include "AcceptSocket.h"
#include "TcpError.h"
#include "Log.h"

AcceptSocket::AcceptSocket(int listenSocket) {
  _socket = accept(listenSocket, NULL, NULL);
  
  if (IsActive()) {
    SetNonBlocking();
  } else if (errno != EWOULDBLOCK) {
    // don't throw here since this is an accept socket
    Log::LogSocketError("accept", listenSocket, _socket);
  }
}