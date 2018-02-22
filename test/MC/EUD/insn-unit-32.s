# RUN: llvm-mc -triple eud -filetype=obj -o %t %s
# RUN: llvm-objdump -d -r %t | FileCheck %s

// ======== EUD_ALU Class ========
  w1 = -w1    // EUD_NEG
  w0 += w1    // EUD_ADD  | EUD_X
  w1 -= w2    // EUD_SUB  | EUD_X
  w2 *= w3    // EUD_MUL  | EUD_X
  w3 /= w4    // EUD_DIV  | EUD_X
// CHECK: 84 01 00 00 00 00 00 00      w1 = -w1
// CHECK: 0c 10 00 00 00 00 00 00      w0 += w1
// CHECK: 1c 21 00 00 00 00 00 00      w1 -= w2
// CHECK: 2c 32 00 00 00 00 00 00      w2 *= w3
// CHECK: 3c 43 00 00 00 00 00 00      w3 /= w4

  w4 |= w5    // EUD_OR   | EUD_X
  w5 &= w6    // EUD_AND  | EUD_X
  w6 <<= w7   // EUD_LSH  | EUD_X
  w7 >>= w8   // EUD_RSH  | EUD_X
  w8 ^= w9    // EUD_XOR  | EUD_X
  w9 = w10    // EUD_MOV  | EUD_X
  w10 s>>= w0 // EUD_ARSH | EUD_X
// CHECK: 4c 54 00 00 00 00 00 00      w4 |= w5
// CHECK: 5c 65 00 00 00 00 00 00      w5 &= w6
// CHECK: 6c 76 00 00 00 00 00 00      w6 <<= w7
// CHECK: 7c 87 00 00 00 00 00 00      w7 >>= w8
// CHECK: ac 98 00 00 00 00 00 00      w8 ^= w9
// CHECK: bc a9 00 00 00 00 00 00      w9 = w10
// CHECK: cc 0a 00 00 00 00 00 00      w10 s>>= w0

  w0 += 1           // EUD_ADD  | EUD_K
  w1 -= 0x1         // EUD_SUB  | EUD_K
  w2 *= -4          // EUD_MUL  | EUD_K
  w3 /= 5           // EUD_DIV  | EUD_K
// CHECK: 04 00 00 00 01 00 00 00      w0 += 1
// CHECK: 14 01 00 00 01 00 00 00      w1 -= 1
// CHECK: 24 02 00 00 fc ff ff ff      w2 *= -4
// CHECK: 34 03 00 00 05 00 00 00      w3 /= 5

  w4 |= 0xff        // EUD_OR   | EUD_K
  w5 &= 0xFF        // EUD_AND  | EUD_K
  w6 <<= 63         // EUD_LSH  | EUD_K
  w7 >>= 32         // EUD_RSH  | EUD_K
  w8 ^= 0           // EUD_XOR  | EUD_K
  w9 = 1            // EUD_MOV  | EUD_K
  w9 = 0xffffffff   // EUD_MOV  | EUD_K
  w10 s>>= 64       // EUD_ARSH | EUD_K
// CHECK: 44 04 00 00 ff 00 00 00      w4 |= 255
// CHECK: 54 05 00 00 ff 00 00 00      w5 &= 255
// CHECK: 64 06 00 00 3f 00 00 00      w6 <<= 63
// CHECK: 74 07 00 00 20 00 00 00      w7 >>= 32
// CHECK: a4 08 00 00 00 00 00 00      w8 ^= 0
// CHECK: b4 09 00 00 01 00 00 00      w9 = 1
// CHECK: b4 09 00 00 ff ff ff ff      w9 = -1
// CHECK: c4 0a 00 00 40 00 00 00      w10 s>>= 64
