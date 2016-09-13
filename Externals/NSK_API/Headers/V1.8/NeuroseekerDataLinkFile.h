#ifndef NeuroseekerDataLinkFile_h_
#define NeuroseekerDataLinkFile_h_

#include "dll_import_export.h"
#include "NeuroseekerDataLinkIntf.h"

class DLL_IMPORT_EXPORT NeuroseekerDataLinkFile : public NeuroseekerDataLinkIntf
{
public:
  NeuroseekerDataLinkFile(const std::string & playbackfilename);
  ~NeuroseekerDataLinkFile();

  bool readn(char * buffer, size_t size);

private:
  std::ifstream playbackFile_;
};

#endif
