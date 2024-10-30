; ModuleID = 'demo'
source_filename = "demo"

@0 = private constant [15 x i8] c"Hello, World!\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @0, i32 0, i32 0))
  ret i32 0
}
