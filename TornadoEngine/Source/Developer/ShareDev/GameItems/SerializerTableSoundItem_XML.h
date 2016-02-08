/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SerializerTableSoundItem_XMLH
#define SerializerTableSoundItem_XMLH

#include <string>
#include "TypeDef.h"
#include "BaseSerializerItem_XML.h"

struct TTableSoundItem;

class DllExport TSerializerTableSoundItem_XML : public TBaseSerializerItem_XML
{
	TTableSoundItem* mTableSound;
public:
	TSerializerTableSoundItem_XML();
	virtual ~TSerializerTableSoundItem_XML();

	virtual bool Load(TBaseItem* pItem);
	virtual bool Save(TBaseItem* pItem);
protected:
};

#endif