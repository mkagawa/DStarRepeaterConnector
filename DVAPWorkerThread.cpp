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
//
// this is a DVAP simulator
//
// the program provides two virtual serial ports, which is
// acting as DVAP board serial port

#include "RepeaterConnector.h"
#include "DVAPWorkerThread.h"
#include "Const.h"

CDVAPWorkerThread::~CDVAPWorkerThread()
{
}

CDVAPWorkerThread::CDVAPWorkerThread(char siteId, unsigned int portNumber,wxString confDir,wxString logDir)
 :CBaseWorkerThread(siteId, portNumber, confDir, logDir),
  m_bStarted(false),
  m_lastStatusSentTimeStamp(wxGetUTCTimeMillis())
{
}

int CDVAPWorkerThread::ProcessData() {
  CTxData *pBuf;
  bool bClosingPacket = false;
  if(m_SendingQueue.ReceiveTimeout(0, pBuf) != wxMSGQUEUE_TIMEOUT) {
    bClosingPacket = pBuf->IsClosingPacket();
    auto data = pBuf->GetData();
    auto data_len = pBuf->GetDataLen();
    m_lastTxPacketTimeStamp = wxGetUTCTimeMillis();

    if(m_bStarted && !m_bTxToHost && !bClosingPacket) {
      wxLogMessage("DVAP -> Host Stream Starts");
      m_bTxToHost = true;
      m_packetCnt = 0;
    }

    //Write to the host
    if(m_bEnableForwardPackets && m_bTxToHost) {
      if(!pBuf->IsHeaderPacket()) {
        m_packetCnt++;
      }
      if(m_bEnableDumpPackets && wxLog::GetVerbose()) {
        wxString head = bClosingPacket ? wxString::Format(wxT("CLOSE:%s"),pBuf->GetCallSign()) :
                                         wxString::Format("R%4X:%s",(uint)(pBuf->GetSessionId() % 0xFFFF),pBuf->GetCallSign());
        dumper(head, data, data_len);
      }
      ::write(m_fd, data, data_len);
    }

    delete pBuf;
  }

  //watch dog for TX.
  //if current time is more than 1 sec from previous TX packet 
  //or packet type is "closing" stop sending
  if(m_bStarted && m_bTxToHost && (bClosingPacket || wxGetUTCTimeMillis() - m_lastTxPacketTimeStamp > 1000)) {
    wxLogMessage(wxT("DVAP -> Host Stream Ends (%d packets sent)"), m_packetCnt);
    m_bTxToHost = false;
    m_curCallSign.Clear();
    m_curSessionId = 0L;
  }

  //Send current status to the host in every 20ms
  if(m_bStarted && wxGetUTCTimeMillis() - m_lastStatusSentTimeStamp > 20) {
    ::memcpy(m_wbuffer,DVAP_STATUS,DVAP_STATUS_LEN);
    //[07][20] [90][00] [B5] [01][7F] //-75dBm squelch open
    //[07][20] [90][00] [9C] [00][7F] //-100dBm squelch closed
    //[07][20] [90][00] [00] [00][12] //Transmitting Tx fifo can except up to 18 new packets
    //7f == Indicates Queue is empty and Tx operation will soon terminate.
    m_wbuffer[4] = 0xb5;
    m_wbuffer[5] = 0x01;
    m_wbuffer[6] = 0x7f;
    ::write(m_fd, m_wbuffer, DVAP_STATUS_LEN);
    m_lastStatusSentTimeStamp = wxGetUTCTimeMillis();
  }

  //Read from the host
  size_t len = ::read(m_fd, m_buffer, DVAP_HEADER_LENGTH);
  if(len == -1) {
    if(errno == EAGAIN) {
      if(m_bStarted && wxGetUTCTimeMillis() - m_lastAckTimeStamp > 3000) {
        m_bTxToHost = false;
        m_bStarted = false;
        wxLogMessage("No ack from the host. may be repeater process stopped.");
        return -1;
      }

      //serial buffer empty
      if(!m_bStarted) {
        return 0;
      }
      return 0;
    }
    wxLogError(wxT("serial read error, err: %d"), errno);
    return 0; //error
  }

  //data must be more than 2 bytes
  if(len < 2) {
    return -1;
  }

  //calculate data length
  size_t data_len = m_buffer[0] + 256 * (m_buffer[1] & 0x1F);
  char type = (m_buffer[1] & 0xe0) >> 5;
  m_lastReceivedFromHostTimeStamp = wxGetUTCTimeMillis();

  //the data_len should not be longer than DVAP_HEADER_LEN
  //if so, data may be garbled. ignore this series
  if(data_len > sizeof(m_buffer)) {
    return -1;
  }

  while(data_len > len) {
    //all packets must be received within 500ms from the host
    //otherwise discard this series
    if(wxGetUTCTimeMillis() - m_lastReceivedFromHostTimeStamp > 500) {
      wxLogMessage("Host data timeout");
      return -1;
    }
    size_t temp_len = ::read(m_fd, &m_buffer[len], data_len - len);
    if(temp_len > 0) {
      len += temp_len;
      m_lastReceivedFromHostTimeStamp = wxGetUTCTimeMillis();
    }
  }

  if(::memcmp(m_buffer,DVAP_GMSK_DATA,2) == 0) {
    int diff = (uint)m_buffer[5] - (uint)m_packetSerialNo;
    if(m_curSessionId != 0 && (diff == 1 || diff == -255)) {
      m_packetSerialNo = m_buffer[5];

      //Detect closing packet pattern
      bool bClosingPacket = false;
      if(data_len > 12 && ::memcmp(DVAP_GMSK_DATA,m_buffer,2) == 0 && ::memcmp(GMSK_END, &m_buffer[6], 6) == 0) {
        bClosingPacket = true; 
      }
      SendToInstance(m_buffer, len, bClosingPacket ? packetType::CLOSING : packetType::NONE);
      if(diff > 1) {
        wxLogMessage(wxT("Serial diff: %d"), diff);
      }
    }
    return 1;

  } else if(::memcmp(m_buffer,DVAP_HEADER,2)==0) {
    ::memcpy(m_wbuffer, m_buffer, DVAP_HEADER_LEN);
    //CalcCRC(&m_wbuffer[6], DVAP_HEADER_LEN-6);
    //CalcCRC(&m_wbuffer[6], DVAP_HEADER_LEN-6);

    //For logging purpose
    wxString cs,r1,r2,my,sx;
    char buffer[9];
    buffer[8] = 0;
    ::memcpy(buffer, &m_buffer[9],  8); r2 = wxString::FromAscii(buffer);
    ::memcpy(buffer, &m_buffer[17], 8); r1 = wxString::FromAscii(buffer);
    ::memcpy(buffer, &m_buffer[25], 8); cs = wxString::FromAscii(buffer);
    ::memcpy(buffer, &m_buffer[33], 8); my = wxString::FromAscii(buffer);
    ::memcpy(buffer, &m_buffer[41], 4); buffer[4] = 0; sx = wxString::FromAscii(buffer);
    wxLogInfo(wxT("Headr: to: %s, r2: %s, r1: %s, my: %s/%s"), cs, r2, r1, my, sx);

    //Store my local dstar repeater info
    m_curCallSign = my;
    m_curSuffix= sx;
    m_curSessionId = (ulong)rand();
    if(m_curSessionId==0) {
      m_curSessionId = (ulong)rand();
    }
    m_packetSerialNo = 0;

    //write back to the host
    m_wbuffer[1] = 0x60;
    ::write(m_fd, m_wbuffer, DVAP_RESP_HEADER_LEN);

    //check if sender callsign is not myNode call sign
    if(m_curCallSign.StartsWith(" ") || ::memcmp(m_myNodeCallSign.c_str(),m_curCallSign.c_str(),7)==0) {
      wxLogMessage("this message is sent by repeater. won't be forwarded");
      m_curSessionId = 0;
      return 1;
    }
    if(cs.EndsWith("L") || cs.EndsWith("U")) {
      wxLogMessage("this message is repeater command. ignoring.");
      m_curSessionId = 0;
      return 1;
    }

    //restore the seoncd byte
    m_wbuffer[1] = 0xa0;
    //empty G1/G2 value, and force CQCQCQ to To field
    ::memcpy(&m_wbuffer[25], "CQCQCQ  ", 8);
    ::memcpy(&m_wbuffer[9],  "                ", 16);
    CalcCRC(&m_wbuffer[6], DVAP_HEADER_LEN-6);

    SendToInstance(m_wbuffer, DVAP_HEADER_LEN, packetType::HEADER);
    return 1;

  } else if(::memcmp(m_buffer,DVAP_ACK,DVAP_ACK_LEN)==0) {
    wxLogInfo(wxT("DVAP_ACK"));
    m_lastAckTimeStamp = wxGetUTCTimeMillis();
    return 1; 

  } else if(::memcmp(m_buffer,DVAP_REQ_NAME,DVAP_REQ_NAME_LEN)==0) {
    //treat as reset signal
    wxLogInfo(wxT("DVAP_REQ_NAME"));
    ::memcpy(m_wbuffer,DVAP_RESP_NAME,DVAP_RESP_NAME_LEN);
    ::write(m_fd, m_wbuffer, DVAP_RESP_NAME_LEN);
    m_bTxToHost = false;
    m_bStarted = false;
    return 1;
 
  } else if(::memcmp(m_buffer,DVAP_REQ_SERIAL,DVAP_REQ_SERIAL_LEN)==0) {
    wxLogInfo(wxT("DVAP_REQ_SERIAL"));
    ::memcpy(m_wbuffer,DVAP_RESP_SERIAL,DVAP_RESP_SERIAL_LEN);
    ::memcpy(&m_wbuffer[4], "MT123456", 9); //including 0x00
    ::write(m_fd, m_wbuffer, DVAP_RESP_SERIAL_LEN+8);
    return 1;

  } else if(::memcmp(m_buffer,DVAP_REQ_FIRMWARE,DVAP_REQ_FIRMWARE_LEN)==0) {
    wxLogInfo(wxT("DVAP_REQ_FIRMWARE"));
    ::memcpy(m_wbuffer,DVAP_RESP_FIRMWARE,DVAP_RESP_FIRMWARE_LEN);
    ::write(m_fd, m_wbuffer, DVAP_RESP_FIRMWARE_LEN);
    return 1;

  } else if(::memcmp(m_buffer,DVAP_REQ_MODULATION,DVAP_REQ_MODULATION_LEN)==0) {
    wxLogInfo(wxT("DVAP_REQ_MODULATION"));
    ::memcpy(m_wbuffer,DVAP_RESP_MODULATION,DVAP_RESP_MODULATION_LEN);
    ::write(m_fd, m_wbuffer, DVAP_RESP_MODULATION_LEN);
    return 1;

  } else if(::memcmp(m_buffer,DVAP_REQ_MODE,DVAP_REQ_MODE_LEN)==0) {
    wxLogInfo(wxT("DVAP_REQ_MODE"));
    ::memcpy(m_wbuffer,DVAP_RESP_MODE,DVAP_RESP_MODE_LEN);
    ::write(m_fd, m_wbuffer, DVAP_RESP_MODE_LEN);
    return 1;

  } else if(::memcmp(m_buffer,DVAP_REQ_POWER,DVAP_REQ_POWER_LEN-2)==0) {
    wxLogInfo(wxT("DVAP_REQ_POWER"));
    ::memcpy(m_wbuffer,DVAP_RESP_POWER,DVAP_RESP_POWER_LEN);
    ::write(m_fd, m_wbuffer, DVAP_RESP_POWER_LEN);
    return 1;

  } else if(::memcmp(m_buffer,DVAP_REQ_SQUELCH,DVAP_REQ_SQUELCH_LEN-2)==0) {
    wxLogInfo(wxT("DVAP_REQ_SQUELCH"));
    ::memcpy(m_wbuffer,DVAP_RESP_SQUELCH,DVAP_RESP_SQUELCH_LEN);
    ::write(m_fd, m_wbuffer, DVAP_RESP_SQUELCH_LEN);
    return 1;

  } else if(::memcmp(m_buffer,DVAP_REQ_FREQUENCY,DVAP_REQ_FREQUENCY_LEN-4)==0) {
    wxLogInfo(wxT("DVAP_REQ_FREQUENCY"));
    ::memcpy(m_wbuffer,DVAP_RESP_FREQUENCY,DVAP_RESP_FREQUENCY_LEN);
    ::write(m_fd, m_wbuffer, DVAP_RESP_FREQUENCY_LEN);
    return 1;

  } else if(::memcmp(m_buffer,DVAP_REQ_START,DVAP_REQ_START_LEN)==0) {
    wxLogInfo(wxT("DVAP_REQ_START"));
    ::memcpy(m_wbuffer,DVAP_RESP_START,DVAP_RESP_START_LEN);
    ::write(m_fd, m_wbuffer, DVAP_RESP_START_LEN);
    m_lastAckTimeStamp = wxGetUTCTimeMillis();
    m_bStarted = true;
    m_bTxToHost = false;
    return 1;

  } else if(::memcmp(m_buffer,DVAP_REQ_STOP,DVAP_REQ_STOP_LEN)==0) {
    wxLogInfo(wxT("DVAP_REQ_STOP"));
    ::memcpy(m_wbuffer,DVAP_RESP_STOP,DVAP_RESP_STOP_LEN);
    ::write(m_fd, m_wbuffer, DVAP_RESP_STOP_LEN);
    m_bStarted = false;
    m_bTxToHost = false;
    return 1;

  } else if(::memcmp(m_buffer,DVAP_REQ_FREQLIMITS,DVAP_REQ_FREQLIMITS_LEN)==0) {
    wxLogInfo(wxT("DVAP_REQ_FREQLIMITS"));
    ::memcpy(m_wbuffer,DVAP_RESP_FREQLIMITS,DVAP_RESP_FREQLIMITS_LEN);
    long low = 144000000;
    long high = 145999999;
    ::memcpy(&m_wbuffer[4], &low, 4);
    ::memcpy(&m_wbuffer[8], &high, 4);
    ::write(m_fd, m_wbuffer, DVAP_RESP_FREQLIMITS_LEN+8);
    return 1;


  } else {
    dumper("Other", m_buffer, len);
    return 1;

  }
}
