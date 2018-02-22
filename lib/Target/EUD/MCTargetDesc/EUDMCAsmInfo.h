//===-- EUDMCAsmInfo.h - EUD asm properties -------------------*- C++ -*--====//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the EUDMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_EUD_MCTARGETDESC_EUDMCASMINFO_H
#define LLVM_LIB_TARGET_EUD_MCTARGETDESC_EUDMCASMINFO_H

#include "llvm/ADT/Triple.h"
#include "llvm/MC/MCAsmInfo.h"

namespace llvm {
class Target;

class EUDMCAsmInfo : public MCAsmInfo {
public:
  explicit EUDMCAsmInfo(const Triple &TT) {
    PrivateGlobalPrefix = ".L";
    WeakRefDirective = "\t.weak\t";

    UsesELFSectionDirectiveForBSS = true;
    HasSingleParameterDotFile = false;
    HasDotTypeDotSizeDirective = false;

    SupportsDebugInformation = true;
    ExceptionsType = ExceptionHandling::DwarfCFI;
    MinInstAlignment = 8;

    // the default is 4 and it only affects dwarf elf output
    // so if not set correctly, the dwarf data will be
    // messed up in random places by 4 bytes. .debug_line
    // section will be parsable, but with odd offsets and
    // line numbers, etc.
    CodePointerSize = 8;
  }
};
}

#endif
