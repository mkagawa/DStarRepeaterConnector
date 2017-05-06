//
// Copyright © 2017 Masahito Kagawa <nw6up@arrl.net>
//
// this is a DVMega simulator
//
// the program provides two virtual serial ports, which is
// acting as DVMega board serial port

#include "dvmega_sim.h"
#include "WorkerThread.h"
#include "Const.h"

using namespace std;
CWorkerThread::~CWorkerThread() {
  wxLogMessage(wxT("%d Destructor for myThread"), m_fd);
}

CWorkerThread::CWorkerThread(int fd,char* devName) 
  : wxThread(wxTHREAD_JOINABLE),m_fd(fd), m_devName(devName)
{
  wxLogMessage(wxT("%d CWorkerThread contructor"), m_fd);
  wxLogMessage(wxT("%d Device: %s"), m_fd, m_devName);
}
void CWorkerThread::OnExit() {
  if(m_fd) {
    close(m_fd);
  }
  wxLogMessage(wxT("%d CWorkerThread OnExit"), m_fd);
}

CWorkerThread::ExitCode CWorkerThread::Entry() {

  int flags = ::fcntl(m_fd, F_GETFL, 0);
  ::fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
  while(!TestDestroy()){
    ProcessData();
    Sleep(1000*1);
  }
  wxLogMessage(wxT("%d Exit from entry"), m_fd);
  return static_cast<ExitCode>(0);
}

void CWorkerThread::ProcessData() {
  wxLogMessage(wxT("%d ProcessData"), m_fd);

  size_t len = 0; 
  while(m_buffer[0]!=DVRPTR_FRAME_START) {
    len = ::read(m_fd, m_buffer, 1);
    if(len != 1) {
      wxLogMessage(wxT("result empty"));
      return;
    }
    wxLogMessage(wxT("buff = %X"), m_buffer[0]);
  }
  len = ::read(m_fd, m_buffer, 3);
  size_t size = m_buffer[0] + m_buffer[1] * 256;
  int cmd = m_buffer[2];
  int pos = 0;
  switch(cmd) {
    case DVRPTR_GET_STATUS :
      if( size == 1 ) {
        m_txEnabled = true; //(m_buffer[4U] & 0x02U) == 0x02U;
        m_checksum  = false; //(m_buffer[4U] & 0x08U) == 0x08U;
        m_tx        = false; //(m_buffer[5U] & 0x02U) == 0x02U;
        m_txSpace   = 100; //m_buffer[8U];
        space       = m_txSpace - m_buffer[9U];

        m_wbuffer[pos++] = DVRPTR_FRAME_START;
        m_wbuffer[pos++] = 9;
        m_wbuffer[pos++] = 0;
        m_wbuffer[pos++] = DVRPTR_GET_STATUS;
        m_wbuffer[pos++] = 0x02;
        m_wbuffer[pos++] = 0; 
        m_wbuffer[pos++] = 0;
        m_wbuffer[pos++] = 0;
        m_wbuffer[pos++] = 100;
        m_wbuffer[pos++] = 10;
        m_wbuffer[pos++] = 0x00; //cs
        m_wbuffer[pos++] = 0x0b; //cs
        ::write(m_fd, m_wbuffer, pos);
        wxLogMessage(wxT("reponsed to DVRPTR_GET_STATUS"));
      } else {
        wxLogMessage(wxT("length was incorrect DVRPTR_GET_STATUS: %d"), size);
      }
      break;
    case DVRPTR_GET_VERSION:
      if( size == 1 ) {
        ::read(m_fd, m_buffer, 2); //check sum

        m_wbuffer[pos++] = DVRPTR_FRAME_START;
        m_wbuffer[pos++] = 6;//??
        m_wbuffer[pos++] = 0;
        m_wbuffer[pos++] = DVRPTR_GET_VERSION;
        m_wbuffer[pos++] = 1;
        m_wbuffer[pos++] = 1; 
        m_wbuffer[pos++] = 1;
        m_wbuffer[pos++] = 0x00;
        m_wbuffer[pos++] = 0x0b;
        ::write(m_fd, m_wbuffer, pos);
        wxLogMessage(wxT("reponsed to DVRPTR_GET_VERSION"));
      } else {
        wxLogMessage(wxT("length was incorrect for DVRPTR_GET_VERSION: %d"), size);
      }
      break;
    case DVRPTR_GET_SERIAL :
      break;
    case DVRPTR_GET_CONFIG :
      break;
    case DVRPTR_SET_CONFIG :
      break;
    case DVRPTR_RXPREAMBLE :
      break;
    case DVRPTR_START      :
      break;
    case DVRPTR_HEADER     :
      break;
    case DVRPTR_RXSYNC     :
      break;
    case DVRPTR_DATA       :
      break;
    case DVRPTR_EOT        :
      break;
    case DVRPTR_RXLOST     :
      break;
    case DVRPTR_MSG_RSVD1  :
      break;
    case DVRPTR_MSG_RSVD2  :
      break;
    case DVRPTR_MSG_RSVD3  :
      break;
    case DVRPTR_SET_TESTMDE:
      break;
    default:
      wxLogMessage(wxT("Unknown cmd %X"), cmd);
      break;
  }
}

