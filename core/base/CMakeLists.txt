# Copyright (C) 1995-2019, Rene Brun and Fons Rademakers.
# All rights reserved.
#
# For the licensing terms see $ROOTSYS/LICENSE.
# For the list of contributors see $ROOTSYS/README/CREDITS.

############################################################################
# CMakeLists.txt file for building ROOT core/base package
############################################################################

if(MSVC AND MSVC_VERSION GREATER_EQUAL 1925 AND MSVC_VERSION LESS 1929)
  # FIXME: since Visual Studio v16.5.0 the /O2 optimization flag makes most of the roofit/roostats tests failing
  # Try to re-enable /O2 after the upgrade of llvm & clang, or when upgrading Visual Studio
  string(REPLACE "-O2" "-O1 -Oi" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
  string(REPLACE "-O2" "-O1 -Oi" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()

set(BASE_HEADERS
  ROOT/RFloat16.hxx
  ROOT/TErrorDefaultHandler.hxx
  ROOT/TExecutorCRTP.hxx
  ROOT/TSequentialExecutor.hxx
  ROOT/StringConv.hxx
  Buttons.h
  Bytes.h
  Byteswap.h
  KeySymbols.h
  MessageTypes.h
  Riostream.h
  Rtypes.h
  TApplication.h
  TAtt3D.h
  TAttAxis.h
  TAttBBox2D.h
  TAttBBox.h
  TAttFill.h
  TAttLine.h
  TAttMarker.h
  TAttPad.h
  TAttText.h
  TBase64.h
  TBenchmark.h
  TBuffer3D.h
  TBuffer3DTypes.h
  TBuffer.h
  TColor.h
  TColorGradient.h
  TDatime.h
  TDirectory.h
  TEnv.h
  TException.h
  TExec.h
  TFileCollection.h
  TFileInfo.h
  TFolder.h
  TInetAddress.h
  TMacro.h
  TMathBase.h
  TMD5.h
  TMemberInspector.h
  TMessageHandler.h
  TNamed.h
  TNotifyLink.h
  TObject.h
  TObjString.h
  TParameter.h
  TPluginManager.h
  TPoint.h
  TPRegexp.h
  TProcessID.h
  TProcessUUID.h
  TQClass.h
  TQCommand.h
  TQConnection.h
  TQObject.h
  TRedirectOutputGuard.h
  TRefCnt.h
  TRef.h
  TRegexp.h
  TRemoteObject.h
  TROOT.h
  TRootIOCtor.h
  TStopwatch.h
  TStorage.h
  TString.h
  TStringLong.h
  TStyle.h
  TSysEvtHandler.h
  TSystemDirectory.h
  TSystemFile.h
  TSystem.h
  TTask.h
  TThreadSlots.h
  TTime.h
  TTimer.h
  TTimeStamp.h
  TUri.h
  TUrl.h
  TUUID.h
  TVersionCheck.h
  TVirtualAuth.h
  TVirtualFFT.h
  TVirtualGL.h
  TVirtualMonitoring.h
  TVirtualMapFile.h
  TVirtualMutex.h
  TVirtualPadEditor.h
  TVirtualPad.h
  TVirtualPadPainter.h
  TVirtualPerfStats.h
  TVirtualPS.h
  TVirtualQConnection.h
  TVirtualRWMutex.h
  TVirtualTableInterface.h
  TVirtualViewer3D.h
  TVirtualX.h
)

set(BASE_SOURCES
  src/Match.cxx
  src/String.cxx
  src/Stringio.cxx
  src/TApplication.cxx
  src/TAtt3D.cxx
  src/TAttAxis.cxx
  src/TAttBBox2D.cxx
  src/TAttBBox.cxx
  src/TAttFill.cxx
  src/TAttLine.cxx
  src/TAttMarker.cxx
  src/TAttPad.cxx
  src/TAttText.cxx
  src/TBase64.cxx
  src/TBenchmark.cxx
  src/TBuffer3D.cxx
  src/TBuffer.cxx
  src/TColor.cxx
  src/TColorGradient.cxx
  src/TDatime.cxx
  src/TDirectory.cxx
  src/TEnv.cxx
  src/TErrorDefaultHandler.cxx
  src/TException.cxx
  src/TExec.cxx
  src/TFileCollection.cxx
  src/TFileInfo.cxx
  src/TFolder.cxx
  src/TInetAddress.cxx
  src/TListOfTypes.cxx
  src/TListOfTypes.h
  src/TMacro.cxx
  src/TMathBase.cxx
  src/TMD5.cxx
  src/TMemberInspector.cxx
  src/TMessageHandler.cxx
  src/TNamed.cxx
  src/TObject.cxx
  src/TObjString.cxx
  src/TParameter.cxx
  src/TPluginManager.cxx
  src/TPRegexp.cxx
  src/TProcessID.cxx
  src/TProcessUUID.cxx
  src/TQCommand.cxx
  src/TQConnection.cxx
  src/TQObject.cxx
  src/TRefCnt.cxx
  src/TRef.cxx
  src/TRegexp.cxx
  src/TRemoteObject.cxx
  src/TROOT.cxx
  src/TStopwatch.cxx
  src/TStorage.cxx
  src/TString.cxx
  src/TStringLong.cxx
  src/TStyle.cxx
  src/TSysEvtHandler.cxx
  src/TSystem.cxx
  src/TSystemDirectory.cxx
  src/TSystemFile.cxx
  src/TTask.cxx
  src/TTime.cxx
  src/TTimer.cxx
  src/TTimeStamp.cxx
  src/TUri.cxx
  src/TUrl.cxx
  src/TUUID.cxx
  src/TVirtualFFT.cxx
  src/TVirtualGL.cxx
  src/TVirtualMonitoring.cxx
  src/TVirtualMutex.cxx
  src/TVirtualPad.cxx
  src/TVirtualPadEditor.cxx
  src/TVirtualPadPainter.cxx
  src/TVirtualPerfStats.cxx
  src/TVirtualPS.cxx
  src/TVirtualViewer3D.cxx
  src/TVirtualX.cxx
)

# only here complete list of headers can be propogated to parent cmake file
set_property(TARGET Core APPEND PROPERTY DICT_HEADERS ${BASE_HEADERS})

target_sources(Core PRIVATE ${BASE_SOURCES})

target_include_directories(Core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
)

if(PCRE2_FOUND)
  target_link_libraries(Core PRIVATE PCRE2::PCRE2)
  set_source_files_properties(src/TPRegexp.cxx
    TARGET_DIRECTORY Core
    PROPERTIES COMPILE_DEFINITIONS USE_PCRE2)
else()
  target_link_libraries(Core PRIVATE PCRE::PCRE)
endif()

ROOT_INSTALL_HEADERS(${BASE_HEADER_DIRS})

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/man)

if(NOT MSVC)
#---addRootC++CmdMan-------------------------------------------------------------------------
generateManual(rootMan ${CMAKE_CURRENT_SOURCE_DIR}/src/root-argparse.py ${CMAKE_BINARY_DIR}/man/root.1)
generateManual(haddMan ${CMAKE_SOURCE_DIR}/main/src/hadd-argparse.py ${CMAKE_BINARY_DIR}/man/hadd.1)
generateManual(rootclingMan ${CMAKE_SOURCE_DIR}/core/dictgen/src/rootcling-argparse.py ${CMAKE_BINARY_DIR}/man/rootcling.1)

#---addRootPyCmdMan---------------------------------------------------------------------------
#generateManual(rootbrowseMan ${CMAKE_SOURCE_DIR}/main/python/rootbrowse.py ${CMAKE_BINARY_DIR}/man/rootbrowse.1)
#generateManual(rootcpMan ${CMAKE_SOURCE_DIR}/main/python/rootcp.py ${CMAKE_BINARY_DIR}/man/rootcp.1)
#generateManual(rootdrawtreeMan ${CMAKE_SOURCE_DIR}/main/python/rootdrawtree.py ${CMAKE_BINARY_DIR}/man/rootdrawtree.1)
#generateManual(rooteventselectorMan ${CMAKE_SOURCE_DIR}/main/python/rooteventselector.py ${CMAKE_BINARY_DIR}/man/rooteventselector.1)
#generateManual(rootlsMan ${CMAKE_SOURCE_DIR}/main/python/rootls.py ${CMAKE_BINARY_DIR}/man/rootls.1)
#generateManual(rootmkdirMan ${CMAKE_SOURCE_DIR}/main/python/rootmkdir.py ${CMAKE_BINARY_DIR}/man/rootmkdir.1)
#generateManual(rootmvMan ${CMAKE_SOURCE_DIR}/main/python/rootmv.py ${CMAKE_BINARY_DIR}/man/rootmv.1)
#generateManual(rootprintMan ${CMAKE_SOURCE_DIR}/main/python/rootprint.py ${CMAKE_BINARY_DIR}/man/rootprint.1)
#generateManual(rootrmMan ${CMAKE_SOURCE_DIR}/main/python/rootrm.py ${CMAKE_BINARY_DIR}/man/rootrm.1)
#generateManual(rootslimtreeMan ${CMAKE_SOURCE_DIR}/main/python/rootslimtree.py ${CMAKE_BINARY_DIR}/man/rootslimtree.1)
endif()

ROOT_ADD_TEST_SUBDIRECTORY(test)
