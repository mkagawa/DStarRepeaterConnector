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
#include "DVAPWorkerThread.h"
#include "DVMegaWorkerThread.h"
#include "Const.h"

using namespace std;
class CDVAPWorkerThread;

CBaseWorkerThread* CBaseWorkerThread::CreateInstance(InstType type) {
  switch(type) {
    case InstType::DVAP:
    return new CDVAPWorkerThread;
    case InstType::DVMega:
    return new CDVMegaWorkerThread;
  }
}

CBaseWorkerThread::CBaseWorkerThread(InstType type)
  : wxThread(wxTHREAD_JOINABLE), m_type(type)
{
  char devname[50];
  if(::openpty(&m_fd, &m_slavefd, devname, NULL, NULL)== -1) {
    wxLogMessage(wxT("Failed to open virtual port, errno:%d"), errno);
    return;
  }
  m_devName = devname;
  wxLogMessage(wxT("%d Device has created: %s"), m_fd, m_devName);
}

CBaseWorkerThread::~CBaseWorkerThread() {
  wxLogMessage(wxT("%d Destructor for myThread"), m_fd);
}

int CBaseWorkerThread::ProcessData() {
}

void CBaseWorkerThread::OnExit() {
  wxLogMessage(wxT("CBaseWorkerThread::OnExit"));
  if(m_fd) {
    ::close(m_fd);
  }
  if(m_slavefd) {
    ::close(m_slavefd);
  }
}

CBaseWorkerThread::ExitCode CBaseWorkerThread::Entry() {
  wxLogMessage(wxT("%d CBaseWorkerThread started"), m_fd);

  int flags = ::fcntl(m_fd, F_GETFL, 0);
  ::fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
  while(!TestDestroy()){
    if(ProcessData()==0) {
      Sleep(50);
    }
  }

  return static_cast<ExitCode>(0);
}

void CBaseWorkerThread::RegisterOtherInstance(CBaseWorkerThread *ptr) {
  m_threads.Add(ptr);
}

void CBaseWorkerThread::SendToInstance(unsigned char* data, int len) {
  for(int i = 0; i < m_threads.GetCount(); i++) {
    wxMemoryBuffer *buf = new wxMemoryBuffer();
    buf->AppendData(m_buffer, len);
    ((CBaseWorkerThread*)m_threads[i])->m_SendingQueue.Post(buf);
  }
}
