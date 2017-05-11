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
#ifndef _CBaseWorkerThread_
#define _CBaseWorkerThread_

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
#include <wx/msgqueue.h> 
#include <wx/buffer.h> 
#include <wx/time.h> 

#include "Const.h"

#include <pty.h>

enum InstType {
  DVMega = 1,
  DVAP = 2
};

WX_DEFINE_ARRAY_PTR(wxThread*, wxArrayThread);
class CTxData {
  private:
    wxMemoryBuffer m_buffer;
    wxString m_myCallSign;
  public:
    wxString GetCallSign() { return m_myCallSign; }
    unsigned char* GetData() { return (unsigned char*)m_buffer.GetData(); }
    size_t GetDataLen() { return m_buffer.GetDataLen(); }
    CTxData(unsigned char* data, size_t data_len, wxString cs) {
      m_buffer.Clear();
      m_buffer.AppendData(data, data_len);
      m_myCallSign = cs; 
    }
    ~CTxData() {
      m_buffer.Clear();
    }
};

class CBaseWorkerThread : public wxThread {
  public:
    CBaseWorkerThread(char);
    virtual ~CBaseWorkerThread() = 0;
    static CBaseWorkerThread* CreateInstance(InstType, char siteId);
    void RegisterOtherInstance(CBaseWorkerThread* ptr);

  private:
    InstType m_type;
    int m_slavefd;
    wxArrayThread m_threads;
    char m_siteId;  //one byte of identifier for this thread

  protected: 
    void SendToInstance(unsigned char* data, size_t len);
    // thread execution starts here
    virtual void *Entry();
    virtual int ProcessData() = 0;
    virtual void OnExit();

    wxString m_devName;
    int m_fd = -1;
    unsigned char m_buffer[DVAP_BUFFER_LENGTH];
    unsigned char m_wbuffer[DVAP_BUFFER_LENGTH];

    wxString m_myNodeCallSign;
    wxString m_myGatewayCallSign;
    wxString m_curCallSign;

    //bool m_txEnabled, m_checksum, m_tx, m_txSpace;
    //int space;

    wxLongLong m_lastTxPacketTimeStamp;
    wxLongLong m_lastReceivedFromHostTimeStamp;
    bool m_bTx;

    wxMessageQueue<CTxData *> m_SendingQueue;

    //inticats ready to receive packet
    bool m_initialized = false;

    static void CalcCRC(unsigned char* data, int len);

    //dump m_buffer
    static void dumper(const char* head, unsigned char* buff, int len);
};

#endif
