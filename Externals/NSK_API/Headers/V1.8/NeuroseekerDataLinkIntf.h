#ifndef NeuroseekerDataLinkIntf_h_
#define NeuroseekerDataLinkIntf_h_

#include "dll_import_export.h"

#include <string>
#include <fstream>

class DLL_IMPORT_EXPORT NeuroseekerDataLinkIntf
{
public:
  NeuroseekerDataLinkIntf();
  virtual ~NeuroseekerDataLinkIntf();

  virtual bool readn(char * buffer, size_t size) = 0;
  virtual bool writen(char * buffer, size_t size);

  // record incoming data
  void startRecording(const std::string & filename = "record.nsk");
  void stopRecording();

  virtual void flushTxFifo();

protected:
  void addToRecordFile(char * buffer, size_t size);

private:
  std::ofstream recordFile_;

};

#endif

