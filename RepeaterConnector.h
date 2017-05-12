#include <iostream>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include <wx/wx.h>
#include <wx/cmdline.h>
#include <wx/regex.h>
#include <wx/thread.h>
#include <wx/memory.h> 
#include <wx/log.h> 
#include <wx/dynarray.h>
#include <wx/utils.h> 
#include <wx/dynarray.h>


#if wxUSE_GUI == 1
#include <wx/frame.h>
#endif

#include "BaseWorkerThread.h"

WX_DEFINE_ARRAY_PTR(CBaseWorkerThread*, wxArrayBaseWorkerThread);

class CRepeaterConnectorLogFormatter : public wxLogFormatter {
  public:
  virtual wxString Format(wxLogLevel level,
                          const wxString& msg,
                          const wxLogRecordInfo& info) const {
    return wxString::Format(wxT("%s [%d][%d]: %s"),
    	wxDateTime(info.timestamp).Format("%Y-%m-%d %H:%M:%S"),
	(int)level, (int)(info.threadId & 0xFFFF), msg.c_str());
  }

};

class CRepeaterConnectorApp : public wxApp {
public:
   virtual bool OnInit();
   virtual void OnInitCmdLine(wxCmdLineParser &);
   virtual bool OnCmdLineParsed(wxCmdLineParser &);
   virtual int OnExit();
   wxArrayBaseWorkerThread m_threads;
   wxArrayInt m_masterFd;
   wxArrayInt m_slaveFd;

   wxCriticalSection m_pThreadCS;    // protects the m_pThread pointer

private:
   std::fstream m_logStream;
   static void OnSignal(int sig);
   unsigned char m_module[MAX_MODULES];
   unsigned long m_portNumber[MAX_MODULES];
   wxString m_logDir;
};

wxDECLARE_APP(CRepeaterConnectorApp);
