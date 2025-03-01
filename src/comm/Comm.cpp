/**
* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
* 
* Copyright (c) 2016 LIBSCAPI (http://crypto.biu.ac.il/SCAPI)
* This file is part of the SCAPI project.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
* We request that any publication and/or code referring to and/or based on SCAPI contain an appropriate citation to SCAPI, including a reference to
* http://crypto.biu.ac.il/SCAPI.
* 
* Libscapi uses several open source libraries. Please see these projects for any further licensing issues.
* For more information , See https://github.com/cryptobiu/libscapi/blob/master/LICENSE.MD
*
* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
* 
*/


#include "../../include/comm/Comm.hpp"


/*****************************************/
/* SocketPartyData						 */
/*****************************************/
int SocketPartyData::compare(const SocketPartyData &other) const {
	string thisString = ipAddress.to_string() + ":" + to_string(port);
	string otherString = other.ipAddress.to_string() + ":" + to_string(other.port);
	return thisString.compare(otherString);
}

/*****************************************/
/* CommParty			                */
/*****************************************/

void CommParty::writeWithSize(const byte* data, int size) {
    t->SendRaw(sizeof(int), (uint8_t *)&size);
    t->SendRaw(size, (uint8_t *)data);
}

int CommParty::readSize() {
	byte buf[sizeof(int)];
	t->RecvRaw(sizeof(int), buf);
	int * res = (int *)buf;
	return *res;
}

size_t CommParty::readWithSizeIntoVector(vector<byte> & targetVector) {
	int msgSize = readSize();
	targetVector.resize(msgSize);
	auto res = t->RecvRaw(msgSize, (byte*)&targetVector[0]);
	return res;
}

/*****************************************/
/* CommPartyTCPSynced                    */
/*****************************************/

void CommPartyTCPSynced::join(int sleepBetweenAttempts, int timeout) {
	int     totalSleep = 0;
	bool    isAccepted  = false;
	bool    isConnected = false;
	// establish connections
    if (me.isServer()) {
        t->SetupServer(NULL, me.getPort());
        t->Accept();
    } else {
        t->SetupClient(other.getIpAddress().to_string().c_str(), other.getPort());
        t->Connect();
    }
}

void CommPartyTCPSynced::setSocketOptions() {
}

void CommPartyTCPSynced::write(const byte* data, int size) {
    uint32_t n = t->SendRaw(size, (uint8_t *)data);
	if (n == 0)
		throw PartyCommunicationException("Error while writing.");
}

CommPartyTCPSynced::~CommPartyTCPSynced() {
    delete t;
}
