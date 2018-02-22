//===-- EUDFrameLowering.cpp - EUD Frame Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the EUD implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "EUDFrameLowering.h"
#include "EUDInstrInfo.h"
#include "EUDSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

bool EUDFrameLowering::hasFP(const MachineFunction &MF) const { return true; }

void EUDFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {}

void EUDFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {}

void EUDFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                            BitVector &SavedRegs,
                                            RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  SavedRegs.reset(EUD::R6);
  SavedRegs.reset(EUD::R7);
  SavedRegs.reset(EUD::R8);
  SavedRegs.reset(EUD::R9);
}
