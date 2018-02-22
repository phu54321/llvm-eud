//===-- EUDSubtarget.cpp - EUD Subtarget Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the EUD specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "EUDSubtarget.h"
#include "EUD.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "eud-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "EUDGenSubtargetInfo.inc"

void EUDSubtarget::anchor() {}

EUDSubtarget &EUDSubtarget::initializeSubtargetDependencies(StringRef CPU,
                                                            StringRef FS) {
  initializeEnvironment();
  initSubtargetFeatures(CPU, FS);
  return *this;
}

void EUDSubtarget::initializeEnvironment() {
  HasJmpExt = false;
}

void EUDSubtarget::initSubtargetFeatures(StringRef CPU, StringRef FS) {
  if (CPU == "probe")
    CPU = "generic";
  if (CPU == "generic" || CPU == "v1")
    return;
  if (CPU == "v2") {
    HasJmpExt = true;
    return;
  }
}

EUDSubtarget::EUDSubtarget(const Triple &TT, const std::string &CPU,
                           const std::string &FS, const TargetMachine &TM)
    : EUDGenSubtargetInfo(TT, CPU, FS), InstrInfo(),
      FrameLowering(initializeSubtargetDependencies(CPU, FS)),
      TLInfo(TM, *this) {}
