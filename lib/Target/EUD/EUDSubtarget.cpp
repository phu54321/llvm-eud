//===-- EUDTargetMachine.cpp - Define TargetMachine for the EUD -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the EUD specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#include "EUDTargetMachine.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

const llvm::SubtargetFeatureKV EUDSubTypeKV[] = {
  { "eud", "Select the eud processor", { }, { } }
};

static const llvm::SubtargetInfoKV EUDProcSchedModels[] = {
  { "eud", &MCSchedModel::GetDefaultSchedModel() }
};

EUDSubtarget::EUDSubtarget(const TargetMachine& TM, const Triple &TT) :
  TargetSubtargetInfo(
    TT, "eud", "eud", None, makeArrayRef(EUDSubTypeKV, 1),EUDProcSchedModels,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr),
  TLInfo(TM)
{}


