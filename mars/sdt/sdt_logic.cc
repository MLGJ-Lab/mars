// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in 
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.

/*
 * sdt_logic.cc
 *
 *  Created on: 2016年3月18日
 *      Author: caoshaokun
 */

#include "mars/sdt/sdt_logic.h"

#include "mars/baseevent/baseevent.h"
#include "mars/baseevent/baseprjevent.h"
#include "mars/comm/xlogger/xlogger.h"
#include "mars/comm/bootrun.h"
#include "mars/sdt/constants.h"

#include "sdt/src/sdt_core.h"
#include "sdt_manager.h"

namespace mars {
namespace sdt {

//static Callback* sg_callback = NULL;
static SdtManager* sdt_manager_ = new SdtManager();

static const std::string kLibName = "sdt";

//#define SDT_WEAK_CALL(func) \
//    boost::shared_ptr<SdtCore> sdt_ptr = SdtCore::Singleton::Instance_Weak().lock();\
//    if (!sdt_ptr) {\
//        xwarn2(TSF"sdt uncreate");\
//        return;\
//    }\
//	sdt_ptr->func

static void onCreate() {
    xinfo2(TSF"sdt oncreate");
    //SdtCore::Singleton::Instance();
    sdt_manager_->OnCreate();
}

static void onDestroy() {
    xinfo2(TSF"sdt onDestroy");
    //SdtCore::Singleton::AsyncRelease();
    sdt_manager_->OnDestroy();
}

static void __initbind_baseprjevent() {

	GetSignalOnCreate().connect(&onCreate);
	GetSignalOnDestroy().connect(5, &onDestroy);
}

BOOT_RUN_STARTUP(__initbind_baseprjevent);

//active netcheck interface
void StartActiveCheck(CheckIPPorts& _longlink_check_items, CheckIPPorts& _shortlink_check_items, int _mode, int _timeout) {
	//SDT_WEAK_CALL(StartCheck(_longlink_check_items, _shortlink_check_items, _mode, _timeout));
        sdt_manager_->StartActiveCheck(_longlink_check_items, _shortlink_check_items, _mode, _timeout);
}

void CancelActiveCheck() {
	//SDT_WEAK_CALL(CancelCheck());
        sdt_manager_->CancelActiveCheck();
}

void SetCallBack(Callback* const callback) {
	//sg_callback = callback;
        sdt_manager_->SetCallBack(callback);
}

#if !defined(ANDROID) || defined (CPP_CALL_BACK)
void (*ReportNetCheckResult)(const std::vector<CheckResultProfile>& _check_results)
= [](const std::vector<CheckResultProfile>& _check_results) {
    sdt_manager_->ReportNetCheckResult(_check_results);
};

#endif

}}
