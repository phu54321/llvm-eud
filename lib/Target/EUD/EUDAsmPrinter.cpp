//===-- EUDAsmPrinter.cpp - EUD LLVM assembly writer ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the EUD assembly language.
//
//===----------------------------------------------------------------------===//

#include "EUD.h"
#include "EUDInstrInfo.h"
#include "EUDMCInstLower.h"
#include "EUDTargetMachine.h"
#include "InstPrinter/EUDInstPrinter.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

namespace {
class EUDAsmPrinter : public AsmPrinter {
public:
  explicit EUDAsmPrinter(TargetMachine &TM,
                         std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "EUD Assembly Printer"; }
  void printOperand(const MachineInstr *MI, int OpNum, raw_ostream &O);
  bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                       unsigned AsmVariant, const char *ExtraCode,
                       raw_ostream &O) override;
  bool PrintAsmMemoryOperand(const MachineInstr *MI, unsigned OpNum,
                             unsigned AsmVariant, const char *ExtraCode,
                             raw_ostream &O) override;

  void EmitInstruction(const MachineInstr *MI) override;
};
} // namespace

void EUDAsmPrinter::printOperand(const MachineInstr *MI, int OpNum,
                                 raw_ostream &O) {
  const MachineOperand &MO = MI->getOperand(OpNum);

  switch (MO.getType()) {
  case MachineOperand::MO_Register:
    O << EUDInstPrinter::getRegisterName(MO.getReg());
    break;

  case MachineOperand::MO_Immediate:
    O << MO.getImm();
    break;

  case MachineOperand::MO_MachineBasicBlock:
    O << *MO.getMBB()->getSymbol();
    break;

  case MachineOperand::MO_GlobalAddress:
    O << *getSymbol(MO.getGlobal());
    break;

  case MachineOperand::MO_BlockAddress: {
    MCSymbol *BA = GetBlockAddressSymbol(MO.getBlockAddress());
    O << BA->getName();
    break;
  }

  case MachineOperand::MO_ExternalSymbol:
    O << *GetExternalSymbolSymbol(MO.getSymbolName());
    break;

  case MachineOperand::MO_JumpTableIndex:
  case MachineOperand::MO_ConstantPoolIndex:
  default:
    llvm_unreachable("<unknown operand type>");
  }
}

bool EUDAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                    unsigned /*AsmVariant*/,
                                    const char *ExtraCode, raw_ostream &O) {
  if (ExtraCode && ExtraCode[0])
    return true; // EUD does not have special modifiers

  printOperand(MI, OpNo, O);
  return false;
}

bool EUDAsmPrinter::PrintAsmMemoryOperand(const MachineInstr *MI,
                                          unsigned OpNum, unsigned AsmVariant,
                                          const char *ExtraCode,
                                          raw_ostream &O) {
  assert(OpNum + 1 < MI->getNumOperands() && "Insufficient operands");
  const MachineOperand &BaseMO = MI->getOperand(OpNum);
  const MachineOperand &OffsetMO = MI->getOperand(OpNum + 1);
  assert(BaseMO.isReg() && "Unexpected base pointer for inline asm memory operand.");
  assert(OffsetMO.isImm() && "Unexpected offset for inline asm memory operand.");
  int Offset = OffsetMO.getImm();

  if (ExtraCode)
    return true; // Unknown modifier.

  if (Offset < 0)
    O << "(" << EUDInstPrinter::getRegisterName(BaseMO.getReg()) << " - " << -Offset << ")";
  else
    O << "(" << EUDInstPrinter::getRegisterName(BaseMO.getReg()) << " + " << Offset << ")";

  return false;
}

void EUDAsmPrinter::EmitInstruction(const MachineInstr *MI) {

  EUDMCInstLower MCInstLowering(OutContext, *this);

  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

// Force static initialization.
extern "C" void LLVMInitializeEUDAsmPrinter() {
  RegisterAsmPrinter<EUDAsmPrinter> X(getTheEUDleTarget());
}
