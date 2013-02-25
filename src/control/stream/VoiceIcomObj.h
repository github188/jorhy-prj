#ifndef __VOICEICOMOBJ_H_
#define __VOICEICOMOBJ_H_
#include "j_includes.h"
#include "AdapterManager.h"
#include "x_socket.h"

class CVoiceIcomObj : public J_MediaObj
{
public:
	CVoiceIcomObj(int nSocket, J_Obj *pObj);
	~CVoiceIcomObj();

public:
	///J_Client
	virtual int Process(int nIoType);
	virtual int Clearn();
	virtual int Run(bool bFlag);

private:
	//语音对讲
	int StartVoice();
	int StopVoice();

private:
	int m_nSocket;
	CRingBuffer *m_pRingBuffer;
	bool m_bStart;
	char *m_pDataBuff;
	char *m_pConvetBuff;

	std::string m_resid;

	J_OS::CTCPSocket m_sendSocket;
	J_StreamHeader m_streamHeader;

	J_Obj *m_pObj;
};

#endif//~__VOICEICOMOBJ_H_