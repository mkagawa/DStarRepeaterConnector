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

#ifndef __CONST__
#define __CONST__

const unsigned char SW_VERSION[] = wxT("0.5.0");
const unsigned char SW_VENDOR[] = wxT("NW6UP");

#define MAX_MODULES 2

//----------DVMega---------

const unsigned char DVRPTR_FRAME_START = 0xD0U;

const unsigned char DVRPTR_GET_STATUS  = 0x10U;
const unsigned char DVRPTR_GET_VERSION = 0x11U;
const unsigned char DVRPTR_GET_SERIAL  = 0x12U;
const unsigned char DVRPTR_GET_CONFIG  = 0x13U;
const unsigned char DVRPTR_SET_CONFIG  = 0x14U;
const unsigned char DVRPTR_RXPREAMBLE  = 0x15U;
const unsigned char DVRPTR_START       = 0x16U;
const unsigned char DVRPTR_HEADER      = 0x17U;
const unsigned char DVRPTR_RXSYNC      = 0x18U;
const unsigned char DVRPTR_DATA        = 0x19U;
const unsigned char DVRPTR_EOT         = 0x1AU;
const unsigned char DVRPTR_RXLOST      = 0x1BU;
const unsigned char DVRPTR_MSG_RSVD1   = 0x1CU;
const unsigned char DVRPTR_MSG_RSVD2   = 0x1DU;
const unsigned char DVRPTR_MSG_RSVD3   = 0x1EU;
const unsigned char DVRPTR_SET_TESTMDE = 0x1FU;

const unsigned char DVRPTR_ACK = 0x06U;
const unsigned char DVRPTR_NAK = 0x15U;

const unsigned int MAX_RESPONSES = 30U;

const unsigned int BUFFER_LENGTH = 200U;

const unsigned char DVRPTR_RESPONSE_BIT = 0x80U;

const unsigned char _DVMEGA_VERSION_STR[] = "\001 DV-MEGA R2.00A \000";



//----------DVAP---------
const unsigned char DVAP_REQ_NAME[] = {0x04, 0x20, 0x01, 0x00};
const unsigned int  DVAP_REQ_NAME_LEN = 4U;

const unsigned char DVAP_RESP_NAME[] = {0x10, 0x00, 0x01, 0x00, 'D', 'V', 'A', 'P', ' ', 'D', 'o', 'n', 'g', 'l', 'e', 0x00};
const unsigned int  DVAP_RESP_NAME_LEN = 16U;

const unsigned char DVAP_REQ_SERIAL[] = {0x04, 0x20, 0x02, 0x00};
const unsigned int  DVAP_REQ_SERIAL_LEN = 4U;

const unsigned char DVAP_RESP_SERIAL[] = {0x0C, 0x00, 0x02, 0x00};
const unsigned int  DVAP_RESP_SERIAL_LEN = 4U;

const unsigned char DVAP_REQ_FIRMWARE[] = {0x05, 0x20, 0x04, 0x00, 0x01};
const unsigned int  DVAP_REQ_FIRMWARE_LEN = 5U;

const unsigned char DVAP_RESP_FIRMWARE[] = {0x07, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00};
const unsigned int  DVAP_RESP_FIRMWARE_LEN = 7U;

const unsigned char DVAP_REQ_MODULATION[] = {0x05, 0x00, 0x28, 0x00, 0x01};
const unsigned int  DVAP_REQ_MODULATION_LEN = 5U;

const unsigned char DVAP_RESP_MODULATION[] = {0x05, 0x00, 0x28, 0x00, 0x01};
const unsigned int  DVAP_RESP_MODULATION_LEN = 5U;

const unsigned char DVAP_REQ_MODE[] = {0x05, 0x00, 0x2A, 0x00, 0x00};
const unsigned int  DVAP_REQ_MODE_LEN = 5U;

const unsigned char DVAP_RESP_MODE[] = {0x05, 0x00, 0x2A, 0x00, 0x00};
const unsigned int  DVAP_RESP_MODE_LEN = 5U;

const unsigned char DVAP_REQ_SQUELCH[] = {0x05, 0x00, 0x80, 0x00, 0x00};
const unsigned int  DVAP_REQ_SQUELCH_LEN = 5U;

const unsigned char DVAP_RESP_SQUELCH[] = {0x05, 0x00, 0x80, 0x00, 0x00};
const unsigned int  DVAP_RESP_SQUELCH_LEN = 5U;

const unsigned char DVAP_REQ_POWER[] = {0x06, 0x00, 0x38, 0x01, 0x00, 0x00};
const unsigned int  DVAP_REQ_POWER_LEN = 6U;

const unsigned char DVAP_RESP_POWER[] = {0x06, 0x00, 0x38, 0x01, 0x00, 0x00};
const unsigned int  DVAP_RESP_POWER_LEN = 6U;

const unsigned char DVAP_REQ_FREQUENCY[] = {0x08, 0x00, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_REQ_FREQUENCY_LEN = 8U;

const unsigned char DVAP_RESP_FREQUENCY[] = {0x08, 0x00, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_RESP_FREQUENCY_LEN = 8U;

const unsigned char DVAP_REQ_FREQLIMITS[] = {0x04, 0x20, 0x30, 0x02};
const unsigned int  DVAP_REQ_FREQLIMITS_LEN = 4U;

const unsigned char DVAP_RESP_FREQLIMITS[] = {0x0C, 0x00, 0x30, 0x02};
const unsigned int  DVAP_RESP_FREQLIMITS_LEN = 4U;

const unsigned char DVAP_REQ_START[] = {0x05, 0x00, 0x18, 0x00, 0x01};
const unsigned int  DVAP_REQ_START_LEN = 5U;

const unsigned char DVAP_RESP_START[] = {0x05, 0x00, 0x18, 0x00, 0x01};
const unsigned int  DVAP_RESP_START_LEN = 5U;

const unsigned char DVAP_REQ_STOP[] = {0x05, 0x00, 0x18, 0x00, 0x00};
const unsigned int  DVAP_REQ_STOP_LEN = 5U;

const unsigned char DVAP_RESP_STOP[] = {0x05, 0x00, 0x18, 0x00, 0x00};
const unsigned int  DVAP_RESP_STOP_LEN = 5U;

const unsigned char DVAP_HEADER[] = {0x2F, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_HEADER_LEN = 47U;

const unsigned char DVAP_RESP_HEADER[] = {0x2F, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_RESP_HEADER_LEN = 47U;

//This message is sent whenever there is a change in the PTT state of the DVAP as an unsolicited type
//msg (does not need to be requested)
const unsigned char DVAP_RESP_PTT[] = {0x05, 0x20, 0x18, 0x01, 0x00};
const unsigned int  DVAP_RESP_PTT_LEN = 5U;

const unsigned char DVAP_GMSK_DATA[] = {0x12, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_GMSK_DATA_LEN = 18U;

const unsigned char DVAP_STATUS[] = {0x07, 0x20, 0x90, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_STATUS_LEN = 7U;

const unsigned char DVAP_ACK[] = {0x03, 0x60, 0x00};
const unsigned int  DVAP_ACK_LEN = 3U;

const unsigned char DVAP_FM_DATA[] = {0x42, 0x81};
const unsigned int  DVAP_FM_DATA_LEN = 2U;

const unsigned int DVAP_HEADER_LENGTH = 2U;

const unsigned int DVAP_MAX_RESPONSES = 20U;

const unsigned int DVAP_BUFFER_LENGTH = 200U;

const unsigned int DVAP_DUMP_LENGTH = 30U;


//Start from buff[6]
const unsigned char GMSK_END[] = { 0x55, 0x55, 0x55, 0x55, 0xC8, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 

#endif
