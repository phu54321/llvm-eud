//===-- EUDInstrFormats.td - EUD Instruction Formats -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

class EUDOpClass<bits<3> val> {
  bits<3> Value = val;
}

def EUD_LD    : EUDOpClass<0x0>;
def EUD_LDX   : EUDOpClass<0x1>;
def EUD_ST    : EUDOpClass<0x2>;
def EUD_STX   : EUDOpClass<0x3>;
def EUD_ALU   : EUDOpClass<0x4>;
def EUD_JMP   : EUDOpClass<0x5>;
def EUD_ALU64 : EUDOpClass<0x7>;

class EUDSrcType<bits<1> val> {
  bits<1> Value = val;
}

def EUD_K : EUDSrcType<0x0>;
def EUD_X : EUDSrcType<0x1>;

class EUDArithOp<bits<4> val> {
  bits<4> Value = val;
}

def EUD_ADD  : EUDArithOp<0x0>;
def EUD_SUB  : EUDArithOp<0x1>;
def EUD_MUL  : EUDArithOp<0x2>;
def EUD_DIV  : EUDArithOp<0x3>;
def EUD_OR   : EUDArithOp<0x4>;
def EUD_AND  : EUDArithOp<0x5>;
def EUD_LSH  : EUDArithOp<0x6>;
def EUD_RSH  : EUDArithOp<0x7>;
def EUD_NEG  : EUDArithOp<0x8>;
def EUD_XOR  : EUDArithOp<0xa>;
def EUD_MOV  : EUDArithOp<0xb>;
def EUD_ARSH : EUDArithOp<0xc>;
def EUD_END  : EUDArithOp<0xd>;

class EUDEndDir<bits<1> val> {
  bits<1> Value = val;
}

def EUD_TO_LE : EUDSrcType<0x0>;
def EUD_TO_BE : EUDSrcType<0x1>;

class EUDJumpOp<bits<4> val> {
  bits<4> Value = val;
}

def EUD_JA   : EUDJumpOp<0x0>;
def EUD_JEQ  : EUDJumpOp<0x1>;
def EUD_JGT  : EUDJumpOp<0x2>;
def EUD_JGE  : EUDJumpOp<0x3>;
def EUD_JNE  : EUDJumpOp<0x5>;
def EUD_JSGT : EUDJumpOp<0x6>;
def EUD_JSGE : EUDJumpOp<0x7>;
def EUD_CALL : EUDJumpOp<0x8>;
def EUD_EXIT : EUDJumpOp<0x9>;
def EUD_JLT  : EUDJumpOp<0xa>;
def EUD_JLE  : EUDJumpOp<0xb>;
def EUD_JSLT : EUDJumpOp<0xc>;
def EUD_JSLE : EUDJumpOp<0xd>;

class EUDWidthModifer<bits<2> val> {
  bits<2> Value = val;
}

def EUD_W  : EUDWidthModifer<0x0>;
def EUD_H  : EUDWidthModifer<0x1>;
def EUD_B  : EUDWidthModifer<0x2>;
def EUD_DW : EUDWidthModifer<0x3>;

class EUDModeModifer<bits<3> val> {
  bits<3> Value = val;
}

def EUD_IMM  : EUDModeModifer<0x0>;
def EUD_ABS  : EUDModeModifer<0x1>;
def EUD_IND  : EUDModeModifer<0x2>;
def EUD_MEM  : EUDModeModifer<0x3>;
def EUD_XADD : EUDModeModifer<0x6>;

class InstEUD<dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {
  field bits<64> Inst;
  field bits<64> SoftFail = 0;
  let Size = 8;

  let Namespace = "EUD";
  let DecoderNamespace = "EUD";

  EUDOpClass EUDClass;
  let Inst{58-56} = EUDClass.Value;

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
  : InstEUD<outs, ins, asmstr, pattern> {
  let Inst{63-0} = 0;
  let isPseudo = 1;
}
