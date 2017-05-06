#include "DVMegaSimApp.h"
#include "WorkerThread.h"
#include "Const.h"

#include <signal.h>


void MyApp::OnSignal(int num) {
  wxGetApp().ExitMainLoop();
}


//----------------------------
wxIMPLEMENT_APP(MyApp);

int MyApp::OnExit() {
  wxLogMessage(wxT("Request for Exit"));

  wxArrayThread arr = wxGetApp().m_threads;
  size_t len = arr.GetCount();
  for(int i = len - 1; i >= 0; i-- ) {
    wxThread *p = arr[i];
    p->Delete();
    while(p->IsRunning()) {
      wxThread::This()->Sleep(100);
    }
    arr.RemoveAt(i);
  }
  wxLogMessage(wxT("OnExit done"));
  return wxAppConsole::OnExit();
}

void MyApp::OnInitCmdLine(wxCmdLineParser &parser) {
  parser.AddSwitch(wxT("nolog"), wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
  parser.AddSwitch(wxT("g"),       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
  wxAppConsole::OnInitCmdLine(parser);
}	


bool MyApp::OnInit() {

  signal(SIGINT, MyApp::OnSignal);

  if (!wxApp::OnInit()) {
    return false;
  }

  wxLog* logger = new wxLogStream(&std::cout);
  wxLog::SetActiveTarget(logger);
  wxLogMessage(wxT("M OnInit"));

  for(int i = 0; i < 2; i++ ) {
    CWorkerThread *pThread = new CWorkerThread();
    wxThreadError e = pThread->Create();
    if(e != wxThreadError::wxTHREAD_NO_ERROR) {
      delete pThread;
      pThread = NULL;
      wxLogMessage(wxT("Thread create err: %d"), e);
      return false; 
    }
    pThread->SetPriority(100);
    m_threads.Add(pThread);
    pThread->Run();
  }

  wxLogMessage(wxT("M OnInit done"));
  return true;
}

