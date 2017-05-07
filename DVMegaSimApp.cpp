/*
   Masahito Kagawa <mkagawa@hotmail.com> NW6UP

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.


*/
#include "DVMegaSimApp.h"
#include "WorkerThread.h"
#include "Const.h"

#include <signal.h>


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
  return wxAppConsole::OnExit();
}

void MyApp::OnInitCmdLine(wxCmdLineParser &parser) {
  parser.AddSwitch(wxT("nolog"), wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
  parser.AddSwitch(wxT("g"),       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
  wxAppConsole::OnInitCmdLine(parser);
}	

//Signal handler
void MyApp::OnSignal(int num) {
  wxGetApp().ExitMainLoop();
}

//Initialization of the program
bool MyApp::OnInit() {

  signal(SIGTERM, MyApp::OnSignal);
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

