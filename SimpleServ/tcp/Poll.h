#ifndef __POLL_H__
#define __POLL_H__

#include "ListenSocket.h"
#include "Ssl.h"
#include "TcpMessageListener.h"
#include "Typedefs.h"
#include "shared/Configuration.h"

#include <array>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * Poll manages the "poll" socket function, passing the raw data to a
 * TcpMessageListener for processing and returning the results back to the socket.
 */
class Poll
{
public:
  Poll(port_t httpPort, const SslConfiguration &sslConfig);
  ~Poll();

  /**
   * Begin polling on all listen and accept sockets, blocking the current thread.
   * Data I/O is processed via the input TcpMessageListener instance.
   */
  void BlockingPoll(TcpMessageListener &listener);

// Private functions below handle the various implementation steps taken in BlockingPoll
private:
  bool PollSockets();
  void ProcessSockets(TcpMessageListener &listener);
  void OnListenSocketReceive(ListenSocket *listenSocket);
  void OnAcceptSocketReceive(AcceptSocket *acceptSocket, TcpMessageListener &listener);
  void CleanupDisposedSockets();

private:
  // Stores a pollfd for the combined set of all listen and accept sockets
  std::vector<pollfd> _pfds;

  // Stores all configured listen sockets by their socket fd
  std::unordered_map<socket_t, ListenSocket*> _listenSockets;

  // Stores all current accept sockets by their socket fd
  std::unordered_map<socket_t, AcceptSocket*> _acceptSockets;

  // Stores all socket fds flagged for disposal at the end of the current polling round
  std::unordered_set<socket_t> _socketsToDispose;

  // Stores all socket fds flagged for retry
  std::unordered_set<socket_t> _socketsToRetry;
};

#endif //__POLL_H__