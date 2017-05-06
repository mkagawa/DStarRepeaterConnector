#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include <wx/wxprec.h>
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

WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);
WX_DEFINE_ARRAY_INT(int, wxFdArray);

class MyApp : public wxApp {
public:
   virtual bool OnInit();
   virtual void OnInitCmdLine(wxCmdLineParser &);


   virtual int OnExit();

   wxArrayThread m_threads;
   wxArrayInt m_masterFd;
   wxArrayInt m_slaveFd;
   wxCriticalSection m_pThreadCS;    // protects the m_pThread pointer
private:
   static void OnSignal(int sig);
};

wxDECLARE_APP(MyApp);
