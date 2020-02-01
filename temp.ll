; ModuleID = 'calculator'
source_filename = "calculator"

@format_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(...)

declare double @pow(double, double)

define void @main() {
entrypoint:
  %b = alloca i32
  store i32 5, i32* %b
  %a = alloca i32
  store i32 2, i32* %a
  %b1 = load i32, i32* %b
  %printf = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @format_int, i32 0, i32 0), i32 %b1)
  ret void
}
