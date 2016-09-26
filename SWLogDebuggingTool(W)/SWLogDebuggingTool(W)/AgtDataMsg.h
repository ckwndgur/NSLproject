#pragma once

class AgtDataMsg
{
public:
	AgtDataMsg(void);
	~AgtDataMsg(void);
};

struct AgtDataMsgStruct
{
	short nReqType;
	char cLogData[2048];
	long lTotalDataSize;
	bool bLastPacket;
};