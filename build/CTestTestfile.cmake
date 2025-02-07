# CMake generated Testfile for 
# Source directory: E:/VisualStudio/projects/3D Game Engine
# Build directory: E:/VisualStudio/projects/3D Game Engine/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[MyTest]=] "E:/VisualStudio/projects/3D Game Engine/build/Debug/3DGameEngineTests.exe")
  set_tests_properties([=[MyTest]=] PROPERTIES  _BACKTRACE_TRIPLES "E:/VisualStudio/projects/3D Game Engine/CMakeLists.txt;97;add_test;E:/VisualStudio/projects/3D Game Engine/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[MyTest]=] "E:/VisualStudio/projects/3D Game Engine/build/Release/3DGameEngineTests.exe")
  set_tests_properties([=[MyTest]=] PROPERTIES  _BACKTRACE_TRIPLES "E:/VisualStudio/projects/3D Game Engine/CMakeLists.txt;97;add_test;E:/VisualStudio/projects/3D Game Engine/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[MyTest]=] "E:/VisualStudio/projects/3D Game Engine/build/MinSizeRel/3DGameEngineTests.exe")
  set_tests_properties([=[MyTest]=] PROPERTIES  _BACKTRACE_TRIPLES "E:/VisualStudio/projects/3D Game Engine/CMakeLists.txt;97;add_test;E:/VisualStudio/projects/3D Game Engine/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[MyTest]=] "E:/VisualStudio/projects/3D Game Engine/build/RelWithDebInfo/3DGameEngineTests.exe")
  set_tests_properties([=[MyTest]=] PROPERTIES  _BACKTRACE_TRIPLES "E:/VisualStudio/projects/3D Game Engine/CMakeLists.txt;97;add_test;E:/VisualStudio/projects/3D Game Engine/CMakeLists.txt;0;")
else()
  add_test([=[MyTest]=] NOT_AVAILABLE)
endif()
