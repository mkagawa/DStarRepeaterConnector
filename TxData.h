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

#include <wx/wx.h>
#include <wx/regex.h>
#include <wx/fileconf.h>
#include <wx/memory.h> 
#include <wx/log.h>
#include <wx/msgqueue.h> 
#include <wx/buffer.h> 
#include <wx/time.h> 

//
//Data structure used to exchange GMSK data between threads
//
class CTxData {
  private:
    wxMemoryBuffer m_buffer;
    wxString m_myCallSign;
    ulong m_sessionId;
    wxLongLong m_lastPacketTimeStamp;
    bool m_bClosingPacket;
  public:
    bool IsClosingPacket() { return m_bClosingPacket; }
    ulong GetSessionId() { return m_sessionId; }
    wxString GetCallSign() { return m_myCallSign; }
    unsigned char* GetData() { return (unsigned char*)m_buffer.GetData(); }
    size_t GetDataLen() { return m_buffer.GetDataLen(); }
    CTxData(unsigned char* data, size_t data_len, wxString cs, ulong sessionId, bool);
    ~CTxData();
};

