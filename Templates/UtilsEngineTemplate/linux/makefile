CC=g++-4.7
CFLAGS=-Wall -O3 -std=gnu++0x
LDFLAGS= -lpthread


#======================================================
#====================== BOOST =========================
#======================================================

CFLAGS += -I$(LibsPath)/boost_1_52_0

#thread
LOCAL_SRC_FILES := $(LibsPath)/boost_1_52_0/libs/thread/src/pthread/thread.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/thread/src/pthread/once.cpp

#signals
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/signals/src/connection.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/signals/src/named_slot_map.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/signals/src/signal_base.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/signals/src/slot.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/signals/src/trackable.cpp

#system
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/system/src/error_code.cpp

#regex
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/c_regex_traits.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/cpp_regex_traits.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/cregex.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/fileiter.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/icu.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/instances.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/posix_api.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/regex.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/regex_debug.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/regex_raw_buffer.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/regex_traits_defaults.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/static_mutex.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/usinstances.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/w32_regex_traits.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/wc_regex_traits.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/wide_posix_api.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/regex/src/winstances.cpp

#date_time
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/date_time/src/gregorian/greg_month.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/date_time/src/gregorian/greg_weekday.cpp
LOCAL_SRC_FILES += $(LibsPath)/boost_1_52_0/libs/date_time/src/gregorian/date_generators.cpp


#======================================================
#==================== Squirrel ========================
#======================================================

CFLAGS += -I$(LibsPath)/sqplus/include -I$(LibsPath)/sqplus/sqplus -I$(LibsPath)/sqplus/squirrel -I$(LibsPath)/sqplus/sqstdlib

LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqapi.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqbaselib.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqclass.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqcompiler.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqdebug.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqfuncstate.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqlexer.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqmem.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqobject.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqstate.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqtable.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/squirrel/sqvm.cpp

LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqplus/SqPlus.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqplus/SqPlusOCharBuf.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqplus/SqPlusUtf8.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqplus/SquirrelBindingsUtils.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqplus/SquirrelObject.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqplus/SquirrelVM.cpp

LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqstdlib/sqstdblob.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqstdlib/sqstdio.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqstdlib/sqstdmath.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqstdlib/sqstdrex.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqstdlib/sqstdstream.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqstdlib/sqstdstring.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqstdlib/sqstdsystem.cpp
LOCAL_SRC_FILES += $(LibsPath)/sqplus/sqstdlib/sqstdaux.cpp

#======================================================
#==================== ENGINE ==========================
#======================================================

CFLAGS += -DUTILS_ENGINE -DTARGET_LINUX

CFLAGS += -I$(SalmonEnginePath)

LOCAL_SRC_FILES += $(SalmonEnginePath)/src/Utils/DataTypes/DataTypes.cpp
LOCAL_SRC_FILES += $(SalmonEnginePath)/src/Utils/DataTypes/NewDataTypes.cpp
LOCAL_SRC_FILES += $(SalmonEnginePath)/src/Utils/FileUtils/FileUtils.cpp
LOCAL_SRC_FILES += $(SalmonEnginePath)/src/Utils/Console/Console.cpp
LOCAL_SRC_FILES += $(SalmonEnginePath)/src/Utils/SerializeInterface/SerializeInterface.cpp
LOCAL_SRC_FILES += $(SalmonEnginePath)/src/Utils/Network/Server.cpp
LOCAL_SRC_FILES += $(SalmonEnginePath)/src/Utils/Network/Network.cpp
LOCAL_SRC_FILES += $(SalmonEnginePath)/src/Utils/SimpleTimer.cpp
LOCAL_SRC_FILES += $(SalmonEnginePath)/src/Utils/ThreadUtils.cpp

#======================================================
#====================== GAME ==========================
#======================================================

CFLAGS += -I../UtilsEngineTemplate

LOCAL_SRC_FILES += ../XO-game_srv/main.cpp
LOCAL_SRC_FILES += ../XO-game_srv/linux_stuff.cpp

all:
	$(CC) $(LOCAL_SRC_FILES) $(CFLAGS) $(LDFLAGS) -o UtilsEngineTemplate