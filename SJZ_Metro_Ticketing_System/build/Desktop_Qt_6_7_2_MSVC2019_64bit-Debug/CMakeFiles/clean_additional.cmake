# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\SJZ_Metro_Ticketing_System_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SJZ_Metro_Ticketing_System_autogen.dir\\ParseCache.txt"
  "SJZ_Metro_Ticketing_System_autogen"
  )
endif()
