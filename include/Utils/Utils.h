#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
/* 
This code combines additional routines (such as console/log, exceptions, math utils, file utils) for engine to use
*/

#include <string>
#include <map>
#include <vector>
#include <stack>
#include "boost/shared_array.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/foreach.hpp"

#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/signal.hpp"


#include "include/Utils/Console/Console.h"
#include "include/Utils/ErrorTypes/ErrorTypes.h"
#include "../GlobalConst.h"
#include "include/Utils/FileUtils/FileUtils.h"
#include "include/Utils/SerializeInterface/SerializeInterface.h"

#include "include/Utils/BindableVar.h"
#include "include/Utils/SimpleTimer.h"
#include "include/Utils/ThreadUtils.h"
#include "include/Utils/Network/Network.h"
#include "include/Utils/Network/Server.h"

#ifdef TARGET_WIN32
#include "WinApi/WinApi.h"
#endif

#ifdef TARGET_ANDROID
#include "JniApi/JniApi.h"
#endif

#ifdef TARGET_IOS
#include "IosApi/IosApi.h"
#endif


#endif