# RUN: llvm-mc -triple eud -filetype=obj -o %t %s
# RUN: llvm-objdump -d -r %t | FileCheck %s

// ======== EUD_LD Class ========
// Some extra whitespaces are deliberately added to test the parser.

  r0 = * (u8 *)skb[0]    // EUD_LD | EUD_ABS | EUD_B
  r0 = *(u16 *)skb[2]    // EUD_LD | EUD_ABS | EUD_H
  r0 = * (u32*)skb[4]    // EUD_LD | EUD_ABS | EUD_W
// CHECK: 30 00 00 00 00 00 00 00 	r0 = *(u8 *)skb[0]
// CHECK: 28 00 00 00 02 00 00 00 	r0 = *(u16 *)skb[2]
// CHECK: 20 00 00 00 04 00 00 00 	r0 = *(u32 *)skb[4]

  r0 = * (u8 *)skb[r0]   // EUD_LD | EUD_IND | EUD_B
  r0 = *  (u16 *)skb[r1] // EUD_LD | EUD_IND | EUD_H
  r0 = *(u32 *)skb[r2]   // EUD_LD | EUD_IND | EUD_W
// CHECK: 50 00 00 00 00 00 00 00 	r0 = *(u8 *)skb[r0]
// CHECK: 48 10 00 00 00 00 00 00 	r0 = *(u16 *)skb[r1]
// CHECK: 40 20 00 00 00 00 00 00 	r0 = *(u32 *)skb[r2]

  r9 = 0xffffffff ll     // EUD_LD | EUD_DW | EUD_IMM
  r9 = 8589934591 ll     // EUD_LD | EUD_DW | EUD_IMM
  r9 = 0x1ffffffff ll    // EUD_LD | EUD_DW | EUD_IMM
  r9 = dummy_map  ll     // EUD_LD | EUD_DW | EUD_IMM
// CHECK: 18 09 00 00 ff ff ff ff 00 00 00 00 00 00 00 00 	r9 = 4294967295 ll
// CHECK: 18 09 00 00 ff ff ff ff 00 00 00 00 01 00 00 00 	r9 = 8589934591 ll
// CHECK: 18 09 00 00 ff ff ff ff 00 00 00 00 01 00 00 00 	r9 = 8589934591 ll
// CHECK: 18 09 00 00 00 00 00 00 00 00 00 00 00 00 00 00 	r9 = 0 ll
// CHECK: 0000000000000060:  R_EUD_64_64	dummy_map

// ======== EUD_LDX Class ========
  r5 = *(u8 *)(r0 + 0)   // EUD_LDX | EUD_B
  r6 = *(u16 *)(r1 + 8)  // EUD_LDX | EUD_H
  r7 = *(u32 *)(r2 + 16) // EUD_LDX | EUD_W
  r8 = *(u64 *)(r3 - 30) // EUD_LDX | EUD_DW
// CHECK: 71 05 00 00 00 00 00 00 	r5 = *(u8 *)(r0 + 0)
// CHECK: 69 16 08 00 00 00 00 00 	r6 = *(u16 *)(r1 + 8)
// CHECK: 61 27 10 00 00 00 00 00 	r7 = *(u32 *)(r2 + 16)
// CHECK: 79 38 e2 ff 00 00 00 00 	r8 = *(u64 *)(r3 - 30)

// ======== EUD_STX Class ========
  *(u8 *)(r0 + 0) = r7    // EUD_STX | EUD_B
  *(u16 *)(r1 + 8) = r8   // EUD_STX | EUD_H
  *(u32 *)(r2 + 16) = r9  // EUD_STX | EUD_W
  *(u64 *)(r3 - 30) = r10 // EUD_STX | EUD_DW
// CHECK: 73 70 00 00 00 00 00 00 	*(u8 *)(r0 + 0) = r7
// CHECK: 6b 81 08 00 00 00 00 00 	*(u16 *)(r1 + 8) = r8
// CHECK: 63 92 10 00 00 00 00 00 	*(u32 *)(r2 + 16) = r9
// CHECK: 7b a3 e2 ff 00 00 00 00 	*(u64 *)(r3 - 30) = r10

  lock *(u32 *)(r2 + 16) += r9  // EUD_STX | EUD_W | EUD_XADD
  lock *(u64 *)(r3 - 30) += r10 // EUD_STX | EUD_DW | EUD_XADD
// CHECK: c3 92 10 00 00 00 00 00 	lock *(u32 *)(r2 + 16) += r9
// CHECK: db a3 e2 ff 00 00 00 00 	lock *(u64 *)(r3 - 30) += r10

// ======== EUD_JMP Class ========
  goto Llabel0               // EUD_JA
  call 1                     // EUD_CALL
  exit                       // EUD_EXIT
// CHECK: 05 00 1a 00 00 00 00 00 	goto +26
// CHECK: 85 00 00 00 01 00 00 00 	call 1
// CHECK: 95 00 00 00 00 00 00 00 	exit

  if r0 == r1 goto Llabel0   // EUD_JEQ  | EUD_X
  if r3 != r4 goto Llabel0   // EUD_JNE  | EUD_X
// CHECK: 1d 10 17 00 00 00 00 00 	if r0 == r1 goto +23
// CHECK: 5d 43 16 00 00 00 00 00 	if r3 != r4 goto +22

  if r1 > r2 goto Llabel0    // EUD_JGT  | EUD_X
  if r2 >= r3 goto Llabel0   // EUD_JGE  | EUD_X
  if r4 s> r5 goto Llabel0   // EUD_JSGT | EUD_X
  if r5 s>= r6 goto Llabel0  // EUD_JSGE | EUD_X
// CHECK: 2d 21 15 00 00 00 00 00 	if r1 > r2 goto +21
// CHECK: 3d 32 14 00 00 00 00 00 	if r2 >= r3 goto +20
// CHECK: 6d 54 13 00 00 00 00 00 	if r4 s> r5 goto +19
// CHECK: 7d 65 12 00 00 00 00 00 	if r5 s>= r6 goto +18

  if r6 < r7 goto Llabel0    // EUD_JLT  | EUD_X
  if r7 <= r8 goto Llabel0   // EUD_JLE  | EUD_X
  if r8 s< r9 goto Llabel0   // EUD_JSLT | EUD_X
  if r9 s<= r10 goto Llabel0 // EUD_JSLE | EUD_X
// CHECK: ad 76 11 00 00 00 00 00 	if r6 < r7 goto +17
// CHECK: bd 87 10 00 00 00 00 00 	if r7 <= r8 goto +16
// CHECK: cd 98 0f 00 00 00 00 00 	if r8 s< r9 goto +15
// CHECK: dd a9 0e 00 00 00 00 00 	if r9 s<= r10 goto +14

  if r0 == 0 goto Llabel0           // EUD_JEQ  | EUD_K
  if r3 != -1 goto Llabel0          // EUD_JNE  | EUD_K
// CHECK: 15 00 0d 00 00 00 00 00 	if r0 == 0 goto +13
// CHECK: 55 03 0c 00 ff ff ff ff 	if r3 != -1 goto +12

  if r1 > 64 goto Llabel0           // EUD_JGT  | EUD_K
  if r2 >= 0xffffffff goto Llabel0  // EUD_JGE  | EUD_K
  if r4 s> 0xffffffff goto Llabel0  // EUD_JSGT | EUD_K
  if r5 s>= 0x7fffffff goto Llabel0 // EUD_JSGE | EUD_K
// CHECK: 25 01 0b 00 40 00 00 00 	if r1 > 64 goto +11
// CHECK: 35 02 0a 00 ff ff ff ff 	if r2 >= -1 goto +10
// CHECK: 65 04 09 00 ff ff ff ff 	if r4 s> -1 goto +9
// CHECK: 75 05 08 00 ff ff ff 7f 	if r5 s>= 2147483647 goto +8

  if r6 < 0xff goto Llabel0         // EUD_JLT  | EUD_K
  if r7 <= 0xffff goto Llabel0      // EUD_JLE  | EUD_K
  if r8 s< 0 goto Llabel0           // EUD_JSLT | EUD_K
  if r9 s<= -1 goto Llabel0         // EUD_JSLE | EUD_K
// CHECK: a5 06 07 00 ff 00 00 00 	if r6 < 255 goto +7
// CHECK: b5 07 06 00 ff ff 00 00 	if r7 <= 65535 goto +6
// CHECK: c5 08 05 00 00 00 00 00 	if r8 s< 0 goto +5
// CHECK: d5 09 04 00 ff ff ff ff 	if r9 s<= -1 goto +4

// ======== EUD_ALU64 Class ========
  r0 += r1    // EUD_ADD  | EUD_X
  r1 -= r2    // EUD_SUB  | EUD_X
  r2 *= r3    // EUD_MUL  | EUD_X
  r3 /= r4    // EUD_DIV  | EUD_X
// CHECK: 0f 10 00 00 00 00 00 00 	r0 += r1
// CHECK: 1f 21 00 00 00 00 00 00 	r1 -= r2
// CHECK: 2f 32 00 00 00 00 00 00 	r2 *= r3
// CHECK: 3f 43 00 00 00 00 00 00 	r3 /= r4

Llabel0 :
  r2 = -r2    // EUD_NEG
  r4 |= r5    // EUD_OR   | EUD_X
  r5 &= r6    // EUD_AND  | EUD_X
  r6 <<= r7   // EUD_LSH  | EUD_X
  r7 >>= r8   // EUD_RSH  | EUD_X
  r8 ^= r9    // EUD_XOR  | EUD_X
  r9 = r10    // EUD_MOV  | EUD_X
  r10 s>>= r0 // EUD_ARSH | EUD_X
// CHECK:Llabel0:
// CHECK: 87 02 00 00 00 00 00 00	r2 = -r2
// CHECK: 4f 54 00 00 00 00 00 00 	r4 |= r5
// CHECK: 5f 65 00 00 00 00 00 00 	r5 &= r6
// CHECK: 6f 76 00 00 00 00 00 00 	r6 <<= r7
// CHECK: 7f 87 00 00 00 00 00 00 	r7 >>= r8
// CHECK: af 98 00 00 00 00 00 00 	r8 ^= r9
// CHECK: bf a9 00 00 00 00 00 00 	r9 = r10
// CHECK: cf 0a 00 00 00 00 00 00 	r10 s>>= r0

  r1 = be16 r1  // EUD_END  | EUD_TO_BE
  r2 = be32 r2  // EUD_END  | EUD_TO_BE
  r3 = be64 r3  // EUD_END  | EUD_TO_BE
// CHECK: dc 01 00 00 10 00 00 00 	r1 = be16 r1
// CHECK: dc 02 00 00 20 00 00 00 	r2 = be32 r2
// CHECK: dc 03 00 00 40 00 00 00 	r3 = be64 r3

  r0 += 1           // EUD_ADD  | EUD_K
  r1 -= 0x1         // EUD_SUB  | EUD_K
  r2 *= -4          // EUD_MUL  | EUD_K
  r3 /= 5           // EUD_DIV  | EUD_K
// CHECK: 07 00 00 00 01 00 00 00 	r0 += 1
// CHECK: 17 01 00 00 01 00 00 00 	r1 -= 1
// CHECK: 27 02 00 00 fc ff ff ff 	r2 *= -4
// CHECK: 37 03 00 00 05 00 00 00 	r3 /= 5

  r4 |= 0xff        // EUD_OR   | EUD_K
  r5 &= 0xFF        // EUD_AND  | EUD_K
  r6 <<= 63         // EUD_LSH  | EUD_K
  r7 >>= 32         // EUD_RSH  | EUD_K
  r8 ^= 0           // EUD_XOR  | EUD_K
  r9 = 1            // EUD_MOV  | EUD_K
  r9 = 0xffffffff   // EUD_MOV  | EUD_K
  r10 s>>= 64       // EUD_ARSH | EUD_K
// CHECK: 47 04 00 00 ff 00 00 00 	r4 |= 255
// CHECK: 57 05 00 00 ff 00 00 00 	r5 &= 255
// CHECK: 67 06 00 00 3f 00 00 00 	r6 <<= 63
// CHECK: 77 07 00 00 20 00 00 00 	r7 >>= 32
// CHECK: a7 08 00 00 00 00 00 00 	r8 ^= 0
// CHECK: b7 09 00 00 01 00 00 00 	r9 = 1
// CHECK: b7 09 00 00 ff ff ff ff 	r9 = -1
// CHECK: c7 0a 00 00 40 00 00 00 	r10 s>>= 64
