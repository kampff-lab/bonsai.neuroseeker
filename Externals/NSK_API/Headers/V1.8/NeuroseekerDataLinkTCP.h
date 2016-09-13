#ifndef NeuroseekerDataLinkTCP_h_
#define NeuroseekerDataLinkTCP_h_

#include "NeuroseekerDataLinkIntf.h"
#include "Easics_FileDescriptor.h"
#include "Easics_TCPSocket.h"

#include <string>

class NeuroseekerDataLinkTCP : public NeuroseekerDataLinkIntf
{
public:
  NeuroseekerDataLinkTCP(const std::string & host, unsigned short port);
  ~NeuroseekerDataLinkTCP();

  bool readn(char * buffer, size_t size);
  bool writen(char * buffer, size_t size);

private:
  easics::TCPSocket socket_;
  easics::FileDescriptor * fd_;
};

#endif
