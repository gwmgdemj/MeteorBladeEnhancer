#pragma once

#define INITGUID

#include <Dshow.h>
#include <Control.h>
#include "..\Common\Wrapper.h"
#include "..\Common\Logging.h"


void genericDshowQueryInterface(REFIID CalledID, LPVOID * ppvObj);
//#define genericQueryInterface(a,b) genericDshowQueryInterface(a,b);
extern AddressLookupTable<void> ProxyAddressLookupTable;

#include "IFilterGraph.h"
#include "IGraphBuilder.h"
#include "IVideoWindow.h"