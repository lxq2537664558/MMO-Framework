/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "BaseSerializerItem_XML.h"
#include "IXML.h"
#include <errno.h>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include "BL_Debug.h"

namespace nsBaseSerializerItem_XML
{
  // Common
  const char* sName     = "name";
  const char* sProperty = "Property";
  const char* sKey      = "key";
  const char* sValue    = "value";

  const char* sAxeX     = "x";
  const char* sAxeY     = "y";
  const char* sAxeZ     = "z";
  const char* sAxeW     = "w";

  const char* sAngle    = "angle";

  const int CountAxes3       = 3;
  const int CountAxes4       = 4;
  //const int CountOrientation = 9;
	const int CountOrientation = 4;

	const char* sColourRed   = "r";
	const char* sColourGreen = "g";
	const char* sColourBlue  = "b";
	const char* sColourAlpha = "a";
}

using namespace nsBaseSerializerItem_XML;

TBaseSerializerItem_XML::TBaseSerializerItem_XML(std::string type)
{
  mType = type;

	for( int i = 0 ; i < 3 ; i++)
	{
		for( int j = 0 ; j < 3 ; j++)
		{
			std::string name = boost::lexical_cast<std::string>(i) + boost::lexical_cast<std::string>(j);
			mMapNameIndex_m3x3.insert(TMapStrIndexVT(name,TIndex(i,j)));
		}
	}
}
//------------------------------------------------------------------------------
TBaseSerializerItem_XML::~TBaseSerializerItem_XML()
{
  mXML = NULL;
}
//------------------------------------------------------------------------------
void TBaseSerializerItem_XML::Init(IXML* xml)
{
  mXML = xml;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::EnterByType(std::string& name)
{
  if(EnterRoot()==false)
    return false;
  int cnt = mXML->GetCountSection(mType.data());
  if(cnt==0)
    return false;

  for(int i = 0 ; i < cnt ; i++)
  {
    std::string sAttrName = mXML->ReadSectionAttr(mType.data(), i, sName);
    if(name==sAttrName)
      return mXML->EnterSection(mType.data(), i);
  }
  return false;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::AddAndEnterByType(std::string& name)
{
  TAttrInfo attr;
  attr.Name  = sName;
  attr.Value = name;
  return mXML->AddSectionAndEnter(mType.data(), 1, &attr);
}
//------------------------------------------------------------------------------
int TBaseSerializerItem_XML::GetCountProperty()
{
  return mXML->GetCountSection(sProperty);
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::LoadProperty(int index, std::string& key, std::string& value)
{
  if(index>=GetCountProperty())
    return false;

  key = mXML->ReadSectionAttr(sProperty, index, sKey);
  value = mXML->ReadSectionAttr(sProperty, index, sValue);
  return true;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::SaveProperty(std::string& key, std::string& value)
{
  TAttrInfo attr[2];
  attr[0].Name  = sKey;
  attr[0].Value = key;
  attr[1].Name  = sValue;
  attr[1].Value = value;
  return mXML->AddSection(sProperty, 2, &attr[0]);
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::LoadVector3ByProperty(nsMathTools::TVector3& v3)
{
  std::string key, value;
  int cnt = GetCountProperty();
  
  if(cnt!=CountAxes3)// кол-во компонентов в векторе
    return false;

  for( int i = 0 ; i < cnt ; i++ )
  {
    if(LoadProperty(i,key,value))
    {
      if(key==sAxeX)
        v3.x = boost::lexical_cast<float>(value.data());
      else if(key==sAxeY)
        v3.y = boost::lexical_cast<float>(value.data());
      else if(key==sAxeZ)
        v3.z = boost::lexical_cast<float>(value.data());

      if(errno==ERANGE)
        return false;
    }
    else 
      return false;
  }
  return true;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::SaveVector3ByProperty(nsMathTools::TVector3& v3)
{
  std::string key, value;

  key = sAxeX;
  value = boost::lexical_cast<std::string>(v3.x);
  if(SaveProperty(key,value)==false)
    return false;
  
  key = sAxeY;
  value = boost::lexical_cast<std::string>(v3.y);
  if(SaveProperty(key,value)==false)
    return false;

  key = sAxeZ;
  value = boost::lexical_cast<std::string>(v3.z);
  if(SaveProperty(key,value)==false)
    return false;

  return true;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::LoadVector4ByProperty(nsMathTools::TVector4& v4)
{
	std::string key, value;
	int cnt = GetCountProperty();

	if(cnt!=CountAxes4)// кол-во компонентов в векторе
		return false;

	for( int i = 0 ; i < cnt ; i++ )
	{
		if(LoadProperty(i,key,value))
		{
			if(key==sAxeX)
				v4.x = boost::lexical_cast<float>(value.data());
			else if(key==sAxeY)
				v4.y = boost::lexical_cast<float>(value.data());
			else if(key==sAxeZ)
				v4.z = boost::lexical_cast<float>(value.data());
			else if(key==sAxeW)
				v4.w = boost::lexical_cast<float>(value.data());

			if(errno==ERANGE)
				return false;
		}
		else 
			return false;
	}
	return true;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::SaveVector4ByProperty(nsMathTools::TVector4& v4)
{
	std::string key, value;

	key = sAxeX;
	value = boost::lexical_cast<std::string>(v4.x);
	if(SaveProperty(key,value)==false)
		return false;

	key = sAxeY;
	value = boost::lexical_cast<std::string>(v4.y);
	if(SaveProperty(key,value)==false)
		return false;

	key = sAxeZ;
	value = boost::lexical_cast<std::string>(v4.z);
	if(SaveProperty(key,value)==false)
		return false;

	key = sAxeW;
	value = boost::lexical_cast<std::string>(v4.w);
	if(SaveProperty(key,value)==false)
		return false;

	return true;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::LoadOrientationByProperty(nsMathTools::TMatrix16& m4x4)
{
	std::string key, value;
	int cnt = GetCountProperty();

	if(cnt!=CountOrientation)
		return false;

	SetMatrixIdentity(&m4x4);
	nsMathTools::TVector3 axis;
	float angle;

	for( int i = 0 ; i < cnt ; i++ )
	{
		if(LoadProperty(i,key,value))
		{
			if(key==sAxeX)
				axis.x = boost::lexical_cast<float>(value.data());
			else if(key==sAxeY)
				axis.y = boost::lexical_cast<float>(value.data());
			else if(key==sAxeZ)
				axis.z = boost::lexical_cast<float>(value.data());
			else if(key==sAngle)
				angle = boost::lexical_cast<float>(value.data());
		}
		else 
			return false;
	}
	if(errno==ERANGE)
		return false;

	SetMatrixRotationAxis(&m4x4, &axis, angle);
	return true;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::SaveOrientationByProperty(nsMathTools::TMatrix16& m4x4)
{
	nsMathTools::TVector3 axis;
	float angle;
	SetMatrixToAxisAngle(&m4x4, &axis, &angle);

	std::string key, value;

	key = sAxeX;
	value = boost::lexical_cast<std::string>(axis.x);
	if(SaveProperty(key,value)==false)
		return false;

	key = sAxeY;
	value = boost::lexical_cast<std::string>(axis.y);
	if(SaveProperty(key,value)==false)
		return false;

	key = sAxeZ;
	value = boost::lexical_cast<std::string>(axis.z);
	if(SaveProperty(key,value)==false)
		return false;

	key = sAngle;
	value = boost::lexical_cast<std::string>(angle);
	if(SaveProperty(key,value)==false)
		return false;

	return true;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::LoadColour(nsMathTools::TVector3& v3)
{
	std::string key, value;
	int cnt = GetCountProperty();

	if(cnt<CountAxes3)// кол-во компонентов в векторе
		return false;

	for( int i = 0 ; i < cnt ; i++ )
	{
		if(LoadProperty(i,key,value))
		{
			if(key==sColourRed)
				v3.x = boost::lexical_cast<float>(value.data());
			else if(key==sColourGreen)
				v3.y = boost::lexical_cast<float>(value.data());
			else if(key==sColourBlue)
				v3.z = boost::lexical_cast<float>(value.data());

			if(errno==ERANGE)
				return false;
		}
		else 
			return false;
	}
	return true;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::SaveColour(nsMathTools::TVector3& v3)
{
	std::string key, value;

	key = sColourRed;
	value = boost::lexical_cast<std::string>(v3.x);
	if(SaveProperty(key,value)==false)
		return false;

	key = sColourGreen;
	value = boost::lexical_cast<std::string>(v3.y);
	if(SaveProperty(key,value)==false)
		return false;

	key = sColourBlue;
	value = boost::lexical_cast<std::string>(v3.z);
	if(SaveProperty(key,value)==false)
		return false;

	return true;
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::EnterRoot()
{
  mXML->ResetPos();
  std::string sRoot = mXML->GetNameSection(0);
  return mXML->EnterSection(sRoot.data(), 0);
}
//------------------------------------------------------------------------------
std::string TBaseSerializerItem_XML::ReadName(int index, IXML* xml)
{
  return xml->ReadSectionAttr(index, sName );
}
//------------------------------------------------------------------------------
bool TBaseSerializerItem_XML::RemoveSection(std::string& name)
{
  if(EnterRoot()==false)
    return false;
  int cnt = mXML->GetCountSection(mType.data());
  if(cnt==0)
    return false;

  for(int i = 0 ; i < cnt ; i++)
  {
    std::string sAttrName = mXML->ReadSectionAttr(mType.data(), i, sName);
    if(name==sAttrName)
      return mXML->RemoveSection(mType.data(), i);
  }
  return false;
}
//------------------------------------------------------------------------------
std::string TBaseSerializerItem_XML::Type()
{
  return mType;
}
//------------------------------------------------------------------------------
TBaseSerializerItem_XML::TIndex* TBaseSerializerItem_XML::FindIndex(std::string name)
{
	TMapStrIndexIt fit = mMapNameIndex_m3x3.find(name);
	if( fit==mMapNameIndex_m3x3.end() )
	{
		BL_FIX_BUG();
		return NULL;
	}
	return &(fit->second);
}
//------------------------------------------------------------------------------
