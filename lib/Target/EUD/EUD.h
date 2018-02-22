//===-- EUD.h - Top-level interface for EUD representation ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_EUD_EUD_H
#define LLVM_LIB_TARGET_EUD_EUD_H

#include "MCTargetDesc/EUDMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class EUDTargetMachine;

FunctionPass *createEUDISelDag(EUDTargetMachine &TM);
}

#endif
