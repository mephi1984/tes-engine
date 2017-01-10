
# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.



#ENGINE_PATH = $(SalmonEnginePathCygwin)

#BOOST_PATH = $(LibsPathCygwin)/boost_1_52_0

#OGG_PATH = $(LibsPathCygwin)/libogg-1.3.0

#VORBIS_PATH = $(LibsPathCygwin)/libvorbis-1.3.2

#SQUIRREL_PATH = $(LibsPathCygwin)/sqplus

#LIBPNG_PATH = $(LibsPathCygwin)/libpng_1.4.1_android

#LIBJPEG_PATH = $(LibsPathCygwin)/jpeg-9

#ZIP_PATH = $(LibsPathCygwin)/julienr-libzip-android/jni

set(BOOST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../boost_1_63_0)

set(BOOST_GIL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/boost-gil-extension)

set(ZIP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/julienr-libzip-android/jni)

set(LIBPNG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/libpng_1.4.1_android)

set(LIBJPEG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/jpeg-9")

set(SQUIRREL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/sqplus)

set(OGG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/libogg-1.3.0)

set(VORBIS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/libvorbis-1.3.2)


#include_directories(ZIP_PATH)

add_library(zip
	${ZIP_PATH}/zip_add.c
	${ZIP_PATH}/zip_add_dir.c
	${ZIP_PATH}/zip_close.c
	${ZIP_PATH}/zip_delete.c
	${ZIP_PATH}/zip_dirent.c
	${ZIP_PATH}/zip_entry_free.c
	${ZIP_PATH}/zip_entry_new.c
	${ZIP_PATH}/zip_err_str.c
	${ZIP_PATH}/zip_error.c
	${ZIP_PATH}/zip_error_clear.c
	${ZIP_PATH}/zip_error_get.c
	${ZIP_PATH}/zip_error_get_sys_type.c
	${ZIP_PATH}/zip_error_strerror.c
	${ZIP_PATH}/zip_error_to_str.c
	${ZIP_PATH}/zip_fclose.c
	${ZIP_PATH}/zip_file_error_clear.c
	${ZIP_PATH}/zip_file_error_get.c
	${ZIP_PATH}/zip_file_get_offset.c
	${ZIP_PATH}/zip_file_strerror.c
	${ZIP_PATH}/zip_filerange_crc.c
	${ZIP_PATH}/zip_fopen.c
	${ZIP_PATH}/zip_fopen_index.c
	${ZIP_PATH}/zip_fread.c
	${ZIP_PATH}/zip_free.c
	${ZIP_PATH}/zip_get_archive_comment.c
	${ZIP_PATH}/zip_get_archive_flag.c
	${ZIP_PATH}/zip_get_file_comment.c
	${ZIP_PATH}/zip_get_num_files.c
	${ZIP_PATH}/zip_get_name.c
	${ZIP_PATH}/zip_memdup.c
	${ZIP_PATH}/zip_name_locate.c
	${ZIP_PATH}/zip_new.c
	${ZIP_PATH}/zip_open.c
	${ZIP_PATH}/zip_rename.c
	${ZIP_PATH}/zip_replace.c
	${ZIP_PATH}/zip_set_archive_comment.c
	${ZIP_PATH}/zip_set_archive_flag.c
	${ZIP_PATH}/zip_set_file_comment.c
	${ZIP_PATH}/zip_source_buffer.c
	${ZIP_PATH}/zip_source_file.c
	${ZIP_PATH}/zip_source_filep.c
	${ZIP_PATH}/zip_source_free.c
	${ZIP_PATH}/zip_source_function.c
	${ZIP_PATH}/zip_source_zip.c
	${ZIP_PATH}/zip_set_name.c
	${ZIP_PATH}/zip_stat.c
	${ZIP_PATH}/zip_stat_index.c
	${ZIP_PATH}/zip_stat_init.c
	${ZIP_PATH}/zip_strerror.c
	${ZIP_PATH}/zip_unchange.c
	${ZIP_PATH}/zip_unchange_all.c
	${ZIP_PATH}/zip_unchange_archive.c
	${ZIP_PATH}/zip_unchange_data.c
	)
	
#include_directories(LIBPNG_PATH)

add_library(png
	${LIBPNG_PATH}/png.c
	${LIBPNG_PATH}/pngerror.c
	${LIBPNG_PATH}/pngget.c
	${LIBPNG_PATH}/pngmem.c
	${LIBPNG_PATH}/pngpread.c
	${LIBPNG_PATH}/pngread.c
	${LIBPNG_PATH}/pngrio.c
	${LIBPNG_PATH}/pngrtran.c
	${LIBPNG_PATH}/pngrutil.c
	${LIBPNG_PATH}/pngset.c
	${LIBPNG_PATH}/pngtrans.c
	${LIBPNG_PATH}/pngwio.c
	${LIBPNG_PATH}/pngwrite.c
	${LIBPNG_PATH}/pngwtran.c
	${LIBPNG_PATH}/pngwutil.c 
	)

include_directories(${LIBJPEG_PATH}/vc10)

add_library(jpeg
	${LIBJPEG_PATH}/jaricom.c
	${LIBJPEG_PATH}/jcapimin.c
	${LIBJPEG_PATH}/jcapistd.c
	${LIBJPEG_PATH}/jcarith.c
	${LIBJPEG_PATH}/jccoefct.c
	${LIBJPEG_PATH}/jccolor.c
	${LIBJPEG_PATH}/jcdctmgr.c
	${LIBJPEG_PATH}/jchuff.c
	${LIBJPEG_PATH}/jcinit.c
	${LIBJPEG_PATH}/jcmainct.c
	${LIBJPEG_PATH}/jcmarker.c
	${LIBJPEG_PATH}/jcmaster.c
	${LIBJPEG_PATH}/jcomapi.c
	${LIBJPEG_PATH}/jcparam.c
	${LIBJPEG_PATH}/jcprepct.c
	${LIBJPEG_PATH}/jcsample.c
	${LIBJPEG_PATH}/jctrans.c
	${LIBJPEG_PATH}/jdapimin.c
	${LIBJPEG_PATH}/jdapistd.c
	${LIBJPEG_PATH}/jdarith.c
	${LIBJPEG_PATH}/jdatadst.c
	${LIBJPEG_PATH}/jdatasrc.c
	${LIBJPEG_PATH}/jdcoefct.c
	${LIBJPEG_PATH}/jdcolor.c
	${LIBJPEG_PATH}/jddctmgr.c
	${LIBJPEG_PATH}/jdhuff.c
	${LIBJPEG_PATH}/jdinput.c
	${LIBJPEG_PATH}/jdmainct.c
	${LIBJPEG_PATH}/jdmarker.c
	${LIBJPEG_PATH}/jdmaster.c
	${LIBJPEG_PATH}/jdmerge.c
	${LIBJPEG_PATH}/jdpostct.c
	${LIBJPEG_PATH}/jdsample.c
	${LIBJPEG_PATH}/jdtrans.c
	${LIBJPEG_PATH}/jerror.c
	${LIBJPEG_PATH}/jfdctflt.c
	${LIBJPEG_PATH}/jfdctfst.c
	${LIBJPEG_PATH}/jfdctint.c
	${LIBJPEG_PATH}/jidctflt.c
	${LIBJPEG_PATH}/jidctfst.c
	${LIBJPEG_PATH}/jidctint.c
	${LIBJPEG_PATH}/jmemmgr.c
	${LIBJPEG_PATH}/jmemnobs.c
	${LIBJPEG_PATH}/jquant1.c
	${LIBJPEG_PATH}/jquant2.c
	${LIBJPEG_PATH}/jutils.c
	)



#LOCAL_PATH       := $(VORBIS_PATH)/lib
#LOCAL_C_INCLUDES := $(VORBIS_PATH)/include
#LOCAL_C_INCLUDES += $(VORBIS_PATH)/include/vorbis
#LOCAL_C_INCLUDES += $(VORBIS_PATH)/lib/src
#LOCAL_C_INCLUDES += $(VORBIS_PATH)/lib/books
#LOCAL_C_INCLUDES += $(VORBIS_PATH)/lib/modes
#LOCAL_C_INCLUDES += $(OGG_PATH)/include
#LOCAL_C_INCLUDES += $(OGG_PATH)/include/ogg

include_directories(${OGG_PATH}/include)
include_directories(${VORBIS_PATH}/include)

add_library(ogg
	${OGG_PATH}/src/framing.c 
	${OGG_PATH}/src/bitwise.c
	)

add_library(vorbis
	${VORBIS_PATH}/lib/mdct.c
	${VORBIS_PATH}/lib/smallft.c
	${VORBIS_PATH}/lib/block.c
	${VORBIS_PATH}/lib/envelope.c
	${VORBIS_PATH}/lib/window.c
	${VORBIS_PATH}/lib/lsp.c
	${VORBIS_PATH}/lib/lpc.c
	${VORBIS_PATH}/lib/analysis.c
	${VORBIS_PATH}/lib/synthesis.c
	${VORBIS_PATH}/lib/psy.c
	${VORBIS_PATH}/lib/info.c
	${VORBIS_PATH}/lib/floor1.c
	${VORBIS_PATH}/lib/floor0.c
	${VORBIS_PATH}/lib/res0.c
	${VORBIS_PATH}/lib/mapping0.c
	${VORBIS_PATH}/lib/registry.c
	${VORBIS_PATH}/lib/codebook.c
	${VORBIS_PATH}/lib/sharedbook.c
	${VORBIS_PATH}/lib/lookup.c
	${VORBIS_PATH}/lib/bitrate.c
	${VORBIS_PATH}/lib/vorbisfile.c
	)


include_directories(${BOOST_PATH})

add_library(boost
	${BOOST_PATH}/libs/thread/src/pthread/thread.cpp


#thread
${BOOST_PATH}/libs/thread/src/pthread/thread.cpp
${BOOST_PATH}/libs/thread/src/pthread/once.cpp

#signals
${BOOST_PATH}/libs/signals/src/connection.cpp
${BOOST_PATH}/libs/signals/src/named_slot_map.cpp
${BOOST_PATH}/libs/signals/src/signal_base.cpp
${BOOST_PATH}/libs/signals/src/slot.cpp
${BOOST_PATH}/libs/signals/src/trackable.cpp

#system
${BOOST_PATH}/libs/system/src/error_code.cpp

#regex
${BOOST_PATH}/libs/regex/src/c_regex_traits.cpp
${BOOST_PATH}/libs/regex/src/cpp_regex_traits.cpp
${BOOST_PATH}/libs/regex/src/cregex.cpp
${BOOST_PATH}/libs/regex/src/fileiter.cpp
${BOOST_PATH}/libs/regex/src/icu.cpp
${BOOST_PATH}/libs/regex/src/instances.cpp
${BOOST_PATH}/libs/regex/src/posix_api.cpp
${BOOST_PATH}/libs/regex/src/regex.cpp
${BOOST_PATH}/libs/regex/src/regex_debug.cpp
${BOOST_PATH}/libs/regex/src/regex_raw_buffer.cpp
${BOOST_PATH}/libs/regex/src/regex_traits_defaults.cpp
${BOOST_PATH}/libs/regex/src/static_mutex.cpp
${BOOST_PATH}/libs/regex/src/usinstances.cpp
${BOOST_PATH}/libs/regex/src/w32_regex_traits.cpp
${BOOST_PATH}/libs/regex/src/wc_regex_traits.cpp
${BOOST_PATH}/libs/regex/src/wide_posix_api.cpp
${BOOST_PATH}/libs/regex/src/winstances.cpp

#date_time
${BOOST_PATH}/libs/date_time/src/gregorian/greg_month.cpp
${BOOST_PATH}/libs/date_time/src/gregorian/greg_weekday.cpp
${BOOST_PATH}/libs/date_time/src/gregorian/date_generators.cpp
)



find_library( # Sets the name of the path variable.
              z-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              z )
			  
find_library( # Sets the name of the path variable.
              GLESv2-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              GLESv2 )
find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
			  
			  
			  

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../)


include_directories(${BOOST_PATH})
include_directories(${LIBPNG_PATH})
include_directories(${LIBJPEG_PATH})
include_directories(${ZIP_PATH})

include_directories(${BOOST_GIL_PATH})



add_definitions(-DTARGET_ANDROID)


add_library( # Sets the name of the library.
             engine

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.

             #testcpp.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/DataTypes/DataTypes.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/DataTypes/NewDataTypes.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/FileUtils/FileUtils.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/JniApi/JniApi.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/JniApi/JniApplication.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/Console/Console.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/SerializeInterface/SerializeInterface.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/Network/Network.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/PngHelper.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/JpegHelper.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/TgaLoader.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/SimpleTimer.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Utils/ThreadUtils.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TextureManager/SalmonTexture.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/ShaderManager/ShaderManager.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/FrameManager/FrameManager.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/LightManager/LightManager.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/SoundManager/SoundManagerAndroid.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/SoundManager/SoundManagerDataTypes.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/FontManager/FontManager.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/ScriptManager/ScriptManager.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/SmartValueManager/SmartValueManager.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/GUIManager/GUIManager.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/GUIManager/ButtonWidget.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/GUIManager/WidgetXmlParsers.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/HalibutAnimation/HalibutAnimation.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/ModelManager/ModelManager.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/ModelManager/NewModelManager.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/SimpleLand/SimpleLand.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/PhysicsManager/PhysicsManager.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Render/RenderMisc.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Render/RenderParams.cpp

             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Render/SalmonRender/BackgroundCubemap.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Render/SalmonRender/SalmonRenderInterface.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Render/SalmonRender/SalmonRenderGLES20.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Render/SalmonRender/SalmonRenderAndroid.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Render/SalmonRender/Cameras.cpp

             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/Animation/SalmonAnimation.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/SalmonEngineAndroid.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../../src/SalmonEngineInterface.cpp
             )


target_link_libraries(engine
${z-lib}
${log-lib}
${GLESv2-lib}
png
zip
boost
jpeg
ogg
vorbis
)
