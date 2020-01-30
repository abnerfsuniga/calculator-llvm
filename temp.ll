; ModuleID = 'calculator'
source_filename = "calculator"

@format_float = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@format_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(...)

declare double @pow(double, double)

define void @main() {
entrypoint:
  %b = alloca double
  %pow = call double @pow(double 3.000000e+00, double 3.000000e+00)
  store double %pow, double* %b
  %b1 = load double, double* %b
  %printf = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @format_float, i32 0, i32 0), double %b1)
  %c = alloca i32
  %pow2 = call double @pow(double 3.000000e+00, double 4.000000e+00)
  %icast = fptosi double %pow2 to i32
  store i32 %icast, i32* %c
  %c3 = alloca i32
  store i32 1, i32* %c3
  %c4 = load i32, i32* %c3
  %printf5 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @format_int, i32 0, i32 0), i32 %c4)
  ret void
}
