//===-- EUDRegisterInfo.cpp - EUD Register Information ----------*- C++ -*-===//
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

#include "EUDRegisterInfo.h"
#include "EUD.h"
#include "EUDSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_REGINFO_TARGET_DESC
#include "EUDGenRegisterInfo.inc"
using namespace llvm;

EUDRegisterInfo::EUDRegisterInfo()
    : EUDGenRegisterInfo(EUD::R0) {}

const MCPhysReg *
EUDRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_SaveList;
}

BitVector EUDRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(EUD::R10); // R10 is read only frame pointer
  Reserved.set(EUD::R11); // R11 is pseudo stack pointer
  return Reserved;
}

static void WarnSize(int Offset, MachineFunction &MF, DebugLoc& DL)
{
  if (Offset <= -512) {
      const Function &F = MF.getFunction();
      DiagnosticInfoUnsupported DiagStackSize(F,
          "Looks like the EUD stack limit of 512 bytes is exceeded. "
          "Please move large on stack variables into EUD per-cpu array map.\n",
          DL);
      F.getContext().diagnose(DiagStackSize);
  }
}

void EUDRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  unsigned i = 0;
  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  DebugLoc DL = MI.getDebugLoc();

  if (!DL)
    /* try harder to get some debug loc */
    for (auto &I : MBB)
      if (I.getDebugLoc()) {
        DL = I.getDebugLoc();
        break;
      }

  while (!MI.getOperand(i).isFI()) {
    ++i;
    assert(i < MI.getNumOperands() && "Instr doesn't have FrameIndex operand!");
  }

  unsigned FrameReg = getFrameRegister(MF);
  int FrameIndex = MI.getOperand(i).getIndex();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

  if (MI.getOpcode() == EUD::MOV_rr) {
    int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex);

    WarnSize(Offset, MF, DL);
    MI.getOperand(i).ChangeToRegister(FrameReg, false);
    unsigned reg = MI.getOperand(i - 1).getReg();
    BuildMI(MBB, ++II, DL, TII.get(EUD::ADD_ri), reg)
        .addReg(reg)
        .addImm(Offset);
    return;
  }

  int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex) +
               MI.getOperand(i + 1).getImm();

  if (!isInt<32>(Offset))
    llvm_unreachable("bug in frame offset");

  WarnSize(Offset, MF, DL);

  if (MI.getOpcode() == EUD::FI_ri) {
    // architecture does not really support FI_ri, replace it with
    //    MOV_rr <target_reg>, frame_reg
    //    ADD_ri <target_reg>, imm
    unsigned reg = MI.getOperand(i - 1).getReg();

    BuildMI(MBB, ++II, DL, TII.get(EUD::MOV_rr), reg)
        .addReg(FrameReg);
    BuildMI(MBB, II, DL, TII.get(EUD::ADD_ri), reg)
        .addReg(reg)
        .addImm(Offset);

    // Remove FI_ri instruction
    MI.eraseFromParent();
  } else {
    MI.getOperand(i).ChangeToRegister(FrameReg, false);
    MI.getOperand(i + 1).ChangeToImmediate(Offset);
  }
}

unsigned EUDRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return EUD::R10;
}
