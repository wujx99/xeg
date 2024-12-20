set(base_SRCS
  AsyncLogging.cpp
  CurrentThread.cpp
  FileUtil.cpp
  LogFile.cpp
  Logging.cpp
  LogStream.cpp
  Timestamp.cpp
  Thread.cpp
  )

  
add_library(xeg_base ${base_SRCS})
target_link_libraries(xeg_base pthread rt)

#add_library(xeg_base_cpp11 ${base_SRCS})
#target_link_libraries(xeg_base_cpp11 pthread rt)
#set_target_properties(xeg_base_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS xeg_base DESTINATION lib)
#install(TARGETS xeg_base_cpp11 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/xeg/base)


if(XEG_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()