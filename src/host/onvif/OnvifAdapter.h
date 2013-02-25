#ifndef __ONVIFADAPTER_H_
#define __ONVIFADAPTER_H_
#include "j_includes.h"
#include "ModuleManagerDef.h"

class COnvifAdapterBase : public J_DevAdapter
{};

class COnvifAdapter : J_BaseAdapter<COnvifAdapterBase>
{
	friend class COnvifChannel;
    public:
        COnvifAdapter(int nDvrId, const char *pAddr, int nPort, const char *pUsername, const char *pPassword);
        ~COnvifAdapter();

        static int Maker(J_Obj *&pObj, int nDrvId, const char *pAddr, int nPort, const char *pUsername, const char *pPassword)
        {
            pObj = new COnvifAdapter(nDrvId, pAddr, nPort, pUsername, pPassword);
            return J_OK;
        }

    public:
        ///J_VideoAdapter
        virtual J_DevStatus GetStatus() const;
        virtual int Broken();
        virtual int MakeChannel(const char *pResid, void *&pObj, void *pOwner, int nChannel, int nStream, int nMode);

    protected:
        char *GetRemoteIp() const { return (char *)m_remoteIP; }
        int GetRemotePort() const { return m_remotePort; }
        char *GetUser() const { return (char *)m_username; }
        char *GetPassword() const { return (char *)m_password; }

    private:
        J_DevStatus m_status;
        char m_remoteIP[16];
        int  m_remotePort;
        char m_username[64];
        char m_password[64];
};

ADAPTER_BEGIN_MAKER(onvif)
	ADAPTER_ENTER_MAKER("onvif", COnvifAdapter::Maker)
ADAPTER_END_MAKER()

#endif //~__ONVIFADAPTER_H_