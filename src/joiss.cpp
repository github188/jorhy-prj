///流媒体服务相关头文件
#include "StreamManager.h"
#include "ControlManager.h"
#include "x_loadso.h"
#include "x_include.h"

#include "DeviceControl.h"

bool bRun = true;

///流媒体服务相关定义
std::vector<CStreamManager *> streamManagerVec;
std::vector<CControlManager *> controlManagerVec;

//CDeviceControl deviceControl;

void OnSignal(int nSigNum)
{
	if (nSigNum == SIGINT)
	{
		J_OS::LOGINFO("Quit JoServer");
		bRun = false;
	}
};

int main(int argc,char **argv)
{
    CXLoadso loadSo;
    ///加载模块
    if (loadSo.JoLoadSo() != J_OK)
    {
        J_OS::LOGINFO("main loadSo JoLoadSo error");
        return 0;
    }

	std::vector<J_ServerInfo> serverInfo = CXConfig::GetServerInfo();
	std::vector<J_ServerInfo>::iterator it = serverInfo.begin();
	for (; it!=serverInfo.end(); it++)
	{
		///流媒体服务相关
		CStreamManager *pStreamManager = new CStreamManager;
		if (pStreamManager->StartService(it->stream_port, it->stream_type) < 0)
		{
			delete pStreamManager;
			J_OS::LOGINFO("main streamManager Start service error, port = %d", it->stream_port);
			goto end;
		}
		streamManagerVec.push_back(pStreamManager);

		///管理服务器相关
		CControlManager *pControlManager = new CControlManager;
		if (pControlManager->StartService(it->control_port, it->control_type) < 0)
		{
			delete pControlManager;
			J_OS::LOGINFO("main controlManager Start service error, port = %d", it->control_port);
			goto end;
		}
		controlManagerVec.push_back(pControlManager);
	}

	//deviceControl.StartRecord("77");

    signal(SIGINT, OnSignal);
	while(bRun)
	{
		usleep(10000);
	}

end:
	std::vector<CStreamManager *>::iterator itStreamManager = streamManagerVec.begin();
	for (; itStreamManager!=streamManagerVec.end(); itStreamManager++)
	{
		(*itStreamManager)->StopService();
		delete (*itStreamManager);
	}
	
	std::vector<CControlManager *>::iterator itControlManager = controlManagerVec.begin();
	for (; itControlManager!=controlManagerVec.end(); itControlManager++)
	{
		(*itControlManager)->StopService();
		delete (*itControlManager);
	}
	loadSo.JoUnloadSo();

	return 0;
}

