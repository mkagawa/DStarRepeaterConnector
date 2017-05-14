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

#include "TxData.h"

CTxData::CTxData(unsigned char* data, size_t data_len, wxString cs, ulong sessionId, packetType ptype)
      : m_myCallSign(cs),
        m_sessionId(sessionId),
        m_packetType(ptype) {
  m_buffer.Clear();
  m_buffer.AppendData(data, data_len);
  m_myCallSign = cs;
}

CTxData::~CTxData() {
  m_buffer.Clear();
}
