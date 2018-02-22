//===-- EUDELFObjectWriter.cpp - EUD ELF Writer ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/EUDMCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdint>

using namespace llvm;

namespace {

class EUDELFObjectWriter : public MCELFObjectTargetWriter {
public:
  EUDELFObjectWriter(uint8_t OSABI);
  ~EUDELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};

} // end anonymous namespace

EUDELFObjectWriter::EUDELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit*/ true, OSABI, ELF::EM_BPF,
                              /*HasRelocationAddend*/ false) {}

unsigned EUDELFObjectWriter::getRelocType(MCContext &Ctx, const MCValue &Target,
                                          const MCFixup &Fixup,
                                          bool IsPCRel) const {
  // determine the type of the relocation
  switch ((unsigned)Fixup.getKind()) {
  default:
    llvm_unreachable("invalid fixup kind!");
  case FK_SecRel_8:
    return ELF::R_BPF_64_64;
  case FK_PCRel_4:
  case FK_SecRel_4:
    return ELF::R_BPF_64_32;
  case FK_Data_8:
    return ELF::R_BPF_64_64;
  case FK_Data_4:
    return ELF::R_BPF_64_32;
  }
}

std::unique_ptr<MCObjectWriter>
llvm::createEUDELFObjectWriter(raw_pwrite_stream &OS, uint8_t OSABI,
                               bool IsLittleEndian) {
  return createELFObjectWriter(llvm::make_unique<EUDELFObjectWriter>(OSABI), OS,
                               IsLittleEndian);
}
