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
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/regex.h>
//#include <wx/thread.h>
#include <wx/memory.h> 
#include <wx/log.h> 

#include <pty.h>
#include "BaseWorkerThread.h"

using namespace std;
class CDVAPWorkerThread : public CBaseWorkerThread {
  public:
    CDVAPWorkerThread();
    virtual ~CDVAPWorkerThread();

  private:
    virtual int ProcessData() wxOVERRIDE;
    chrono::time_point<chrono::high_resolution_clock> m_prev_status_ts;
    bool m_bStarted;

    //dump m_buffer
    void dumper(const char* head, unsigned char* buff, int len);
};

