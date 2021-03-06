/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef DevTool_EditorMapH
#define DevTool_EditorMapH

#include "DevTool_Share.h"

class TDevTool_EditorMap : public TDevTool_Share
{
public:
  TDevTool_EditorMap();
  virtual ~TDevTool_EditorMap();

  virtual std::string GetVariantConveyer();
protected:
  virtual TModuleDev* GetModuleLogic();
};

#endif
