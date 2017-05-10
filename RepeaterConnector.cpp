/*
 *   Masahito Kagawa <mkagawa@hotmail.com> NW6UP
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "RepeaterConnector.h"
#include "BaseWorkerThread.h"
#include "Const.h"
#include <signal.h>

wxIMPLEMENT_APP(CRepeaterConnectorApp);

int CRepeaterConnectorApp::OnExit() {
  wxLogMessage(wxT("Request for Exit"));

  size_t len = m_threads.GetCount();
  for(int i = len - 1; i >= 0; i-- ) {
    wxThread *p = m_threads[i];
    p->Delete();
    while(p->IsRunning()) {
      wxThread::This()->Sleep(100);
    }
    delete p;
    m_threads.RemoveAt(i);
  }
  return wxAppConsole::OnExit();
}

void CRepeaterConnectorApp::OnInitCmdLine(wxCmdLineParser &parser) {
  parser.AddSwitch(wxT("nolog"), wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
  parser.AddSwitch(wxT("g"),       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
  wxAppConsole::OnInitCmdLine(parser);
}	

//Signal handler
void CRepeaterConnectorApp::OnSignal(int num) {
  wxGetApp().ExitMainLoop();
}

//Initialization of the program
bool CRepeaterConnectorApp::OnInit() {

  signal(SIGTERM, CRepeaterConnectorApp::OnSignal);
  signal(SIGINT, CRepeaterConnectorApp::OnSignal);

  if (!wxApp::OnInit()) {
    return false;
  }

  wxLog* logger = new wxLogStream(&std::cout);
  logger->SetFormatter(new MyLogFormatter);
  wxLog::SetActiveTarget(logger);
  wxLogMessage(wxT("M OnInit"));

  //make two instances of worker thread
  for(int i = 0; i < 2; i++ ) {
    CBaseWorkerThread *pThread = CBaseWorkerThread::CreateInstance(InstType::DVAP);
    wxThreadError e = pThread->Create();
    if(e != wxThreadError::wxTHREAD_NO_ERROR) {
      delete pThread;
      pThread = NULL;
      wxLogMessage(wxT("Thread create err: %d"), e);
      return false; 
    }
    pThread->SetPriority(100);
    m_threads.Add(pThread);
  }

  //register other instance each oher
  for(int i = 0; i < m_threads.GetCount(); i++ ) {
    for(int j = 0; j < m_threads.GetCount(); j++ ) {
      if(m_threads[i] != m_threads[j]) {
        m_threads[i]->RegisterOtherInstance(m_threads[j]);
      }
    }
    m_threads[i]->Run();
  }

  wxLogMessage(wxT("M OnInit done"));
  return true;
}
