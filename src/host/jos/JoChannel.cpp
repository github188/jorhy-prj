#include "JoChannel.h"
#include "JoStream.h"
#include "x_base64.h"
#include "x_config.h"

CJoChannel::CJoChannel(const char *pResid, void *pOwner, int nChannel, int nStream, int nMode)
: m_pAdapter(NULL)
, m_nChannel(0)
, m_bOpened(false)
{
	m_pAdapter = (CJoAdapter *) pOwner;
	m_nChannel = nChannel;
	m_nStreamType = nStream;
	m_nProtocol = nMode;
	m_resid = pResid;

	m_recvSocket = NULL;
}

CJoChannel::~CJoChannel()
{

}

int CJoChannel::OpenStream(void *&pObj, CRingBuffer *pRingBuffer)
{
	if (m_pAdapter->GetStatus() != jo_dev_ready)
	{
		m_bOpened = false;
		return J_DEV_BROKEN;
	}

	if (m_bOpened && pObj != NULL)
	{
		(static_cast<CJoStream *> (pObj))->AddRingBuffer(pRingBuffer);

		return J_OK;
	}

	int nRet = StartView();
	if (nRet != J_OK)
	{
	    //m_pAdapter->Broken();
		return J_STREAM_ERROR;
	}

	m_bOpened = true;
	pObj = new CJoStream(m_recvSocket, m_resid);
	(static_cast<CJoStream *> (pObj))->AddRingBuffer(pRingBuffer);
	(static_cast<CJoStream *> (pObj))->Startup();

	return J_OK;
}

int CJoChannel::CloseStream(void *pObj, CRingBuffer *pRingBuffer)
{
	if (!m_bOpened)
		return J_OK;

	CJoStream *pSonyStream = static_cast<CJoStream *>(pObj);
	if (pSonyStream == NULL)
		return J_OK;

	if (pSonyStream->RingBufferCount() == 1)
	{
		StopView();
		m_bOpened = false;
		(static_cast<CJoStream *> (pObj))->Shutdown();
		pSonyStream->DelRingBuffer(pRingBuffer);
		delete (CJoStream *) pObj;

		return J_NO_REF;
	}

	if (pSonyStream->RingBufferCount() > 0)
		pSonyStream->DelRingBuffer(pRingBuffer);

	return J_OK;
}


int CJoChannel::StartView()
{
	if (m_recvSocket != NULL)
	{
		delete m_recvSocket;
		m_recvSocket = NULL;
	}
	
	J_DeviceInfo info = {0};
	if (CManagerFactory::Instance()->GetManager(CXConfig::GetConfigType())->GetDeviceInfo(m_resid.c_str(), info) != J_OK)
		return J_INVALID_DEV;
	m_recvSocket = new J_OS::CTCPSocket();
	m_recvSocket->Connect(info.devIp, info.devPort);

	if (m_jospHelper.OpenStream(m_recvSocket, m_resid.c_str(), m_nStreamType) != J_OK)
	{
		delete m_recvSocket;
		m_recvSocket = NULL;
		
		return J_INVALID_DEV;
	}

	return J_OK;
}

int CJoChannel::StopView()
{
	m_jospHelper.CloseStream(m_recvSocket, m_resid.c_str());
	if (m_recvSocket != NULL)
	{
		delete m_recvSocket;
		m_recvSocket = NULL;
	}

	return J_OK;
}
