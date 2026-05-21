# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appBMSApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appBMSApp_autogen.dir\\ParseCache.txt"
  "appBMSApp_autogen"
  )
endif()
