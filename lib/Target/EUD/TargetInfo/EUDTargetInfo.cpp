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
Target &getTheEUDbeTarget() {
  static Target TheEUDbeTarget;
  return TheEUDbeTarget;
}
} // namespace llvm

extern "C" void LLVMInitializeEUDTargetInfo() {
  RegisterTarget<Triple::eudel, /*HasJIT=*/true> X(
      getTheEUDleTarget(), "eudel", "EUD (little endian)", "EUD");
  RegisterTarget<Triple::eudeb, /*HasJIT=*/true> Y(getTheEUDbeTarget(), "eudeb",
                                                   "EUD (big endian)", "EUD");
}
