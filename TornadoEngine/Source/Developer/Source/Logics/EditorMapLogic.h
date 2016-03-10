/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef EditorMapLogicH
#define EditorMapLogicH

#include "ModuleClientLogic.h"
#include "BuilderGameMap.h"

class TEditorMap;

class TEditorMapLogic : public TModuleClientLogic
{
  volatile bool flgLoadGameMap;
  volatile bool flgNeedLoad_OGRE;
  volatile bool flgNeedLoad_Bullet;
  volatile bool flgNeedLoad_OpenAL;

  TBuilderGameMap mBuilderGameMap;

  TEditorMap* mEditorMap;
public:
  TEditorMapLogic();
  virtual ~TEditorMapLogic();

  virtual void StartEvent();
  virtual void InitLog();
protected:
  virtual bool WorkClient();

  virtual void Input(int id_sender, void* p, int size);
private:
  void StartTimer();
  void InitForms();

  void InitPhysic();

  void GraphicBeginWork();
  void FreeGraphicResource();

  void PhysicBeginWork();
  void PhysicEndWork();

private:
  void LoadGameMap(std::string& nameMap);
};

#endif
