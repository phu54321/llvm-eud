//===-- EUD.td - Describe the EUD Target Machine -----------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

include "EUDRegisterInfo.td"
include "EUDCallingConv.td"
include "EUDInstrInfo.td"

def EUDInstrInfo : InstrInfo;

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;
def : Proc<"v1", []>;
def : Proc<"v2", []>;
def : Proc<"probe", []>;

def DummyFeature : SubtargetFeature<"dummy", "isDummyMode",
                                    "true", "unused feature">;

def EUDInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

def EUDAsmParser : AsmParser {
  bit HasMnemonicFirst = 0;
}

def EUDAsmParserVariant : AsmParserVariant {
  int Variant = 0;
  string Name = "EUD";
  string BreakCharacters = ".";
  string TokenizingCharacters = "#()[]=:.<>!+*";
}

def EUD : Target {
  let InstructionSet = EUDInstrInfo;
  let AssemblyWriters = [EUDInstPrinter];
  let AssemblyParsers = [EUDAsmParser];
  let AssemblyParserVariants = [EUDAsmParserVariant];
}
