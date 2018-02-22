; RUN: llc -march=eud -filetype=obj < %s | llvm-objdump -r - | FileCheck --check-prefix=CHECK-RELOC %s

%struct.eud_context = type { i64, i64, i64, i64, i64, i64, i64 }
%struct.sk_buff = type { i64, i64, i64, i64, i64, i64, i64 }
%struct.net_device = type { i64, i64, i64, i64, i64, i64, i64 }

@eud_prog1.devname = private unnamed_addr constant [3 x i8] c"lo\00", align 1
@eud_prog1.fmt = private unnamed_addr constant [15 x i8] c"skb %x dev %x\0A\00", align 1

; Function Attrs: norecurse
define i32 @eud_prog1(%struct.eud_context* nocapture %ctx) #0 section "events/net/netif_receive_skb" {
  %devname = alloca [3 x i8], align 1
  %fmt = alloca [15 x i8], align 1
  %1 = getelementptr inbounds [3 x i8], [3 x i8]* %devname, i64 0, i64 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @eud_prog1.devname, i64 0, i64 0), i64 3, i1 false)
  %2 = getelementptr inbounds %struct.eud_context, %struct.eud_context* %ctx, i64 0, i32 0
  %3 = load i64, i64* %2, align 8
  %4 = inttoptr i64 %3 to %struct.sk_buff*
  %5 = getelementptr inbounds %struct.sk_buff, %struct.sk_buff* %4, i64 0, i32 2
  %6 = bitcast i64* %5 to i8*
  %7 = call i8* inttoptr (i64 4 to i8* (i8*)*)(i8* %6) #1
  %8 = call i32 inttoptr (i64 9 to i32 (i8*, i8*, i32)*)(i8* %7, i8* %1, i32 2) #1
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %10, label %13

; <label>:10                                      ; preds = %0
  %11 = getelementptr inbounds [15 x i8], [15 x i8]* %fmt, i64 0, i64 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %11, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @eud_prog1.fmt, i64 0, i64 0), i64 15, i1 false)
  %12 = call i32 (i8*, i32, ...) inttoptr (i64 11 to i32 (i8*, i32, ...)*)(i8* %11, i32 15, %struct.sk_buff* %4, i8* %7) #1
  br label %13

; <label>:13                                      ; preds = %10, %0
  ret i32 0

; CHECK-RELOC: file format ELF64-eud
; CHECK-RELOC: RELOCATION RECORDS FOR [.rel.eh_frame]:
; CHECK-RELOC: R_eud_64_64 events/net/netif_receive_skb
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture, i64, i1) #1

attributes #0 = { norecurse }
