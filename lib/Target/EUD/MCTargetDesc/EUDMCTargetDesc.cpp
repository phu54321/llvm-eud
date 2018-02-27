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

#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

extern "C" void LLVMInitializeEUDTargetMC() {
  // Nothing to register
}
