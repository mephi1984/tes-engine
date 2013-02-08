#Engine and libs path for Android projects


ENGINE_PATH = $(SalmonEnginePathCygwin)

BOOST_PATH = $(LibsPathCygwin)/boost_1_52_0

OGG_PATH = $(LibsPathCygwin)/libogg-1.3.0

VORBIS_PATH = $(LibsPathCygwin)/libvorbis-1.3.2

SQUIRREL_PATH = $(LibsPathCygwin)/sqplus

LIBPNG_PATH = $(LibsPathCygwin)/libpng_1.4.1_android

ZIP_PATH = $(LibsPathCygwin)/julienr-libzip-android/jni

#ENGINE_PATH = $(SalmonEnginePath)

#BOOST_PATH = $(LibsPath)/boost_1_52_0

#OGG_PATH = $(LibsPath)/libogg-1.3.0

#VORBIS_PATH = $(LibsPath)/libvorbis-1.3.2

#SQUIRREL_PATH = $(LibsPath)/sqplus

#LIBPNG_PATH = $(LibsPath)/libpng_1.4.1_android

#ZIP_PATH = $(LibsPath)/julienr-libzip-android/jni


#================== ZIP =================

LOCAL_PATH := $(ZIP_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE    := zip

LOCAL_SRC_FILES :=\
	zip_add.c \
	zip_add_dir.c \
	zip_close.c \
	zip_delete.c \
	zip_dirent.c \
	zip_entry_free.c \
	zip_entry_new.c \
	zip_err_str.c \
	zip_error.c \
	zip_error_clear.c \
	zip_error_get.c \
	zip_error_get_sys_type.c \
	zip_error_strerror.c \
	zip_error_to_str.c \
	zip_fclose.c \
	zip_file_error_clear.c \
	zip_file_error_get.c \
	zip_file_get_offset.c \
	zip_file_strerror.c \
	zip_filerange_crc.c \
	zip_fopen.c \
	zip_fopen_index.c \
	zip_fread.c \
	zip_free.c \
	zip_get_archive_comment.c \
	zip_get_archive_flag.c \
	zip_get_file_comment.c \
	zip_get_num_files.c \
	zip_get_name.c \
	zip_memdup.c \
	zip_name_locate.c \
	zip_new.c \
	zip_open.c \
	zip_rename.c \
	zip_replace.c \
	zip_set_archive_comment.c \
	zip_set_archive_flag.c \
	zip_set_file_comment.c \
	zip_source_buffer.c \
	zip_source_file.c \
	zip_source_filep.c \
	zip_source_free.c \
	zip_source_function.c \
	zip_source_zip.c \
	zip_set_name.c \
	zip_stat.c \
	zip_stat_index.c \
	zip_stat_init.c \
	zip_strerror.c \
	zip_unchange.c \
	zip_unchange_all.c \
	zip_unchange_archive.c \
	zip_unchange_data.c

LOCAL_LDLIBS := -lz

#debug
#LOCAL_CFLAGS := -g -ggdb -O0
#LOCAL_LDLIBS    +=  -g -ggdb


include $(BUILD_STATIC_LIBRARY)

#================== PNG =================

LOCAL_PATH := $(LIBPNG_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE    := png_lib
LOCAL_SRC_FILES :=\
	png.c \
	pngerror.c \
	pngget.c \
	pngmem.c \
	pngpread.c \
	pngread.c \
	pngrio.c \
	pngrtran.c \
	pngrutil.c \
	pngset.c \
	pngtrans.c \
	pngwio.c \
	pngwrite.c \
	pngwtran.c \
	pngwutil.c 
	
LOCAL_LDLIBS := -lz

#debug
#LOCAL_CFLAGS := -g -ggdb -O0
#LOCAL_LDLIBS    +=  -g -ggdb

include $(BUILD_STATIC_LIBRARY)


#================== SQUIRREL =================


include $(CLEAR_VARS)

LOCAL_MODULE     := squirrel
LOCAL_PATH       := $(SQUIRREL_PATH)
LOCAL_C_INCLUDES := $(SQUIRREL_PATH)/include $(SQUIRREL_PATH)/sqplus/sqplus $(SQUIRREL_PATH)/squirrel $(SQUIRREL_PATH)/sqstdlib
LOCAL_SRC_FILES  := squirrel/sqapi.cpp \
					squirrel/sqbaselib.cpp \
					squirrel/sqclass.cpp \
					squirrel/sqcompiler.cpp \
					squirrel/sqdebug.cpp \
					squirrel/sqfuncstate.cpp \
					squirrel/sqlexer.cpp \
					squirrel/sqmem.cpp \
					squirrel/sqobject.cpp \
					squirrel/sqstate.cpp \
					squirrel/sqtable.cpp \
					squirrel/sqvm.cpp \
					sqplus/SqPlus.cpp \
					sqplus/SqPlusOCharBuf.cpp \
					sqplus/SqPlusUtf8.cpp \
					sqplus/SquirrelBindingsUtils.cpp \
					sqplus/SquirrelObject.cpp \
					sqplus/SquirrelVM.cpp \
					sqstdlib/sqstdblob.cpp \
					sqstdlib/sqstdio.cpp \
					sqstdlib/sqstdmath.cpp \
					sqstdlib/sqstdrex.cpp \
					sqstdlib/sqstdstream.cpp \
					sqstdlib/sqstdstring.cpp \
					sqstdlib/sqstdsystem.cpp \
					sqstdlib/sqstdaux.cpp \


#debug
#LOCAL_CFLAGS := -g -ggdb -O0
#LOCAL_LDLIBS    :=  -g -ggdb

include $(BUILD_STATIC_LIBRARY)


#================== OGG / VORBIS / VORBISFILE ===================

include $(CLEAR_VARS)

LOCAL_MODULE := vorbis

LOCAL_PATH       := $(VORBIS_PATH)/lib
LOCAL_C_INCLUDES := $(VORBIS_PATH)/include
LOCAL_C_INCLUDES += $(VORBIS_PATH)/include/vorbis
LOCAL_C_INCLUDES += $(VORBIS_PATH)/lib/src
LOCAL_C_INCLUDES += $(VORBIS_PATH)/lib/books
LOCAL_C_INCLUDES += $(VORBIS_PATH)/lib/modes
LOCAL_C_INCLUDES += $(OGG_PATH)/include
LOCAL_C_INCLUDES += $(OGG_PATH)/include/ogg
LOCAL_SRC_FILES := ../../libogg-1.3.0/src/framing.c ../../libogg-1.3.0/src/bitwise.c
LOCAL_SRC_FILES += mdct.c smallft.c block.c envelope.c window.c lsp.c \
			lpc.c analysis.c synthesis.c psy.c info.c \
			floor1.c floor0.c\
			res0.c mapping0.c registry.c codebook.c sharedbook.c\
			lookup.c bitrate.c
LOCAL_SRC_FILES += vorbisfile.c

#debug
#LOCAL_CFLAGS     := -g -ggdb -O0
#LOCAL_LDLIBS    :=  -g -ggdb

include $(BUILD_STATIC_LIBRARY)


#================== BOOST ====================

LOCAL_PATH := $(BOOST_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE    := boost
LOCAL_C_INCLUDES := $(BOOST_PATH)

LOCAL_CFLAGS := -std=gnu++11 --std=c++11


#thread
LOCAL_SRC_FILES := /libs/thread/src/pthread/thread.cpp
LOCAL_SRC_FILES += /libs/thread/src/pthread/once.cpp

#signals
LOCAL_SRC_FILES += /libs/signals/src/connection.cpp
LOCAL_SRC_FILES += /libs/signals/src/named_slot_map.cpp
LOCAL_SRC_FILES += /libs/signals/src/signal_base.cpp
LOCAL_SRC_FILES += /libs/signals/src/slot.cpp
LOCAL_SRC_FILES += /libs/signals/src/trackable.cpp

#system
LOCAL_SRC_FILES += /libs/system/src/error_code.cpp

#regex
LOCAL_SRC_FILES += /libs/regex/src/c_regex_traits.cpp
LOCAL_SRC_FILES += /libs/regex/src/cpp_regex_traits.cpp
LOCAL_SRC_FILES += /libs/regex/src/cregex.cpp
LOCAL_SRC_FILES += /libs/regex/src/fileiter.cpp
LOCAL_SRC_FILES += /libs/regex/src/icu.cpp
LOCAL_SRC_FILES += /libs/regex/src/instances.cpp
LOCAL_SRC_FILES += /libs/regex/src/posix_api.cpp
LOCAL_SRC_FILES += /libs/regex/src/regex.cpp
LOCAL_SRC_FILES += /libs/regex/src/regex_debug.cpp
LOCAL_SRC_FILES += /libs/regex/src/regex_raw_buffer.cpp
LOCAL_SRC_FILES += /libs/regex/src/regex_traits_defaults.cpp
LOCAL_SRC_FILES += /libs/regex/src/static_mutex.cpp
LOCAL_SRC_FILES += /libs/regex/src/usinstances.cpp
LOCAL_SRC_FILES += /libs/regex/src/w32_regex_traits.cpp
LOCAL_SRC_FILES += /libs/regex/src/wc_regex_traits.cpp
LOCAL_SRC_FILES += /libs/regex/src/wide_posix_api.cpp
LOCAL_SRC_FILES += /libs/regex/src/winstances.cpp

#date_time
LOCAL_SRC_FILES += /libs/date_time/src/gregorian/greg_month.cpp
LOCAL_SRC_FILES += /libs/date_time/src/gregorian/greg_weekday.cpp
LOCAL_SRC_FILES += /libs/date_time/src/gregorian/date_generators.cpp

#asio
#nothing

LOCAL_LDLIBS     := -llog -Wl

#debug
#LOCAL_CFLAGS += -g -ggdb -O0
#LOCAL_LDLIBS     += -g -ggdb 


include $(BUILD_STATIC_LIBRARY)


#============= SALMON ENGINE ================

LOCAL_PATH := $(ENGINE_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE    := SalmonEngine


LOCAL_CFLAGS := -DTARGET_ANDROID -std=gnu++11 --std=c++11

LOCAL_STATIC_LIBRARIES := boost
LOCAL_STATIC_LIBRARIES += ogg
LOCAL_STATIC_LIBRARIES += vorbis
LOCAL_STATIC_LIBRARIES += squirrel
LOCAL_STATIC_LIBRARIES += png_lib
LOCAL_STATIC_LIBRARIES += zip
LOCAL_SHARED_LIBRARIES := gnustl_shared
LOCAL_C_INCLUDES := $(BOOST_PATH)
LOCAL_C_INCLUDES += $(VORBIS_PATH)/include
LOCAL_C_INCLUDES += $(VORBIS_PATH)/include/vorbis
LOCAL_C_INCLUDES += $(VORBIS_PATH)/lib
LOCAL_C_INCLUDES += $(VORBIS_PATH)/lib/books
LOCAL_C_INCLUDES += $(VORBIS_PATH)/lib/modes
LOCAL_C_INCLUDES += $(OGG_PATH)/include
LOCAL_C_INCLUDES += $(OGG_PATH)/include/ogg
LOCAL_C_INCLUDES += $(SQUIRREL_PATH)/include
LOCAL_C_INCLUDES += $(SQUIRREL_PATH)/sqplus
LOCAL_C_INCLUDES += $(SQUIRREL_PATH)/sqstdlib
LOCAL_C_INCLUDES += $(SQUIRREL_PATH)/squirrel
LOCAL_C_INCLUDES += $(LIBPNG_PATH)
LOCAL_C_INCLUDES += $(ZIP_PATH)
LOCAL_SRC_FILES := src/Utils/DataTypes/DataTypes.cpp
LOCAL_SRC_FILES += src/Utils/DataTypes/NewDataTypes.cpp
LOCAL_SRC_FILES += src/Utils/FileUtils/FileUtils.cpp
LOCAL_SRC_FILES += src/Utils/JniApi/JniApi.cpp
LOCAL_SRC_FILES += src/Utils/JniApi/JniApplication.cpp
LOCAL_SRC_FILES += src/Utils/Console/Console.cpp
LOCAL_SRC_FILES += src/Utils/SerializeInterface/SerializeInterface.cpp
LOCAL_SRC_FILES += src/Utils/PngHelper.cpp
LOCAL_SRC_FILES += src/Utils/SimpleTimer.cpp
LOCAL_SRC_FILES += src/Utils/ThreadUtils.cpp
LOCAL_SRC_FILES += src/TextureManager/SalmonTexture.cpp
LOCAL_SRC_FILES += src/ShaderManager/ShaderManager.cpp
LOCAL_SRC_FILES += src/FrameManager/FrameManager.cpp
LOCAL_SRC_FILES += src/LightManager/LightManager.cpp
LOCAL_SRC_FILES += src/SoundManager/SoundManagerAndroid.cpp
LOCAL_SRC_FILES += src/SoundManager/SoundManagerDataTypes.cpp
LOCAL_SRC_FILES += src/FontManager/FontManager.cpp
LOCAL_SRC_FILES += src/ScriptManager/ScriptManager.cpp
LOCAL_SRC_FILES += src/SmartValueManager/SmartValueManager.cpp

LOCAL_SRC_FILES += src/GUIManager/GUIManager.cpp
LOCAL_SRC_FILES += src/GUIManager/ButtonWidget.cpp
LOCAL_SRC_FILES += src/GUIManager/WidgetXmlParsers.cpp

LOCAL_SRC_FILES += src/HalibutAnimation/HalibutAnimation.cpp

LOCAL_SRC_FILES += src/ModelManager/ModelManager.cpp
LOCAL_SRC_FILES += src/ModelManager/NewModelManager.cpp
LOCAL_SRC_FILES += src/SimpleLand/SimpleLand.cpp
LOCAL_SRC_FILES += src/PhysicsManager/PhysicsManager.cpp

LOCAL_SRC_FILES += src/Render/RenderInterface.cpp
LOCAL_SRC_FILES += src/Render/RenderMisc.cpp
LOCAL_SRC_FILES += src/Render/RenderParams.cpp

LOCAL_SRC_FILES += src/Render/SalmonRender/BackgroundCubemap.cpp
LOCAL_SRC_FILES += src/Render/SalmonRender/SalmonRenderInterface.cpp
LOCAL_SRC_FILES += src/Render/SalmonRender/SalmonRenderGLES20.cpp
LOCAL_SRC_FILES += src/Render/SalmonRender/SalmonRenderAndroid.cpp
LOCAL_SRC_FILES += src/Render/SalmonRender/Cameras.cpp

LOCAL_SRC_FILES += src/Animation/SalmonAnimation.cpp
LOCAL_SRC_FILES += src/SalmonEngineAndroid.cpp
LOCAL_SRC_FILES += src/SalmonEngineInterface.cpp
LOCAL_LDLIBS    := -lGLESv2

LOCAL_LDLIBS    += -llog -Wl -lz

#debug
#LOCAL_CFLAGS += -g -ggdb -O0
#LOCAL_LDLIBS    += -g -ggdb

#no -s cause no need stripping!!!!

include $(BUILD_SHARED_LIBRARY)
