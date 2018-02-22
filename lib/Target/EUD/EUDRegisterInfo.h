//===-- EUDRegisterInfo.h - EUD Register Information Impl -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the EUD implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_EUD_EUDREGISTERINFO_H
#define LLVM_LIB_TARGET_EUD_EUDREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "EUDGenRegisterInfo.inc"

namespace llvm {

struct EUDRegisterInfo : public EUDGenRegisterInfo {

  EUDRegisterInfo();

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool enableMultipleCopyHints() const override { return true; }

  void eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  unsigned getFrameRegister(const MachineFunction &MF) const override;
};
}

#endif
