#ifndef __TCPMESSAGELISTENER_H__
#define __TCPMESSAGELISTENER_H__

#include <optional>
#include <string>

/**
 * Interface used for a type that receives TCP messages
 */
class TcpMessageListener
{
public:
  /** Called by the socket Poll layer when data is received **/
	virtual std::string TcpMessageReceived(
    const std::string &msg, 
    const std::optional<std::string> &ipAddress,
    const port_t port
  ) = 0;
};

#endif //__TCPMESSAGELISTENER_H__