# add PORTANDROID options
if(PORTANDROID)
  set(distribution_DIR ${CFG_LIBS_DIR})
  set(import_libs_DIR ${CFG_LIBS_DIR})
  set(main_entry_source ${CFG_ENTRY_DIR}/emu_${CFG_MODULE_NAME}_main.cpp)

  # add include directories
  include_directories(${import_libs_DIR}/classicboy/include
          ${import_libs_DIR}/libBlocksRuntime/include
          ${import_libs_DIR}/libcpufeatures/include
          ${import_libs_DIR}/libemu/include
          ${import_libs_DIR}/libemuRetro/include
          ${import_libs_DIR}/libfex/include
          ${import_libs_DIR}/libpng/include
          ${import_libs_DIR}/libzip/include)

  # added imported libraries
  add_library(lib_emu STATIC IMPORTED)
  set_target_properties(lib_emu PROPERTIES IMPORTED_LOCATION
          ${import_libs_DIR}/libemu/${CFG_BUILD_TYPE}/${ANDROID_ABI}/libemu.a)

  # added imported libraries
  add_library(lib_emuRetro STATIC IMPORTED)
  set_target_properties(lib_emuRetro PROPERTIES IMPORTED_LOCATION
          ${import_libs_DIR}/libemuRetro/${CFG_BUILD_TYPE}/${ANDROID_ABI}/libemuRetro.a)
endif()


add_library(swanstation_libretro SHARED
  libretro_audio_stream.cpp
  libretro_audio_stream.h
  libretro_game_settings.cpp
  libretro_game_settings.h
  libretro_host_display.cpp
  libretro_host_display.h
  libretro_host_interface.cpp
  libretro_host_interface.h
  libretro_settings_interface.cpp
  libretro_settings_interface.h
  ${main_entry_source}
)

if(PORTANDROID)
  # no lib prefix
  set_target_properties(swanstation_libretro PROPERTIES PREFIX "")
  # rename target and change output directory
  set_target_properties(swanstation_libretro  PROPERTIES OUTPUT_NAME "libcore_${CFG_MODULE_NAME}")
  set_target_properties(swanstation_libretro
          PROPERTIES
          LIBRARY_OUTPUT_DIRECTORY
          "${distribution_DIR}/core_${CFG_MODULE_NAME}/${CFG_BUILD_TYPE}/${ANDROID_ABI}")

  #set target link libraries for PORTANDROID
  target_link_libraries(swanstation_libretro PRIVATE
          android
          log
          lib_emu
          lib_emuRetro
          core
          common
          glad
          vulkan-loader
          libretro-common
  )

else()

  target_link_libraries(swanstation_libretro PRIVATE core common glad vulkan-loader libretro-common)

  # no lib prefix
  set_target_properties(swanstation_libretro PROPERTIES PREFIX "")

  # drop in the build directory
  set_target_properties(swanstation_libretro PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(swanstation_libretro PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

  # work around .sln issue of hardcoded config directories
  if(WIN32)
    set_target_properties(swanstation_libretro PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}")
    set_target_properties(swanstation_libretro PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")
  endif()

  # for android, suffix _android
  if(ANDROID)
    set_target_properties(swanstation_libretro PROPERTIES OUTPUT_NAME "swanstation_libretro_android")
  endif()

endif()