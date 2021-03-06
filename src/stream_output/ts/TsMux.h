#ifndef __TS_MUX_H_
#define __TS_MUX_H_
#include "j_includes.h"
#include "x_time.h"
#include "x_module_manager_def.h"

#define PAT_INTERVAL			30
#define PCR_INTERVAL			10
#define TS_DATA_LEN			188
#define TS_MAX_PAYLOAD_LEN	184
#define TS_NEED_CUT_LEN		183
#define TS_PAYLOAD_CUT_LEN	92

class CTsMux : public J_MuxFilter
{
public:
	CTsMux();
	~CTsMux();

	static int Maker(J_Obj *&pObj)
	{
		pObj = new CTsMux();
		return J_OK;
	}

public:
	virtual j_result_t Init();
	virtual void Deinit();
	virtual j_result_t Convert(const char *pInputData, J_StreamHeader &streamHeader, char *pOutputData, int &nOutLen, void *pExtdata = 0);

private:
	int MakePESHead(char *pHead, int nDataLen, j_uint64_t timeStamp, j_boolean_t isAudio = false);
	int MakePAT(char *pHead);
	int MakePMT(char *pHead);
	int MakePCR(char *pHead, j_uint64_t timeStamp);
	int MakePacket(char *pHead, const char *pInputData, int nLen, j_boolean_t bIsAudio = false, j_boolean_t bIsStart = false);

private:
	int m_nPATCounter;
	int m_nPMTCounter;
	int m_nPCRCounter;
	int m_nVideoCounter;
	int m_nAudioCounter;
	int m_nFrameNum;

	int m_videoStamp;
	int m_audioStamp;
};

#endif //~__TS_MUX_H_
