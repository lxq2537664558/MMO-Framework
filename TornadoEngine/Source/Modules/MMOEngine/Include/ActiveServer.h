/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ACTIVESERVER_H
#define ACTIVESERVER_H

#include "BaseServer.h"

namespace nsMMOEngine
{
  class DllExport TActiveServer : public TBaseServer
  {

  public:
    TActiveServer();
    virtual ~TActiveServer();
    
    virtual void ConnectUp(unsigned int ip, unsigned short port,  
                           void* pLogin, int sizeLogin, void* pPassword, int sizePassword,
                           unsigned char subNet = 0) = 0;

	protected:

	private:

  };
}

#endif