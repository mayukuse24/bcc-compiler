@ant = common global i64 0
@temp = common global i64 0
@0 = private unnamed_addr constant [12 x i8] c"4 5 6 %d %d\00"
@1 = private unnamed_addr constant [3 x i8] c"%d\00"

define void @main() {
entrypoint:
  store i64 4, i64* @ant
  %tmp = load i64, i64* @ant
  %addtmp = add i64 %tmp, 1
  store i64 %addtmp, i64* @ant
  %tmp1 = load i64, i64* @ant
  %addtmp2 = add i64 %tmp1, 10
  store i64 %addtmp2, i64* @temp
  %tmp3 = load i64, i64* @ant
  %tmp4 = load i64, i64* @temp
  %printfCall = call i64 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, 
i32 0), i64 %tmp3, i64 %tmp4)
  br label %while_header

while_header:                                     ; preds = %while_body, %entrypoint
  %tmp5 = load i64, i64* @ant
  %gttmp = icmp ugt i64 %tmp5, 1
  %loopcond = fcmp one i1 %gttmp, i64 0
  br i1 %loopcond, label %while_body, label %while_after

while_body:                                       ; preds = %while_header
  %tmp6 = load i64, i64* @ant
  %printfCall7 = call i64 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @1, i32 0, 
i32 0), i64 %tmp6)
  %tmp8 = load i64, i64* @ant
  %subtmp = sub i64 %tmp8, 1
  store i64 %subtmp, i64* @ant
  br label %while_header

while_after:                                      ; preds = %while_header
  ret void
}

declare i64 @printf(i8*, ...)
