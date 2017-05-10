#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/regex.h>
//#include <wx/thread.h>
#include <wx/memory.h> 
#include <wx/log.h> 

#include <pty.h>
#include "BaseWorkerThread.h"

class CDVMegaWorkerThread : public CBaseWorkerThread {
  public:
    CDVMegaWorkerThread(char siteId);
    ~CDVMegaWorkerThread();

  private:
    int ProcessData() wxOVERRIDE;
};

