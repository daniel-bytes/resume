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
  Poll(port_t httpPort, const SslContext &context);
  ~Poll();

  void BlockingPoll(TcpMessageListener &listener);

private:
  bool PollSockets();
  void ProcessSockets(TcpMessageListener &listener);
  void OnListenSocketReceive(ListenSocket *listenSocket);
  void OnAcceptSocketReceive(AcceptSocket *acceptSocket, TcpMessageListener &listener);
  void CleanupDisposedSockets();

private:
  std::vector<pollfd> _pfds;
  std::vector<ListenSocket*> _listenSockets;
  std::unordered_map<socket_t, AcceptSocket*> _acceptSockets;
  std::unordered_set<socket_t> _socketsToDispose;

  const int _socketTimeout = -1;
};

#endif //__POLL_H__