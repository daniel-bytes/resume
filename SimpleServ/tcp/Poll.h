#ifndef __POLL_H__
#define __POLL_H__

#include "Socket.h"
#include "ListenSocket.h"
#include "AcceptSocket.h"
#include "TcpMessageListener.h"

#include <array>
#include <map>
#include <unordered_set>
#include <vector>
#include <memory>

class Poll
{
public:
  Poll(int serverPort);
  void BlockingPoll(TcpMessageListener &listener);

private:
  bool PollSockets();
  void ProcessSockets(TcpMessageListener &listener);
  void OnListenSocketReceive();
  void OnAcceptSocketReceive(int fd, TcpMessageListener &listener);
  void CleanupDisposedSockets();

private:
  std::vector<pollfd> _pfds;
  std::unique_ptr<ListenSocket> _listenSocket;
  std::map<int, std::unique_ptr<AcceptSocket>> _acceptSockets;
  std::unordered_set<int> _socketsToDispose;
  const int _socketTimeout = -1;
};

#endif //__POLL_H__