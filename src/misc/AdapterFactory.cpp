#include "AdapterFactory.h"
#include "ManagerFactory.h"
#include "x_sdk.h"
#include "x_config.h"

#define TYPE_OR_ID_SIZE 8

CAdapterFactory::CAdapterFactory(int)
{
	m_bRegiste = false;
	m_timer.Create(6 * 1000, CAdapterFactory::TimerThread, this);
}

CAdapterFactory::~CAdapterFactory()
{
	m_timer.Destroy();
}

int CAdapterFactory::RegisterAdapter(const char *adapterType, J_MakeAdapterFun pFun)
{
	AdapterRegistMap::iterator it = m_adapterRegistMap.find(adapterType);
	if (it == m_adapterRegistMap.end())
	{
		m_adapterRegistMap[adapterType] = pFun;
		J_OS::LOGINFO("CAdapterFactory::RegisterAdapter %s Regist Sucess!!!", adapterType);

		return J_OK;
	}

	return J_EXIST;
}

void *CAdapterFactory::CreateInstance(const char *pResId, int nStreamType, OBJ_TYPE nType)
{
	J_ChannelKey key;
	key.resid = pResId;
	key.stream_type = nStreamType;

	if (OBJ_ADAPTER == nType)
	{
		if (NULL == pResId)
		{
			J_OS::LOGINFO("CAdapterFactory::CreateInstence param error, type = %d", nType);
			return NULL;
		}

		AdapterMap::iterator it = m_adapterMap.find(pResId);
		if (it == m_adapterMap.end())
		{

		}
	}
	else if (OBJ_CHANNEL == nType)
	{
		if (NULL == pResId)
		{
			J_OS::LOGINFO("CAdapterFactory::CreateInstence param error, type = %d", nType);
			return NULL;
		}


        J_ChannelInfo channelInfo;
        int nRet = CManagerFactory::Instance()->GetManager(CXConfig::GetConfigType())->GetChannelInfo(pResId, channelInfo);
        if (nRet != J_OK)
        {
            J_OS::LOGINFO("CAdapterFactory::CreateInstence GetDevinfoByChannel error, resid = %s", pResId);
            return NULL;
        }

        char res_or_dvr[TYPE_OR_ID_SIZE];
        memset (res_or_dvr, 0, sizeof(res_or_dvr));
        sprintf(res_or_dvr, "%d", channelInfo.devId);

        AdapterMap::iterator it = m_adapterMap.find(res_or_dvr);
        if (it == m_adapterMap.end())
            return NULL;

        void *pObjChannel = NULL;
        J_DevAdapter *pDevAdapter = dynamic_cast<J_DevAdapter *>(it->second);
        if (pDevAdapter != NULL)
        {
            pDevAdapter->MakeChannel(pResId, pObjChannel, it->second, channelInfo.channelNum, nStreamType);
            //J_VoiceAdapter *pVoiceAdapter = dynamic_cast<J_VoiceAdapter *>((J_VideoAdapter *)it->second);
            //pVoiceAdapter->MakeVoice(pResId, pObjChannel, it->second, defConfig->parm.cha);
        }
        else
        {
            J_OS::LOGINFO("CAdapterFactory::CreateInstence VideoAdapter not Exist");
        }

        if (pObjChannel != NULL)
        {
            m_channelMap[key] = pObjChannel;
            J_ChannelStream *pChannel = static_cast<J_ChannelStream *>(pObjChannel);
            if (pChannel && !pChannel->HasMultiStream())
            {
                key.stream_type = (nStreamType == 0 ? 1 : 0);
                m_channelMap[key] = pObjChannel;
            }
            return pObjChannel;
        }
	}

	return NULL;
}

void* CAdapterFactory::GetInstance(const char *pResId, OBJ_TYPE nType, int nStreamType)
{
	if (OBJ_ADAPTER == nType)
	{
		//pResId 标识的是DVR ID
		AdapterMap::iterator it = m_adapterMap.find(pResId);
		if (it != m_adapterMap.end())
			return it->second;
	}
	else
	{
		//pResId 标识的是通道ID
		J_ChannelKey key;
		key.resid = pResId;
		key.stream_type = nStreamType;

		ChannelMap::iterator it = m_channelMap.find(key);
		if (it == m_channelMap.end())
		{
		    return CreateInstance(pResId, nStreamType, nType);
		}
		else
		{
            return it->second;
		}
	}

	return NULL;
}

int CAdapterFactory::RemoveInstance(const char *pResId, OBJ_TYPE nType)
{
	return J_OK;
}

void CAdapterFactory::OnTimer()
{
	if (m_bRegiste)
		return;

	int nRet = J_OK;
	char dev_id[TYPE_OR_ID_SIZE];
	std::vector<J_DeviceInfo> devList;
	nRet = CManagerFactory::Instance()->GetManager(CXConfig::GetConfigType())->ListDevices(devList);
	if (nRet == J_OK)
	{
		std::vector<J_DeviceInfo>::iterator itDvr = devList.begin();
		for (; itDvr != devList.end(); itDvr++)
		{
			memset(dev_id, 0, sizeof(dev_id));
			sprintf(dev_id, "%d", itDvr->devId);
			if (m_devMap.find(dev_id) == m_devMap.end())
			{
				itDvr->devStatus = J_DevBroken;
				m_devMap[dev_id] = *itDvr;
			}
		}

		DeviceMap::iterator itDev = m_devMap.begin();
		for (; itDev != m_devMap.end(); itDev++)
		{
			if (itDev->second.devStatus == J_DevBroken)
			{
				AdapterRegistMap::iterator itRegister = m_adapterRegistMap.find(itDev->second.devType);
				if (itRegister == m_adapterRegistMap.end())
				{
					J_OS::LOGINFO("CAdapterFactory::OnTimer Adapter not registed, adapterType = %d", itDvr->devType);
					continue;
				}

				J_Obj *pObj = NULL;
				itRegister->second(pObj, itDev->second.devId, itDev->second.devIp,
						itDev->second.devPort, itDev->second.userName, itDev->second.passWd);
				if (pObj != NULL)
				{
					char dev_id[TYPE_OR_ID_SIZE];
					memset(dev_id, 0, sizeof(dev_id));
					sprintf(dev_id, "%d", itDev->second.devId);
					itDev->second.devStatus = J_DevReady;
					m_adapterMap[dev_id] = pObj;
				}
			}
		}
		m_bRegiste = true;
	}

}