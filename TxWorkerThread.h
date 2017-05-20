/*
 *  Copyright (C) 2017 by Masahito Kagawa NW6UP <mkagawa@hotmail.com>
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
#ifndef _CTxWorkerThread_
#define _CTxWorkerThread_

#include "TxData.h"
#include <wx/timer.h>

class CTxWorkerThread;
class CTxTicker : public wxTimer {
  private:
    char m_siteId;
    CTxWorkerThread *m_pTxThread;

  public:
    CTxTicker(char id, CTxWorkerThread * pw) : wxTimer(), m_siteId(id), m_pTxThread(pw) { }
    void Notify();
};


class CTxWorkerThread : public wxThread {
  public:
    CTxWorkerThread(int fd, char, wxMessageQueue<CTxData*> *, wxMutex*);
    virtual ~CTxWorkerThread();
    void PostData(CTxData*);
    int ProcessTxToHost();
    void Stop();

  protected: 
    // thread execution starts here
    virtual ExitCode Entry();
    virtual void OnExit();

  private:
    wxMutex *m_pMutexSerialWrite;
    bool m_bStarted;
    wxMessageQueue<CTxData*>* m_pSendingQueue;
    CTxData* m_pTxHeaderPacket;
    //CBaseWorkerThread* m_pBaseWorker;
    char m_siteId;
    int m_fd;
    ulong m_curTxSessionId;
    wxLongLong m_lastHeaderPacketTimeStamp;
    wxLongLong m_lastDataPacketTimeStamp;
    wxLongLong m_lastWriteTimeStamp;

    ulong m_iTxPacketCnt;
    wxString m_curCallSign;
    wxString m_curSuffix;
    bool m_bTxToHost;
    bool m_curWrongSessionIdNotified;
    bool m_bInvalid;
    bool m_bEnableForwardPackets;
    bool m_bEnableDumpPackets;

    bool m_bRunning = false;

public:
    void OnTimer(wxTimerEvent& event);
private:
    CTxTicker *m_ptimer;
};

#endif

