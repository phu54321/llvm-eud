//===-- EUDTargetInfo.cpp - EUD Target Implementation ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "EUD.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

namespace llvm {
Target &getTheEUDleTarget() {
  static Target TheEUDleTarget;
  return TheEUDleTarget;
}
} // namespace llvm

extern "C" void LLVMInitializeEUDTargetInfo() {
  RegisterTarget<Triple::eud, /*HasJIT=*/true> X(
    getTheEUDleTarget(), "eud", "EUD (little endian)", "EUD");
}
