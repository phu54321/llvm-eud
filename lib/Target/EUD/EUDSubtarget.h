//===-- EUDSubtarget.h - Define Subtarget for the EUD -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the EUD specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_EUD_EUDSUBTARGET_H
#define LLVM_LIB_TARGET_EUD_EUDSUBTARGET_H

#include "EUDFrameLowering.h"
#include "EUDISelLowering.h"
#include "EUDInstrInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

#define GET_SUBTARGETINFO_HEADER
#include "EUDGenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class EUDSubtarget : public EUDGenSubtargetInfo {
  virtual void anchor();
  EUDInstrInfo InstrInfo;
  EUDFrameLowering FrameLowering;
  EUDTargetLowering TLInfo;
  SelectionDAGTargetInfo TSInfo;

private:
  void initializeEnvironment();
  void initSubtargetFeatures(StringRef CPU, StringRef FS);
  bool probeJmpExt();

protected:
  // unused
  bool isDummyMode;

  // whether the cpu supports jmp ext
  bool HasJmpExt;

public:
  // This constructor initializes the data members to match that
  // of the specified triple.
  EUDSubtarget(const Triple &TT, const std::string &CPU, const std::string &FS,
               const TargetMachine &TM);

  EUDSubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS);

  // ParseSubtargetFeatures - Parses features string setting specified
  // subtarget options.  Definition of function is auto generated by tblgen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);
  bool getHasJmpExt() const { return HasJmpExt; }

  const EUDInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const EUDFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const EUDTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }
  const TargetRegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
};
} // End llvm namespace

#endif
