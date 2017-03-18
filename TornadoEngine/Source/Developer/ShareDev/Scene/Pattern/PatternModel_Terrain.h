/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef PatternModel_TerrainH
#define PatternModel_TerrainH

#include "TypeDef.h"
#include "BehaviourPatternModel.h"

class TPatternContext_Terrain;

class DllExport TPatternModel_Terrain : public TBehaviourPatternModel
{
public:
  TPatternModel_Terrain();
  virtual ~TPatternModel_Terrain();

  virtual TBehaviourPatternContext* MakeNewConext();

	virtual bool LoadFromThread_Ogre(TBehaviourPatternContext* pContext, bool fast = true);
  virtual bool LoadFromThread_Bullet(TBehaviourPatternContext* pContext, bool fast = true);

protected:
	void BeginLoad_Bullet(TPatternContext_Terrain* pContextTerrain);
	bool TryLoad_Bullet(TPatternContext_Terrain* pContextTerrain);

	void BeginLoad_Ogre(TPatternContext_Terrain* pContextTerrain);
	bool TryLoad_Ogre(TPatternContext_Terrain* pContextTerrain);
};

#endif
