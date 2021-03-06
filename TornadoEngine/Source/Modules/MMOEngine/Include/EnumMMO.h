/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ENUM_MMO_H
#define ENUM_MMO_H

namespace nsMMOEngine
{
  typedef enum
  {
		// open
		OpenClient_MoreThenOneSubNet,
    // CreateGroup
    CreateGroup_ClientNotExist,
    // LoginSlave
    LoginSlave_MasterNotReady,
    LoginSlave_NoAnswerFromMaster,
    //--------------------------------------
    // LoginMaster
    LoginMaster_SSNotReady,
    LoginMaster_NoAnswerFromSS,
    //--------------------------------------
    // LoginClient
    LoginClient_EmptyLoginPassword,
    LoginClient_ClientSecondEnter,
    LoginClient_ClientMasterNotReady,
    LoginClient_ClientNoAnswer,
    LoginClient_ClientNotExistSlave,
    LoginClient_SlaveNoAnswer,
    LoginClient_MasterKeyBusy,
    LoginClient_MasterClientNotActive,
    LoginClient_SuperServerNoAnswer,
    //--------------------------------------
    // RCM
    RCM_ClientNoAnswer,
  }ErrorCode;

  typedef enum
  {
    eConnectDown,
    eDisconnectDown,
    eConnectUp,
    eDisconnectUp,
    eError,
    eRecvFromDown,
    eRecvFromUp,
    eSaveContext,
    eRestoreContext,
    eTryLogin, 
    eResultLogin,
    eDestroyGroup,
    eEnterInQueue,
    eLeaveQueue,
  }tTypeEvent;
}
#endif