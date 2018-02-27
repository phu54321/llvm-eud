//===-- EUDMCTargetDesc.h - EUD Target Descriptions -------------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_EUD_MCTARGETDESC_EUDMCTARGETDESC_H
#define LLVM_LIB_TARGET_EUD_MCTARGETDESC_EUDMCTARGETDESC_H

#include "llvm/Support/TargetRegistry.h"

namespace llvm {
Target &getEUDTarget();
}

#endif
