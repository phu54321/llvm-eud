//===-- EUDMCTargetDesc.cpp - EUD Target Descriptions ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides EUD specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/EUDMCTargetDesc.h"
#include "EUD.h"
#include "InstPrinter/EUDInstPrinter.h"
#include "MCTargetDesc/EUDMCAsmInfo.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "EUDGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "EUDGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "EUDGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createEUDMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitEUDMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createEUDMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitEUDMCRegisterInfo(X, EUD::R11 /* RAReg doesn't exist */);
  return X;
}

static MCSubtargetInfo *createEUDMCSubtargetInfo(const Triple &TT,
                                                 StringRef CPU, StringRef FS) {
  return createEUDMCSubtargetInfoImpl(TT, CPU, FS);
}

static MCStreamer *createEUDMCStreamer(const Triple &T, MCContext &Ctx,
                                       std::unique_ptr<MCAsmBackend> &&MAB,
                                       raw_pwrite_stream &OS,
                                       std::unique_ptr<MCCodeEmitter> &&Emitter,
                                       bool RelaxAll) {
  return createELFStreamer(Ctx, std::move(MAB), OS, std::move(Emitter),
                           RelaxAll);
}

static MCInstPrinter *createEUDMCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new EUDInstPrinter(MAI, MII, MRI);
  return nullptr;
}

namespace {

class EUDMCInstrAnalysis : public MCInstrAnalysis {
public:
  explicit EUDMCInstrAnalysis(const MCInstrInfo *Info)
      : MCInstrAnalysis(Info) {}

  bool evaluateBranch(const MCInst &Inst, uint64_t Addr, uint64_t Size,
                      uint64_t &Target) const override {
    // The target is the 3rd operand of cond inst and the 1st of uncond inst.
    int16_t Imm;
    if (isConditionalBranch(Inst)) {
      Imm = Inst.getOperand(2).getImm();
    } else if (isUnconditionalBranch(Inst))
      Imm = Inst.getOperand(0).getImm();
    else
      return false;

    Target = Addr + Size + Imm * Size;
    return true;
  }
};

} // end anonymous namespace

static MCInstrAnalysis *createEUDInstrAnalysis(const MCInstrInfo *Info) {
  return new EUDMCInstrAnalysis(Info);
}

extern "C" void LLVMInitializeEUDTargetMC() {
  for (Target *T :
    {&getTheEUDleTarget()}) {
    // Register the MC asm info.
    RegisterMCAsmInfo<EUDMCAsmInfo> X(*T);

    // Register the MC instruction info.
    TargetRegistry::RegisterMCInstrInfo(*T, createEUDMCInstrInfo);

    // Register the MC register info.
    TargetRegistry::RegisterMCRegInfo(*T, createEUDMCRegisterInfo);

    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(*T,
                                            createEUDMCSubtargetInfo);

    // Register the object streamer
    TargetRegistry::RegisterELFStreamer(*T, createEUDMCStreamer);

    // Register the MCInstPrinter.
    TargetRegistry::RegisterMCInstPrinter(*T, createEUDMCInstPrinter);

    // Register the MC instruction analyzer.
    TargetRegistry::RegisterMCInstrAnalysis(*T, createEUDInstrAnalysis);
  }

  // Register the MC code emitter
  TargetRegistry::RegisterMCCodeEmitter(getTheEUDleTarget(),
                                        createEUDMCCodeEmitter);

  // Register the ASM Backend
  TargetRegistry::RegisterMCAsmBackend(getTheEUDleTarget(),
                                       createEUDAsmBackend);
}
