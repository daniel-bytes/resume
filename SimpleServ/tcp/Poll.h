#ifndef __POLL_H__
#define __POLL_H__

#include "Socket.h"
#include "ListenSocket.h"
#include "AcceptSocket.h"
#include "TcpMessageListener.h"
#include "Typedefs.h"

#include <array>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

class Poll
{
public:
  Poll(const port_t serverPort);
  void BlockingPoll(TcpMessageListener &listener);
  ~Poll();

private:
  bool PollSockets();
  void ProcessSockets(TcpMessageListener &listener);
  void OnListenSocketReceive();
  void OnAcceptSocketReceive(socket_t fd, TcpMessageListener &listener);
  void CleanupDisposedSockets();

private:
  std::vector<pollfd> _pfds;
  ListenSocket _listenSocket;
  std::unordered_map<socket_t, AcceptSocket> _acceptSockets;
  std::unordered_set<socket_t> _socketsToDispose;
  const int _socketTimeout = -1;
};

#endif //__POLL_H__