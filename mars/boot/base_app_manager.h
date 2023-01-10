//
// Created by Changpeng Pan on 2022/9/9.
//

#ifndef MMNET_BASE_APP_MANAGER_H
#define MMNET_BASE_APP_MANAGER_H

#include "base_manager.h"
#include "mars/app/app.h"
#include "mars/comm/comm_data.h"
#include "mars/comm/time_utils.h"

namespace mars {
namespace app {

class BaseAppManager : public boot::BaseManager {
 public:
    virtual ~BaseAppManager() {
    }
    virtual void Init() = 0;
    virtual void UnInit() = 0;

 public:
    virtual void SetCallback(Callback* callback) = 0;
    virtual mars::comm::ProxyInfo GetProxyInfo(const std::string& _host) = 0;
    virtual std::string GetAppFilePath() = 0;
    virtual AccountInfo GetAccountInfo() = 0;
    virtual std::string GetUserName() = 0;
    virtual std::string GetRecentUserName() = 0;
    virtual unsigned int GetClientVersion() = 0;
    virtual DeviceInfo GetDeviceInfo() = 0;
    virtual double GetOsVersion() = 0;

#ifdef NATIVE_CALLBACK
    virtual void SetAppLogicNativeCallback(std::shared_ptr<AppLogicNativeCallback> _cb) = 0;
#endif

//TODO mars2 cpan
//#if !defined(ANDROID) || defined(CPP_CALL_BACK)
    virtual void __GetProxyInfo(const std::string& _host, uint64_t _timetick) = 0;
//#endif

//TODO mars2 cpan
//#if TARGET_OS_IPHONE
    virtual void __ClearProxyInfo() = 0;
//#endif
    
};

}  // namespace app
}  // namespace mars

#endif  // MMNET_BASE_APP_MANAGER_H