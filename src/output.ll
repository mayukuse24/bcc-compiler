; ModuleID = 'BccLLVMir'
source_filename = "BccLLVMir"
target triple = "x86_64-pc-linux-gnu"

@ant = common global i64 0
@temp = common global i64 0
@i = common global i64 0
@j = common global i64 0
@k = common global i64 0
@0 = private unnamed_addr constant [16 x i8] c"hey there %d \5Cn\00"
@1 = private unnamed_addr constant [22 x i8] c"testing loop works %d\00"

define void @main() {
entrypoint:
  store i64 4, i64* @ant
  %tmp = load i64, i64* @ant
  %addtmp = add i64 %tmp, 3
  store i64 %addtmp, i64* @ant
  %tmp1 = load i64, i64* @ant
  %addtmp2 = add i64 %tmp1, 10
  store i64 %addtmp2, i64* @temp
  %tmp3 = load i64, i64* @temp
  %printfCall = call i64 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @0, i32 0, i32 0), i64 %tmp3)
  store i64 0, i64* @i
  br label %while_header

while_header:                                     ; preds = %while_after7, %entrypoint
  %tmp4 = load i64, i64* @i
  %lttmp = icmp ult i64 %tmp4, 100
  %0 = zext i1 %lttmp to i64
  %ifcond = icmp ne i64 %0, 0
  br i1 %ifcond, label %while_body, label %while_after

while_body:                                       ; preds = %while_header
  store i64 0, i64* @j
  br label %while_header5

while_after:                                      ; preds = %while_header
  %tmp25 = load i64, i64* @ant
  %printfCall26 = call i64 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @1, i32 0, i32 0), i64 %tmp25)
  ret void

while_header5:                                    ; preds = %while_after13, %while_body
  %tmp8 = load i64, i64* @j
  %lttmp9 = icmp ult i64 %tmp8, 100
  %1 = zext i1 %lttmp9 to i64
  %ifcond10 = icmp ne i64 %1, 0
  br i1 %ifcond10, label %while_body6, label %while_after7

while_body6:                                      ; preds = %while_header5
  store i64 0, i64* @k
  br label %while_header11

while_after7:                                     ; preds = %while_header5
  %tmp23 = load i64, i64* @i
  %addtmp24 = add i64 %tmp23, 1
  store i64 %addtmp24, i64* @i
  br label %while_header

while_header11:                                   ; preds = %while_body12, %while_body6
  %tmp14 = load i64, i64* @k
  %lttmp15 = icmp ult i64 %tmp14, 100
  %2 = zext i1 %lttmp15 to i64
  %ifcond16 = icmp ne i64 %2, 0
  br i1 %ifcond16, label %while_body12, label %while_after13

while_body12:                                     ; preds = %while_header11
  %tmp17 = load i64, i64* @ant
  %addtmp18 = add i64 %tmp17, 1
  store i64 %addtmp18, i64* @ant
  %tmp19 = load i64, i64* @k
  %addtmp20 = add i64 %tmp19, 1
  store i64 %addtmp20, i64* @k
  br label %while_header11

while_after13:                                    ; preds = %while_header11
  %tmp21 = load i64, i64* @j
  %addtmp22 = add i64 %tmp21, 1
  store i64 %addtmp22, i64* @j
  br label %while_header5
}

declare i64 @printf(i8*, ...)
