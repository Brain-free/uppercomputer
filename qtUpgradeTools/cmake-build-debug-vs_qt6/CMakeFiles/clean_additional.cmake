# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\qtUpgradeTools_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\qtUpgradeTools_autogen.dir\\ParseCache.txt"
  "qtUpgradeTools_autogen"
  )
endif()
