//===-- EUDTargetMachine.cpp - Define TargetMachine for EUD ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about EUD target spec.
//
//===----------------------------------------------------------------------===//

#include "EUDTargetMachine.h"
#include "EUD.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"
using namespace llvm;

extern "C" void LLVMInitializeEUDTarget() {
  // Register the target.
  RegisterTargetMachine<EUDTargetMachine> X(getTheEUDleTarget());
  RegisterTargetMachine<EUDTargetMachine> Y(getTheEUDbeTarget());
}

// DataLayout: little or big endian
static std::string computeDataLayout(const Triple &TT) {
  if (TT.getArch() == Triple::eudeb)
    return "E-m:e-p:64:64-i64:64-n32:64-S128";
  else
    return "e-m:e-p:64:64-i64:64-n32:64-S128";
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::PIC_;
  return *RM;
}

static CodeModel::Model getEffectiveCodeModel(Optional<CodeModel::Model> CM) {
  if (CM)
    return *CM;
  return CodeModel::Small;
}

EUDTargetMachine::EUDTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Optional<Reloc::Model> RM,
                                   Optional<CodeModel::Model> CM,
                                   CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT), TT, CPU, FS, Options,
                        getEffectiveRelocModel(RM), getEffectiveCodeModel(CM),
                        OL),
      TLOF(make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}
namespace {
// EUD Code Generator Pass Configuration Options.
class EUDPassConfig : public TargetPassConfig {
public:
  EUDPassConfig(EUDTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  EUDTargetMachine &getEUDTargetMachine() const {
    return getTM<EUDTargetMachine>();
  }

  bool addInstSelector() override;
};
}

TargetPassConfig *EUDTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new EUDPassConfig(*this, PM);
}

// Install an instruction selector pass using
// the ISelDag to gen EUD code.
bool EUDPassConfig::addInstSelector() {
  addPass(createEUDISelDag(getEUDTargetMachine()));

  return false;
}
