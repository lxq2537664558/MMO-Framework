/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "EditorTerrainLogic.h"
#include "ListModules.h"
#include "Logger.h"

#include "ModuleTimer.h"
#include "ModuleGraphicEngine.h"
#include "ModulePhysicEngine.h"
#include "EditorTerrain.h"

TEditorTerrainLogic::TEditorTerrainLogic()
{
  mEditorTerrain = NULL;
}
//-------------------------------------------------------------------
TEditorTerrainLogic::~TEditorTerrainLogic()
{

}
//-------------------------------------------------------------------
void TEditorTerrainLogic::StartEvent()
{
  CallBackModule(nsListModules::Timer, &TEditorTerrainLogic::StartTimer);
  CallBackModule(nsListModules::GraphicEngine, &TEditorTerrainLogic::InitForms);

  mComp.pGraphicEngine->GetCBBeginWork()->Register( &TEditorTerrainLogic::GraphicBeginWork ,this);
  mComp.pGraphicEngine->GetCBStopEvent()->Register( &TEditorTerrainLogic::FreeGraphicResource,this);
}
//-------------------------------------------------------------------
void TEditorTerrainLogic::InitLog()
{
  GetLogger()->Register("Inner");// ��� ����������� ���������� �������
  GetLogger()->Init("EditorMaterial");
}
//-------------------------------------------------------------------
bool TEditorTerrainLogic::WorkClient()
{
  return true;
}
//-------------------------------------------------------------------
void TEditorTerrainLogic::Input(int id_sender, void* p, int size)
{
  switch(id_sender)
  {
    case nsListModules::GraphicEngine:
      HandleGraphicEngine(p, size);
      break;
    case nsListModules::PhysicEngine:
      break;
    case nsListModules::Timer:
      //CallBackModule(nsListModules::GraphicEngine, &TEditorShapeLogic::CheckMoveCamera);
      break;
    default:BL_FIX_BUG();
  }
}
//-------------------------------------------------------------------
void TEditorTerrainLogic::StartTimer()
{
  // ��������� �� ������ �������
  unsigned int mID_Timer = mComp.pTimer->New(100);
}
//----------------------------------------------------------
void TEditorTerrainLogic::InitForms()
{
  mEditorTerrain = new TEditorTerrain;
  mEditorTerrain->Show();
}
//----------------------------------------------------------
void TEditorTerrainLogic::GraphicBeginWork()
{
  CheckMoveCamera();
}
//---------------------------------------------------------------------------------------------
void TEditorTerrainLogic::FreeGraphicResource()
{
  delete mEditorTerrain;
}
//---------------------------------------------------------------------------------------------
void TEditorTerrainLogic::HandleGraphicEngine(void* p, int size)
{
  nsGraphicEngine::TBaseEvent* pBase = (nsGraphicEngine::TBaseEvent*)p;
  switch(pBase->type)
  {
    case nsGraphicEngine::eMouse:
      HandleMouseEvent(p);
      break;
    case nsGraphicEngine::eKeyBoard:
      HandleKeyEvent(p);
      break;
  }
}
//---------------------------------------------------------------------------------------------
void TEditorTerrainLogic::HandleMouseEvent(void* p)
{
  Ogre::Camera* pCamera = mComp.pGraphicEngine->GetGE()->GetCamera();
  Ogre::Radian angle;

  nsGraphicEngine::TMouseEvent* pME = (nsGraphicEngine::TMouseEvent*)p;
  switch(pME->typeEvent)
  {
    case nsGraphicEngine::eButtonDown:
      mOld_X = pME->x;
      mOld_Y = pME->y;
      break;
    case nsGraphicEngine::eButtonUp:
      break;
    case nsGraphicEngine::eButtonDblClick:
      break;
    case nsGraphicEngine::eWheel:
      break;
    case nsGraphicEngine::eMove:
      {
        if(pME->pressedButtons & (1<<OIS::MB_Left))
        {
          int dx = pME->x - mOld_X;
          int dy = pME->y - mOld_Y;
          mOld_X = pME->x;
          mOld_Y = pME->y;

          angle = dx/600.0f;
          pCamera->yaw(angle);

          angle = dy/600.0f;
          pCamera->pitch(angle);
        }
      }
      break;
  }
}
//---------------------------------------------------------------------------------------------
void TEditorTerrainLogic::HandleKeyEvent(void* p)
{
  nsGraphicEngine::TKeyEvent* pKE = (nsGraphicEngine::TKeyEvent*)p;
  switch(pKE->key)
  {
    case OIS::KC_ESCAPE:
      Exit();
      break;
    case OIS::KC_W:
      mDrive.for_back.dir = pKE->pressed ? -1 : 0;
      mDrive.for_back.SetNow();
      break;
    case OIS::KC_S:
      mDrive.for_back.dir = pKE->pressed ? 1 : 0;
      mDrive.for_back.SetNow();
      break;
    case OIS::KC_A:
      mDrive.left_right.dir = pKE->pressed ? -1 : 0;
      mDrive.left_right.SetNow();
      break;
    case OIS::KC_D:
      mDrive.left_right.dir = pKE->pressed ? 1 : 0;
      mDrive.left_right.SetNow();
      break;
    case OIS::KC_Q:
      mDrive.up_down.dir = pKE->pressed ? 1 : 0;
      mDrive.up_down.SetNow();
      break;
    case OIS::KC_E:
      mDrive.up_down.dir = pKE->pressed ? -1 : 0;
      mDrive.up_down.SetNow();
      break;
  }
}
//---------------------------------------------------------------------------------------------
void TEditorTerrainLogic::CheckMoveCamera()
{
  Ogre::Camera* pCamera = mComp.pGraphicEngine->GetGE()->GetCamera();
  Ogre::Vector3 moveVec(0,0,0);

  CalcMoveForAxe(&mDrive.up_down, &moveVec.y);
  CalcMoveForAxe(&mDrive.for_back, &moveVec.z);
  CalcMoveForAxe(&mDrive.left_right, &moveVec.x);

  pCamera->moveRelative(moveVec);
}
//---------------------------------------------------------------------------------------------
void TEditorTerrainLogic::CalcMoveForAxe(TDriveCamera::TMove* pMove, float* pAxe)
{
  unsigned int now = ht_GetMSCount();
  float dt = now - pMove->last_time;
  if(dt==0)
    return;

  const float shift = 0.06f;
  *pAxe = shift * pMove->dir * dt;

  pMove->last_time = now;
}
//---------------------------------------------------------------------------------------------
