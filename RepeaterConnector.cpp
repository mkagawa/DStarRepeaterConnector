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



using namespace std;

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
  parser.AddSwitch(wxT("v"), wxEmptyString, wxT("show version"), wxCMD_LINE_PARAM_OPTIONAL);
  if(wxGetApp().argc > 1 && wxGetApp().argv[1] == wxT("-v")) {
    return;
  }
  parser.AddOption(wxT("callsign"), wxEmptyString, wxT("gw and repeater base callsign without suffix"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
  parser.AddOption(wxT("gwport"), wxEmptyString, wxT("gw port number (default:20010)"), wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL);
  parser.AddOption(wxT("rcfg"), wxEmptyString, wxT("base dstarrepeater config file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
  for(int i=1;i<=MAX_MODULES;i++) {
    parser.AddOption(wxString::Format(wxT("mod%d"),i), wxEmptyString,
      wxString::Format(wxT("dstarrepeater %d module letter [A-E] and port num. (ex: -mod%d:A,20011)"),i,i), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
  }
  parser.AddOption(wxT("rptcmd"), wxEmptyString, wxT("full path of dstarrepeater executable"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
  parser.AddOption(wxT("logdir"), wxEmptyString, wxT("log directory (default: current dir)"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
  wxAppConsole::OnInitCmdLine(parser);
}	

//
//Executable parameter validation
//
bool CRepeaterConnectorApp::OnCmdLineParsed(wxCmdLineParser &parser) {

  if(parser.Found(wxT("v"))) {
    cout << wxString::Format(wxT("%s - %s"), wxString(APPLICATION_NAME), wxString(SW_VERSION)) << endl;
    return false;
  }
  if(parser.Found(wxT("logdir"), &m_logDir)) {
    if(!mkdir(m_logDir, 0700)) {
      cout << "ERROR: couldn't create log dir " << m_logDir << endl;
      return false;
    }
  } else {
    m_logDir = ".";
  }
  parser.Found(wxT("callsign"), &CBaseWorkerThread::m_dstarRepeaterCallSign);
  parser.Found(wxT("rcfg"), &CBaseWorkerThread::m_dstarRepeaterConfigFile);
  if( access( CBaseWorkerThread::m_dstarRepeaterConfigFile, F_OK ) == -1 ) {
    cout << "ERROR: dstarrepeater configuration file does not exist" << endl;
    return false;
  }

  if(!parser.Found("gwport", &CBaseWorkerThread::m_dstarGatewayPort)) {
    CBaseWorkerThread::m_dstarGatewayPort = 20010;
  }

  parser.Found("rptcmd", &CBaseWorkerThread::m_dstarRepeaterExe);
  if( access( CBaseWorkerThread::m_dstarRepeaterExe, F_OK ) == -1 ) {
    cout << "ERROR: dstarrepeater executable does not exist, or no permission to access" << endl;
    return false;
  }

  wxString tmpM, tmpP;
  long l;
  for(int i=1;i<=MAX_MODULES;i++) {
     parser.Found(wxString::Format(wxT("mod%d"),i), &tmpM);
     m_module[i-1] = (tmpM.MakeUpper().c_str())[0];
     tmpM.Mid(2).ToLong(&l);
     m_portNumber[i-1] = l;
     if(m_module[i-1] < 'A' || m_module[i-1] > 'E') {
       cout << wxString::Format(wxT("ERROR: range of mod%d must be A to E"), i) << endl;
       return false;
     }
     if(l < 20000U) {
       cout << wxString::Format(wxT("ERROR: port number %d for mod%d is incorrect (ex. -mod%d A,20011)"), l, i, i) << endl;
       return false;
     }
  }

  parser.Found("rptcmd", &CBaseWorkerThread::m_dstarRepeaterExe);
  if(m_module[0]==m_module[1] || 
     m_module[1]==m_module[2] ||
     m_module[2]==m_module[0]) {
    cout << "ERROR: module ids (mod1,mod2) must be unique" << endl;
    return false;
  }

  return wxAppConsole::OnCmdLineParsed(parser);
}

//Signal handler
void CRepeaterConnectorApp::OnSignal(int num) {
  wxGetApp().ExitMainLoop();
}

//Initialization of the program
bool CRepeaterConnectorApp::OnInit() {
  //Signal Handlers (SIGINT and SIGKILL)
  signal(SIGTERM, CRepeaterConnectorApp::OnSignal);
  signal(SIGINT, CRepeaterConnectorApp::OnSignal);

  if (!wxApp::OnInit()) {
    return false;
  }

  //Logging
  auto logFileName = wxString::Format("%s/repeaterconnector.log", m_logDir);
  m_logStream.open(logFileName, fstream::out | fstream::app);
  auto logger = new wxLogStream(&m_logStream);
  logger->SetFormatter(new CRepeaterConnectorLogFormatter);
  wxLog::SetLogLevel(wxLOG_Max);
  wxLog::SetActiveTarget(logger);
  wxLog::EnableLogging();

  wxLogMessage(wxT("--------------------"));
  wxLogMessage(wxT("Starting %s - %s"), wxString(APPLICATION_NAME), wxString(SW_VERSION));

  //make two instances of worker thread
  int i;
  try {
    for(i = 0; i < MAX_MODULES; i++) {
      auto pThread = CBaseWorkerThread::CreateInstance(InstType::DVAP, m_module[i], m_portNumber[i]);
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
  } catch(MyException* ex) {
    cout << wxString::Format(wxT("ERROR: Failed to initialize worker thread %c, ex: %s"), m_module[i], ex->GetMessage()) << endl;
    delete ex;
    return false;
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

  wxLogInfo(wxT("CRepeaterConnectorApp::OnInit done"));
  return true;
}
