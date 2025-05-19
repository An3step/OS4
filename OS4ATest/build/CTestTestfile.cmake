# CMake generated Testfile for 
# Source directory: C:/Users/Антон/Desktop/OS4/OS4_Test
# Build directory: C:/Users/Антон/Desktop/OS4/OS4_Test/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(ReceiverTests "C:/Users/Антон/Desktop/OS4/OS4_Test/build/Debug/ReceiverTests.exe")
  set_tests_properties(ReceiverTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Антон/Desktop/OS4/OS4_Test/CMakeLists.txt;25;add_test;C:/Users/Антон/Desktop/OS4/OS4_Test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(ReceiverTests "C:/Users/Антон/Desktop/OS4/OS4_Test/build/Release/ReceiverTests.exe")
  set_tests_properties(ReceiverTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Антон/Desktop/OS4/OS4_Test/CMakeLists.txt;25;add_test;C:/Users/Антон/Desktop/OS4/OS4_Test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(ReceiverTests "C:/Users/Антон/Desktop/OS4/OS4_Test/build/MinSizeRel/ReceiverTests.exe")
  set_tests_properties(ReceiverTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Антон/Desktop/OS4/OS4_Test/CMakeLists.txt;25;add_test;C:/Users/Антон/Desktop/OS4/OS4_Test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(ReceiverTests "C:/Users/Антон/Desktop/OS4/OS4_Test/build/RelWithDebInfo/ReceiverTests.exe")
  set_tests_properties(ReceiverTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Антон/Desktop/OS4/OS4_Test/CMakeLists.txt;25;add_test;C:/Users/Антон/Desktop/OS4/OS4_Test/CMakeLists.txt;0;")
else()
  add_test(ReceiverTests NOT_AVAILABLE)
endif()
