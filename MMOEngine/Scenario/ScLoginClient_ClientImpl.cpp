/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScLoginClient_ClientImpl.h"
#include "ContextScLoginClient.h"
#include "ManagerSession.h"
#include "Base.h"
#include "Logger.h"
#include "Events.h"
#include "ErrorCode.h"
#include "SrcEvent_ex.h"
#include "MD5.h"
#include "CryptMITM.h"

using namespace nsMMOEngine;
using namespace nsLoginClientStruct;

TScLoginClient_ClientImpl::TScLoginClient_ClientImpl(IScenario* pSc):
TBaseScLoginClient(pSc)
{

}
//-----------------------------------------------------------------------------
void TScLoginClient_ClientImpl::RecvInherit(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->from)
  {
    case eSlave:
      RecvFromSlave(pDesc);
      break;
    case eMaster:
      RecvFromMaster(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//-----------------------------------------------------------------------------
void TScLoginClient_ClientImpl::Work(unsigned int time_ms)
{
	unsigned int time_end_ms = Context()->GetTimeWait() + eTimeWait;
	if(time_end_ms < time_ms)
  {
    // ������ �� ��� �������
    TEventError event;
    event.code = LoginClient_ClientNoAnswer;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
}
//-----------------------------------------------------------------------------
void TScLoginClient_ClientImpl::TryLogin(unsigned int ip, unsigned short port, 
                                         void* data, int size, unsigned char subNet)
{
	if(Begin()==false)
  {
    // ��������� ������
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TScenarioLoginClient::TryLogin() scenario is not active.\n");
    BL_FIX_BUG();
    return;
  }
  Context()->SetNeedLeaveQueue(false);
  Context()->SetSubNet(subNet);
  // ������������ ������ ����� - ���
  TBreakPacket bp;
  SetID_SessionClientMaster(Context()->GetMS()->Send(ip, port, bp, subNet));
  if(GetID_SessionClientMaster()==INVALID_HANDLE_SESSION)
  {
    // ��������� ������
    TEventError event;
    event.code = LoginClient_ClientMasterNotReady;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
  }
  else
  {
    TBreakPacket bpLP;// ��������� ��� ����� ������
    if(Context()->GetMS()->GetUseCryptTCP())
    {
      // ���� ������ ���������, �� ����������� ���:
      // �������� RSA public key �� ManagerSession
      TContainer cRSA;
      bool resRSA = Context()->GetMS()->GetRSAPublicKeyForUp(cRSA);
      BL_ASSERT(resRSA);
			TCryptMITM cryptMITM;
      TContainer cEncryptRSA_bySHA1_LP;
			bool res = cryptMITM.Calc(cRSA.GetPtr(), cRSA.GetSize(),
																data, size, cEncryptRSA_bySHA1_LP);
      BL_ASSERT(res);
      
			TMD5 md5;
			TContainer cMD5LP;
			md5.FastCalc(data, size, cMD5LP);
      // ������� MD5LP
      bpLP.PushFront((char*)cMD5LP.GetPtr(), cMD5LP.GetSize());
      // ����� ������������� RSA
      bpLP.PushFront((char*)cEncryptRSA_bySHA1_LP.GetPtr(), cEncryptRSA_bySHA1_LP.GetSize());
    }
    else
    {
      // ����� ������ �������� ������:
      // ������������ ������
      bpLP.PushFront((char*)data,size);
    }
    THeaderTryLoginC2M h;
    bpLP.PushFront((char*)&h, sizeof(h));
    // �������� ����� ��� ������� �����������
    Context()->GetMS()->Send(GetID_SessionClientMaster(), bpLP);

    SetTimeWaitForNow();
  }
}
//-----------------------------------------------------------------------------
void TScLoginClient_ClientImpl::RecvFromSlave(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eCheckConnectToSlaveS2C:
      CheckConnectToSlaveS2C(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::RecvFromMaster(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eResultLoginM2C:
      ResultLoginM2C(pDesc);
      break;
    case eInfoSlaveM2C:
      InfoSlaveM2C(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::CheckConnectToSlaveS2C(TDescRecvSession* pDesc)
{
  TEventResultLogin event;
  event.res = TMaster::eAccept;
  Context()->GetSE()->AddEventCopy(&event, sizeof(event));
  
  ((IContextScenario*)Context())->SetID_Session(pDesc->id_session);

  End();
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::ResultLoginM2C(TDescRecvSession* pDesc)
{
  // �������� ����� �������
  SetTimeWaitForNow();
  //==============================
  THeaderResultLoginM2C* pH = (THeaderResultLoginM2C*)pDesc->data;
  switch(pH->result)
  {
    case THeaderResultLoginM2C::eAccept:
      // ��������� ���� ����
      Context()->SetClientKey(pH->id_client);
      EventSetClientKey(pH->id_client);
      break;
    case THeaderResultLoginM2C::eReject:
    {
      CloseSessionMaster();

			TEventResultLogin* pEvent = new TEventResultLogin;
			pEvent->res = TMaster::eReject;
			// ��������� ������, ������� �������� ������� ������
			char* pDataResClient = pDesc->data + sizeof(THeaderResultLoginM2C);
			pEvent->c.SetDataByCount( pDataResClient, pH->sizeResClient);
			Context()->GetSE()->AddEventWithoutCopy<TEventResultLogin>(pEvent);
      End();
    }
      break;
    case THeaderResultLoginM2C::eQueue:
    {
      Context()->SetClientKey(pH->id_client);
      EventSetClientKey(pH->id_client);

      Context()->SetNumInQueue(pH->numInQueue);

      TEventEnterInQueue event;
      event.numInQueue = pH->numInQueue;
      Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    }
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::InfoSlaveM2C(TDescRecvSession* pDesc)
{
  Context()->SetNeedLeaveQueue(false);
  // ������� ��� ��� ��������
  THeaderInfoSlaveM2C* pInfoSlave = (THeaderInfoSlaveM2C*)pDesc->data;
  Context()->SetSlaveIP_Port(pInfoSlave->ip_port_slave);
  // ����� ��� �������, ��� �� �������������� ��� ���������
  // � ������� ����� ����� �������
  Context()->SetClientKey(pInfoSlave->id_client);
  EventSetClientKey(pInfoSlave->id_client);

  // ��������� ����� ��� Master
  TBreakPacket bp;
  THeaderCheckInfoSlaveC2M h;
  h.id_client = Context()->GetClientKey();// ����������� - pInfoSlave->id_client;
  bp.PushFront((char*)&h, sizeof(h));

  Context()->GetMS()->Send(GetID_SessionClientMaster(),bp);
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::LeaveQueue()
{
  // �������� � ��������� ������� �� ������ � �������
  if(Context()->GetNumInQueue()==0)
    return;

  TBreakPacket bp;
  THeaderLeaveQueueC2M h;
  bp.PushFront((char*)&h, sizeof(h));

  unsigned int id_master = GetID_SessionClientMaster();
  if(id_master==INVALID_HANDLE_SESSION)
    return;

  Context()->SetNeedLeaveQueue(true);
  Context()->GetMS()->Send(id_master,bp);
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::CloseSessionMaster()
{
  Context()->GetMS()->CloseSession(GetID_SessionClientMaster());
  SetID_SessionClientMaster(INVALID_HANDLE_SESSION);
}
//--------------------------------------------------------------
void TScLoginClient_ClientImpl::Disconnect()
{
  // ���� �� � ������� � ����� �� ����� �� ���
  if(Context()->NeedLeaveQueue())
  {
    End();
    return;
  }
  // ���� ���������� ��������� � ������� �� Slave
  // ��������� ����� ��� Slave
  TBreakPacket bp;
  THeaderConnectToSlaveC2S h;
  // ��� Slave ������ ���� ID, �� �� ���� ��� ��������������  
  h.id_client = Context()->GetClientKey();
  bp.PushFront((char*)&h, sizeof(h));
  // ������� ������ �� IP:port
  TIP_Port ip_port_slave = Context()->GetSlaveIP_Port();
  unsigned int id_slave = 
    Context()->GetMS()->Send(ip_port_slave.ip,
                             ip_port_slave.port,
                             bp, Context()->GetSubNet());
  // �������� �� ������� ���������� Slave
  if(id_slave==INVALID_HANDLE_SESSION)
  {
    // ��������� ������
    TEventError event;
    event.code = LoginClient_ClientNotExistSlave;
    Context()->GetSE()->AddEventCopy(&event, sizeof(event));
    End();
    return;
  }
  SetID_SessionClientSlave(id_slave);
  SetTimeWaitForNow();
}
//--------------------------------------------------------------
