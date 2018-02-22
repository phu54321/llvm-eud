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

#include "llvm/Config/config.h"
#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class StringRef;
class Target;
class Triple;
class raw_ostream;
class raw_pwrite_stream;

Target &getTheEUDleTarget();
Target &getTheEUDbeTarget();

MCCodeEmitter *createEUDMCCodeEmitter(const MCInstrInfo &MCII,
                                      const MCRegisterInfo &MRI,
                                      MCContext &Ctx);
MCCodeEmitter *createEUDbeMCCodeEmitter(const MCInstrInfo &MCII,
                                        const MCRegisterInfo &MRI,
                                        MCContext &Ctx);

MCAsmBackend *createEUDAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                  const MCRegisterInfo &MRI,
                                  const MCTargetOptions &Options);
MCAsmBackend *createEUDbeAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);

std::unique_ptr<MCObjectWriter> createEUDELFObjectWriter(raw_pwrite_stream &OS,
                                                         uint8_t OSABI,
                                                         bool IsLittleEndian);
}

// Defines symbolic names for EUD registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "EUDGenRegisterInfo.inc"

// Defines symbolic names for the EUD instructions.
//
#define GET_INSTRINFO_ENUM
#include "EUDGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "EUDGenSubtargetInfo.inc"

#endif
