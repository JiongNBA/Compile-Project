; ModuleID = 'flySCC'
source_filename = "../test/test3.fly"
target triple = "x86_64-pc-linux-gnu"

@name = common global [100 x [6 x i8]] zeroinitializer
@credit = common global [100 x i32] zeroinitializer
@precourse = common global [100 x [300 x i8]] zeroinitializer
@grade = common global [100 x i8] zeroinitializer
@i = global i32 0
@j = global i32 0
@k = global i32 0
@passcourse = common global [100 x [6 x i8]] zeroinitializer
@remainCourse = common global [100 x i32] zeroinitializer
@cur = common global [6 x i8] zeroinitializer
@Hours_Attempted = global i32 0
@Hours_Completed = global i32 0
@Remaining_Credits = global i32 0
@sum = global double 0.000000e+00
@ch = global i8 0
@info_idx = global i32 0
@char_idx = global i32 0
@.str = private unnamed_addr constant [10 x i8] c"GPA: 0.0\0A\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"GPA: %.1lf\0A\00", align 1
@.str.2 = private unnamed_addr constant [21 x i8] c"Hours Attempted: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [21 x i8] c"Hours Completed: %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [23 x i8] c"Credits Remaining: %d\0A\00", align 1
@.str.5 = private unnamed_addr constant [32 x i8] c"\0APossible Courses to Take Next\0A\00", align 1
@.str.6 = private unnamed_addr constant [27 x i8] c"  None - Congratulations!\0A\00", align 1
@.str.7 = private unnamed_addr constant [3 x i8] c"  \00", align 1
@.str.8 = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@.str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

declare i32 @scanf(i8*, ...)

declare i32 @getchar(...)

define void @readIn() {
entryreadIn:
  br label %whileStart

whileStart:                                       ; preds = %.merge, %entryreadIn
  %0 = call i32 (...) @getchar()
  %1 = trunc i32 %0 to i8
  store i8 %1, i8* @ch
  %2 = icmp ne i8 %1, 124
  %3 = zext i1 %2 to i8
  %.cmpneqc = icmp ne i8 0, %3
  br i1 %.cmpneqc, label %whileBody, label %whileEnd

whileBody:                                        ; preds = %whileStart
  %4 = load i32, i32* @info_idx
  %5 = icmp eq i32 %4, 0
  %6 = zext i1 %5 to i32
  %.cmpneqi = icmp ne i32 0, %6
  br i1 %.cmpneqi, label %.then, label %.else

whileEnd:                                         ; preds = %whileStart
  store i32 0, i32* @char_idx
  %7 = load i32, i32* @info_idx
  %addtmp4 = add i32 %7, 1
  store i32 %addtmp4, i32* @info_idx
  ret void

.then:                                            ; preds = %whileBody
  %8 = load i32, i32* @char_idx
  %9 = load i32, i32* @i
  %10 = getelementptr [100 x [6 x i8]], [100 x [6 x i8]]* @name, i32 0, i32 %9
  %11 = getelementptr [6 x i8], [6 x i8]* %10, i32 0, i32 %8
  %12 = load i8, i8* @ch
  store i8 %12, i8* %11
  br label %.merge

.else:                                            ; preds = %whileBody
  %13 = load i32, i32* @info_idx
  %14 = icmp eq i32 %13, 1
  %15 = zext i1 %14 to i32
  %.cmpneqi1 = icmp ne i32 0, %15
  br i1 %.cmpneqi1, label %.then2, label %.else3

.then2:                                           ; preds = %.else
  %16 = load i32, i32* @i
  %17 = getelementptr [100 x i32], [100 x i32]* @credit, i32 0, i32 %16
  %18 = load i8, i8* @ch
  %subtmp = sub i8 %18, 48
  %19 = sext i8 %subtmp to i32
  store i32 %19, i32* %17
  br label %.merge

.else3:                                           ; preds = %.else
  %20 = load i32, i32* @char_idx
  %21 = load i32, i32* @i
  %22 = getelementptr [100 x [300 x i8]], [100 x [300 x i8]]* @precourse, i32 0, i32 %21
  %23 = getelementptr [300 x i8], [300 x i8]* %22, i32 0, i32 %20
  %24 = load i8, i8* @ch
  store i8 %24, i8* %23
  br label %.merge

.merge:                                           ; preds = %.else3, %.then2, %.then
  %25 = load i32, i32* @char_idx
  %addtmp = add i32 %25, 1
  store i32 %addtmp, i32* @char_idx
  br label %whileStart
}

define void @prtCredit() {
entryprtCredit:
  %0 = load i32, i32* @Hours_Attempted
  %1 = icmp eq i32 %0, 0
  %2 = zext i1 %1 to i32
  %.cmpneqi = icmp ne i32 0, %2
  br i1 %.cmpneqi, label %.then, label %.else

.then:                                            ; preds = %entryprtCredit
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0))
  br label %.merge

.else:                                            ; preds = %entryprtCredit
  %4 = load double, double* @sum
  %5 = load i32, i32* @Hours_Attempted
  %6 = sitofp i32 %5 to double
  %divtmp = fdiv double %4, %6
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i32 0, i32 0), double %divtmp)
  br label %.merge

.merge:                                           ; preds = %.else, %.then
  %8 = load i32, i32* @Hours_Attempted
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.2, i32 0, i32 0), i32 %8)
  %10 = load i32, i32* @Hours_Completed
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i32 0, i32 0), i32 %10)
  %12 = load i32, i32* @Remaining_Credits
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.4, i32 0, i32 0), i32 %12)
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.5, i32 0, i32 0))
  ret void
}

define i32 @ifFind() {
entryifFind:
  %i = alloca i32
  %n = alloca i32
  store i32 0, i32* %n
  br label %BBForCond

BBForCond:                                        ; preds = %BBForStart, %entryifFind
  %0 = load i32, i32* %n
  %1 = icmp slt i32 %0, 100
  %2 = zext i1 %1 to i32
  %.cmpneqi = icmp ne i32 0, %2
  br i1 %.cmpneqi, label %BBForBody, label %BBForEnd

BBForStart:                                       ; preds = %.merge8
  %3 = load i32, i32* %n
  %addtmp9 = add i32 %3, 1
  store i32 %addtmp9, i32* %n
  br label %BBForCond

BBForBody:                                        ; preds = %BBForCond
  store i32 0, i32* %i
  br label %BBForCond1

BBForEnd:                                         ; preds = %BBForCond
  ret i32 0

BBForCond1:                                       ; preds = %BBForStart2, %BBForBody
  %4 = load i32, i32* %i
  %5 = icmp slt i32 %4, 6
  %6 = zext i1 %5 to i32
  %.cmpneqi5 = icmp ne i32 0, %6
  br i1 %.cmpneqi5, label %BBForBody3, label %BBForEnd4

BBForStart2:                                      ; preds = %.merge
  %7 = load i32, i32* %i
  %addtmp = add i32 %7, 1
  store i32 %addtmp, i32* %i
  br label %BBForCond1

BBForBody3:                                       ; preds = %BBForCond1
  %8 = load i32, i32* %i
  %9 = getelementptr [6 x i8], [6 x i8]* @cur, i32 0, i32 %8
  %10 = load i8, i8* %9
  %11 = load i32, i32* %i
  %12 = load i32, i32* %n
  %13 = getelementptr [100 x [6 x i8]], [100 x [6 x i8]]* @passcourse, i32 0, i32 %12
  %14 = getelementptr [6 x i8], [6 x i8]* %13, i32 0, i32 %11
  %15 = load i8, i8* %14
  %16 = icmp ne i8 %10, %15
  %17 = zext i1 %16 to i8
  %.cmpneqc = icmp ne i8 0, %17
  br i1 %.cmpneqc, label %.then, label %.merge

BBForEnd4:                                        ; preds = %.then, %BBForCond1
  %18 = load i32, i32* %i
  %19 = icmp eq i32 %18, 6
  %20 = zext i1 %19 to i32
  %.cmpneqi6 = icmp ne i32 0, %20
  br i1 %.cmpneqi6, label %.then7, label %.merge8

.then:                                            ; preds = %BBForBody3
  br label %BBForEnd4

.merge:                                           ; preds = %BBForBody3
  br label %BBForStart2

.then7:                                           ; preds = %BBForEnd4
  ret i32 1

.merge8:                                          ; preds = %BBForEnd4
  br label %BBForStart
}

define i32 @fly() {
entryfly:
  %x = alloca i32
  store i32 0, i32* %x
  br label %whileStart

whileStart:                                       ; preds = %.merge21, %entryfly
  %0 = call i32 (...) @getchar()
  %1 = trunc i32 %0 to i8
  store i8 %1, i8* @ch
  %2 = icmp ne i8 %1, 10
  %3 = zext i1 %2 to i8
  %.cmpneqc = icmp ne i8 0, %3
  br i1 %.cmpneqc, label %whileBody, label %whileEnd

whileBody:                                        ; preds = %whileStart
  %4 = load i32, i32* @char_idx
  %5 = load i32, i32* @i
  %6 = getelementptr [100 x [6 x i8]], [100 x [6 x i8]]* @name, i32 0, i32 %5
  %7 = getelementptr [6 x i8], [6 x i8]* %6, i32 0, i32 %4
  %8 = load i8, i8* @ch
  store i8 %8, i8* %7
  %9 = load i32, i32* @char_idx
  %addtmp = add i32 %9, 1
  store i32 %addtmp, i32* @char_idx
  call void @readIn()
  call void @readIn()
  call void @readIn()
  %10 = call i32 (...) @getchar()
  %11 = trunc i32 %10 to i8
  store i8 %11, i8* @ch
  %12 = icmp ne i8 %11, 10
  %13 = zext i1 %12 to i8
  %.cmpneqc1 = icmp ne i8 0, %13
  br i1 %.cmpneqc1, label %.then, label %.merge

whileEnd:                                         ; preds = %whileStart
  call void @prtCredit()
  %14 = load i32, i32* @k
  %15 = icmp eq i32 %14, 0
  %16 = zext i1 %15 to i32
  %.cmpneqi23 = icmp ne i32 0, %16
  br i1 %.cmpneqi23, label %.then24, label %.else25

.then:                                            ; preds = %whileBody
  %17 = load i32, i32* @i
  %18 = getelementptr [100 x i8], [100 x i8]* @grade, i32 0, i32 %17
  %19 = load i8, i8* @ch
  store i8 %19, i8* %18
  %20 = call i32 (...) @getchar()
  br label %.merge

.merge:                                           ; preds = %.then, %whileBody
  store i32 0, i32* @char_idx
  store i32 0, i32* @info_idx
  %21 = load i32, i32* @i
  %22 = getelementptr [100 x i8], [100 x i8]* @grade, i32 0, i32 %21
  %23 = load i8, i8* %22
  %24 = icmp sge i8 %23, 65
  %25 = zext i1 %24 to i8
  %26 = load i32, i32* @i
  %27 = getelementptr [100 x i8], [100 x i8]* @grade, i32 0, i32 %26
  %28 = load i8, i8* %27
  %29 = icmp sle i8 %28, 68
  %30 = zext i1 %29 to i8
  %31 = sext i8 %25 to i32
  %32 = sext i8 %30 to i32
  %33 = and i32 %31, %32
  %cmpneqi = icmp ne i32 0, %33
  %34 = zext i1 %cmpneqi to i32
  %35 = zext i1 %cmpneqi to i8
  %.cmpneqc2 = icmp ne i8 0, %35
  br i1 %.cmpneqc2, label %.then3, label %.else12

.then3:                                           ; preds = %.merge
  %36 = load double, double* @sum
  %37 = load i32, i32* @i
  %38 = getelementptr [100 x i8], [100 x i8]* @grade, i32 0, i32 %37
  %39 = load i8, i8* %38
  %subtmp = sub i8 69, %39
  %40 = load i32, i32* @i
  %41 = getelementptr [100 x i32], [100 x i32]* @credit, i32 0, i32 %40
  %42 = load i32, i32* %41
  %43 = zext i8 %subtmp to i32
  %multmp = mul i32 %43, %42
  %44 = sitofp i32 %multmp to double
  %addtmp4 = fadd double %36, %44
  store double %addtmp4, double* @sum
  %45 = load i32, i32* @Hours_Attempted
  %46 = load i32, i32* @i
  %47 = getelementptr [100 x i32], [100 x i32]* @credit, i32 0, i32 %46
  %48 = load i32, i32* %47
  %addtmp5 = add i32 %45, %48
  store i32 %addtmp5, i32* @Hours_Attempted
  %49 = load i32, i32* @Hours_Completed
  %50 = load i32, i32* @i
  %51 = getelementptr [100 x i32], [100 x i32]* @credit, i32 0, i32 %50
  %52 = load i32, i32* %51
  %addtmp6 = add i32 %49, %52
  store i32 %addtmp6, i32* @Hours_Completed
  store i32 0, i32* %x
  br label %BBForCond

BBForCond:                                        ; preds = %BBForStart, %.then3
  %53 = load i32, i32* %x
  %54 = icmp slt i32 %53, 6
  %55 = zext i1 %54 to i32
  %.cmpneqi = icmp ne i32 0, %55
  br i1 %.cmpneqi, label %BBForBody, label %BBForEnd

BBForStart:                                       ; preds = %.merge9
  %56 = load i32, i32* %x
  %addtmp10 = add i32 %56, 1
  store i32 %addtmp10, i32* %x
  br label %BBForCond

BBForBody:                                        ; preds = %BBForCond
  %57 = load i32, i32* %x
  %58 = load i32, i32* @i
  %59 = getelementptr [100 x [6 x i8]], [100 x [6 x i8]]* @name, i32 0, i32 %58
  %60 = getelementptr [6 x i8], [6 x i8]* %59, i32 0, i32 %57
  %61 = load i8, i8* %60
  %62 = icmp ne i8 %61, 0
  %63 = zext i1 %62 to i8
  %.cmpneqc7 = icmp ne i8 0, %63
  br i1 %.cmpneqc7, label %.then8, label %.else

BBForEnd:                                         ; preds = %BBForCond
  %64 = load i32, i32* @j
  %addtmp11 = add i32 %64, 1
  store i32 %addtmp11, i32* @j
  br label %.merge21

.then8:                                           ; preds = %BBForBody
  %65 = load i32, i32* %x
  %66 = load i32, i32* @j
  %67 = getelementptr [100 x [6 x i8]], [100 x [6 x i8]]* @passcourse, i32 0, i32 %66
  %68 = getelementptr [6 x i8], [6 x i8]* %67, i32 0, i32 %65
  %69 = load i32, i32* %x
  %70 = load i32, i32* @i
  %71 = getelementptr [100 x [6 x i8]], [100 x [6 x i8]]* @name, i32 0, i32 %70
  %72 = getelementptr [6 x i8], [6 x i8]* %71, i32 0, i32 %69
  %73 = load i8, i8* %72
  store i8 %73, i8* %68
  br label %.merge9

.else:                                            ; preds = %BBForBody
  %74 = load i32, i32* %x
  %75 = load i32, i32* @j
  %76 = getelementptr [100 x [6 x i8]], [100 x [6 x i8]]* @passcourse, i32 0, i32 %75
  %77 = getelementptr [6 x i8], [6 x i8]* %76, i32 0, i32 %74
  store i8 0, i8* %77
  br label %.merge9

.merge9:                                          ; preds = %.else, %.then8
  br label %BBForStart

.else12:                                          ; preds = %.merge
  %78 = load i32, i32* @i
  %79 = getelementptr [100 x i8], [100 x i8]* @grade, i32 0, i32 %78
  %80 = load i8, i8* %79
  %81 = icmp eq i8 %80, 70
  %82 = zext i1 %81 to i8
  %.cmpneqc13 = icmp ne i8 0, %82
  br i1 %.cmpneqc13, label %.then14, label %.else18

.then14:                                          ; preds = %.else12
  %83 = load i32, i32* @Hours_Attempted
  %84 = load i32, i32* @i
  %85 = getelementptr [100 x i32], [100 x i32]* @credit, i32 0, i32 %84
  %86 = load i32, i32* %85
  %addtmp15 = add i32 %83, %86
  store i32 %addtmp15, i32* @Hours_Attempted
  %87 = load i32, i32* @Remaining_Credits
  %88 = load i32, i32* @i
  %89 = getelementptr [100 x i32], [100 x i32]* @credit, i32 0, i32 %88
  %90 = load i32, i32* %89
  %addtmp16 = add i32 %87, %90
  store i32 %addtmp16, i32* @Remaining_Credits
  %91 = load i32, i32* @k
  %92 = getelementptr [100 x i32], [100 x i32]* @remainCourse, i32 0, i32 %91
  %93 = load i32, i32* @i
  store i32 %93, i32* %92
  %94 = load i32, i32* @k
  %addtmp17 = add i32 %94, 1
  store i32 %addtmp17, i32* @k
  br label %.merge21

.else18:                                          ; preds = %.else12
  %95 = load i32, i32* @Remaining_Credits
  %96 = load i32, i32* @i
  %97 = getelementptr [100 x i32], [100 x i32]* @credit, i32 0, i32 %96
  %98 = load i32, i32* %97
  %addtmp19 = add i32 %95, %98
  store i32 %addtmp19, i32* @Remaining_Credits
  %99 = load i32, i32* @k
  %100 = getelementptr [100 x i32], [100 x i32]* @remainCourse, i32 0, i32 %99
  %101 = load i32, i32* @i
  store i32 %101, i32* %100
  %102 = load i32, i32* @k
  %addtmp20 = add i32 %102, 1
  store i32 %addtmp20, i32* @k
  br label %.merge21

.merge21:                                         ; preds = %.else18, %.then14, %BBForEnd
  %103 = load i32, i32* @i
  %addtmp22 = add i32 %103, 1
  store i32 %addtmp22, i32* @i
  br label %whileStart

.then24:                                          ; preds = %whileEnd
  %104 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.6, i32 0, i32 0))
  br label %.merge106

.else25:                                          ; preds = %whileEnd
  %m = alloca i32
  store i32 0, i32* %m
  br label %BBForCond26

BBForCond26:                                      ; preds = %BBForStart27, %.else25
  %105 = load i32, i32* %m
  %106 = load i32, i32* @k
  %107 = icmp slt i32 %105, %106
  %108 = zext i1 %107 to i32
  %.cmpneqi30 = icmp ne i32 0, %108
  br i1 %.cmpneqi30, label %BBForBody28, label %BBForEnd29

BBForStart27:                                     ; preds = %.merge104, %.then79
  %109 = load i32, i32* %m
  %addtmp105 = add i32 %109, 1
  store i32 %addtmp105, i32* %m
  br label %BBForCond26

BBForBody28:                                      ; preds = %BBForCond26
  %canStudy = alloca i32
  store i32 1, i32* %canStudy
  %isSame = alloca i32
  store i32 0, i32* %isSame
  %preIndex = alloca i32
  store i32 0, i32* %preIndex
  %isChecked = alloca i32
  store i32 0, i32* %isChecked
  %pre = alloca i8
  %110 = load i32, i32* %preIndex
  %111 = load i32, i32* %m
  %112 = getelementptr [100 x i32], [100 x i32]* @remainCourse, i32 0, i32 %111
  %113 = load i32, i32* %112
  %114 = getelementptr [100 x [300 x i8]], [100 x [300 x i8]]* @precourse, i32 0, i32 %113
  %115 = getelementptr [300 x i8], [300 x i8]* %114, i32 0, i32 %110
  %116 = load i8, i8* %115
  store i8 %116, i8* %pre
  store i32 0, i32* @i
  br label %BBForCond31

BBForEnd29:                                       ; preds = %BBForCond26
  br label %.merge106

BBForCond31:                                      ; preds = %BBForStart32, %BBForBody28
  %117 = load i32, i32* @i
  %118 = icmp slt i32 %117, 6
  %119 = zext i1 %118 to i32
  %.cmpneqi35 = icmp ne i32 0, %119
  br i1 %.cmpneqi35, label %BBForBody33, label %BBForEnd34

BBForStart32:                                     ; preds = %BBForBody33
  %120 = load i32, i32* @i
  %addtmp36 = add i32 %120, 1
  store i32 %addtmp36, i32* @i
  br label %BBForCond31

BBForBody33:                                      ; preds = %BBForCond31
  %121 = load i32, i32* @i
  %122 = getelementptr [6 x i8], [6 x i8]* @cur, i32 0, i32 %121
  store i8 0, i8* %122
  br label %BBForStart32

BBForEnd34:                                       ; preds = %BBForCond31
  %cnt = alloca i32
  store i32 0, i32* %cnt
  br label %whileStart37

whileStart37:                                     ; preds = %.merge76, %BBForEnd34
  %123 = load i8, i8* %pre
  %124 = icmp ne i8 %123, 0
  %125 = zext i1 %124 to i8
  %.cmpneqc40 = icmp ne i8 0, %125
  br i1 %.cmpneqc40, label %whileBody38, label %whileEnd39

whileBody38:                                      ; preds = %whileStart37
  %126 = load i8, i8* %pre
  %127 = icmp eq i8 %126, 44
  %128 = zext i1 %127 to i8
  %.cmpneqc41 = icmp ne i8 0, %128
  br i1 %.cmpneqc41, label %.then42, label %.else55

whileEnd39:                                       ; preds = %.else66, %whileStart37
  %129 = load i32, i32* %canStudy
  %130 = icmp eq i32 %129, 0
  %131 = zext i1 %130 to i32
  %.cmpneqi78 = icmp ne i32 0, %131
  br i1 %.cmpneqi78, label %.then79, label %.else80

.then42:                                          ; preds = %whileBody38
  store i32 0, i32* %isSame
  store i32 0, i32* %isChecked
  store i32 0, i32* %cnt
  %132 = load i32, i32* %canStudy
  %.cmpneqi43 = icmp ne i32 0, %132
  br i1 %.cmpneqi43, label %.then44, label %.merge48

.then44:                                          ; preds = %.then42
  %133 = call i32 @ifFind()
  store i32 %133, i32* %isSame
  %134 = load i32, i32* %isSame
  %135 = icmp eq i32 %134, 0
  %136 = zext i1 %135 to i32
  %.cmpneqi45 = icmp ne i32 0, %136
  br i1 %.cmpneqi45, label %.then46, label %.merge47

.then46:                                          ; preds = %.then44
  store i32 0, i32* %canStudy
  br label %.merge47

.merge47:                                         ; preds = %.then46, %.then44
  br label %.merge48

.merge48:                                         ; preds = %.merge47, %.then42
  store i32 0, i32* @i
  br label %BBForCond49

BBForCond49:                                      ; preds = %BBForStart50, %.merge48
  %137 = load i32, i32* @i
  %138 = icmp slt i32 %137, 6
  %139 = zext i1 %138 to i32
  %.cmpneqi53 = icmp ne i32 0, %139
  br i1 %.cmpneqi53, label %BBForBody51, label %BBForEnd52

BBForStart50:                                     ; preds = %BBForBody51
  %140 = load i32, i32* @i
  %addtmp54 = add i32 %140, 1
  store i32 %addtmp54, i32* @i
  br label %BBForCond49

BBForBody51:                                      ; preds = %BBForCond49
  %141 = load i32, i32* @i
  %142 = getelementptr [6 x i8], [6 x i8]* @cur, i32 0, i32 %141
  store i8 0, i8* %142
  br label %BBForStart50

BBForEnd52:                                       ; preds = %BBForCond49
  br label %.merge76

.else55:                                          ; preds = %whileBody38
  %143 = load i8, i8* %pre
  %144 = icmp eq i8 %143, 59
  %145 = zext i1 %144 to i8
  %.cmpneqc56 = icmp ne i8 0, %145
  br i1 %.cmpneqc56, label %.then57, label %.else74

.then57:                                          ; preds = %.else55
  store i32 0, i32* %isSame
  store i32 1, i32* %isChecked
  store i32 0, i32* %cnt
  %146 = load i32, i32* %canStudy
  %.cmpneqi58 = icmp ne i32 0, %146
  br i1 %.cmpneqi58, label %.then59, label %.merge60

.then59:                                          ; preds = %.then57
  %147 = call i32 @ifFind()
  store i32 %147, i32* %isSame
  br label %.merge60

.merge60:                                         ; preds = %.then59, %.then57
  %148 = load i32, i32* %isSame
  %149 = icmp eq i32 %148, 0
  %150 = zext i1 %149 to i32
  %.cmpneqi61 = icmp ne i32 0, %150
  br i1 %.cmpneqi61, label %.then62, label %.merge63

.then62:                                          ; preds = %.merge60
  store i32 0, i32* %canStudy
  br label %.merge63

.merge63:                                         ; preds = %.then62, %.merge60
  %151 = load i32, i32* %canStudy
  %152 = icmp eq i32 %151, 0
  %153 = zext i1 %152 to i32
  %.cmpneqi64 = icmp ne i32 0, %153
  br i1 %.cmpneqi64, label %.then65, label %.else66

.then65:                                          ; preds = %.merge63
  store i32 1, i32* %canStudy
  br label %.merge67

.else66:                                          ; preds = %.merge63
  br label %whileEnd39

.merge67:                                         ; preds = %.then65
  store i32 0, i32* @i
  br label %BBForCond68

BBForCond68:                                      ; preds = %BBForStart69, %.merge67
  %154 = load i32, i32* @i
  %155 = icmp slt i32 %154, 6
  %156 = zext i1 %155 to i32
  %.cmpneqi72 = icmp ne i32 0, %156
  br i1 %.cmpneqi72, label %BBForBody70, label %BBForEnd71

BBForStart69:                                     ; preds = %BBForBody70
  %157 = load i32, i32* @i
  %addtmp73 = add i32 %157, 1
  store i32 %addtmp73, i32* @i
  br label %BBForCond68

BBForBody70:                                      ; preds = %BBForCond68
  %158 = load i32, i32* @i
  %159 = getelementptr [6 x i8], [6 x i8]* @cur, i32 0, i32 %158
  store i8 0, i8* %159
  br label %BBForStart69

BBForEnd71:                                       ; preds = %BBForCond68
  br label %.merge76

.else74:                                          ; preds = %.else55
  %160 = load i32, i32* %cnt
  %161 = getelementptr [6 x i8], [6 x i8]* @cur, i32 0, i32 %160
  %162 = load i8, i8* %pre
  store i8 %162, i8* %161
  %163 = load i32, i32* %cnt
  %addtmp75 = add i32 %163, 1
  store i32 %addtmp75, i32* %cnt
  store i32 0, i32* %isChecked
  br label %.merge76

.merge76:                                         ; preds = %.else74, %BBForEnd71, %BBForEnd52
  %164 = load i32, i32* %preIndex
  %addtmp77 = add i32 %164, 1
  store i32 %addtmp77, i32* %preIndex
  %165 = load i32, i32* %preIndex
  %166 = load i32, i32* %m
  %167 = getelementptr [100 x i32], [100 x i32]* @remainCourse, i32 0, i32 %166
  %168 = load i32, i32* %167
  %169 = getelementptr [100 x [300 x i8]], [100 x [300 x i8]]* @precourse, i32 0, i32 %168
  %170 = getelementptr [300 x i8], [300 x i8]* %169, i32 0, i32 %165
  %171 = load i8, i8* %170
  store i8 %171, i8* %pre
  br label %whileStart37

.then79:                                          ; preds = %whileEnd39
  br label %BBForStart27

.else80:                                          ; preds = %whileEnd39
  %172 = load i8, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @cur, i32 0, i32 0)
  %173 = icmp eq i8 %172, 0
  %174 = zext i1 %173 to i8
  %.cmpneqc81 = icmp ne i8 0, %174
  br i1 %.cmpneqc81, label %.then82, label %.else83

.then82:                                          ; preds = %.else80
  store i32 1, i32* %canStudy
  br label %.merge90

.else83:                                          ; preds = %.else80
  %175 = load i32, i32* %isChecked
  %176 = icmp eq i32 %175, 0
  %177 = zext i1 %176 to i32
  %.cmpneqi84 = icmp ne i32 0, %177
  br i1 %.cmpneqi84, label %.then85, label %.merge89

.then85:                                          ; preds = %.else83
  store i32 0, i32* %isSame
  %178 = call i32 @ifFind()
  store i32 %178, i32* %isSame
  %179 = load i32, i32* %isSame
  %180 = icmp eq i32 %179, 0
  %181 = zext i1 %180 to i32
  %.cmpneqi86 = icmp ne i32 0, %181
  br i1 %.cmpneqi86, label %.then87, label %.merge88

.then87:                                          ; preds = %.then85
  store i32 0, i32* %canStudy
  br label %.merge88

.merge88:                                         ; preds = %.then87, %.then85
  br label %.merge89

.merge89:                                         ; preds = %.merge88, %.else83
  br label %.merge90

.merge90:                                         ; preds = %.merge89, %.then82
  %182 = load i32, i32* %canStudy
  %.cmpneqi91 = icmp ne i32 0, %182
  br i1 %.cmpneqi91, label %.then92, label %.merge103

.then92:                                          ; preds = %.merge90
  %183 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.7, i32 0, i32 0))
  store i32 0, i32* %x
  br label %BBForCond93

BBForCond93:                                      ; preds = %BBForStart94, %.then92
  %184 = load i32, i32* %x
  %185 = icmp slt i32 %184, 6
  %186 = zext i1 %185 to i32
  %.cmpneqi97 = icmp ne i32 0, %186
  br i1 %.cmpneqi97, label %BBForBody95, label %BBForEnd96

BBForStart94:                                     ; preds = %.merge101
  %187 = load i32, i32* %x
  %addtmp102 = add i32 %187, 1
  store i32 %addtmp102, i32* %x
  br label %BBForCond93

BBForBody95:                                      ; preds = %BBForCond93
  %188 = load i32, i32* %x
  %189 = load i32, i32* %m
  %190 = getelementptr [100 x i32], [100 x i32]* @remainCourse, i32 0, i32 %189
  %191 = load i32, i32* %190
  %192 = getelementptr [100 x [6 x i8]], [100 x [6 x i8]]* @name, i32 0, i32 %191
  %193 = getelementptr [6 x i8], [6 x i8]* %192, i32 0, i32 %188
  %194 = load i8, i8* %193
  %195 = icmp eq i8 %194, 0
  %196 = zext i1 %195 to i8
  %.cmpneqc98 = icmp ne i8 0, %196
  br i1 %.cmpneqc98, label %.then99, label %.else100

BBForEnd96:                                       ; preds = %.then99, %BBForCond93
  %197 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.9, i32 0, i32 0))
  br label %.merge103

.then99:                                          ; preds = %BBForBody95
  br label %BBForEnd96

.else100:                                         ; preds = %BBForBody95
  %198 = load i32, i32* %x
  %199 = load i32, i32* %m
  %200 = getelementptr [100 x i32], [100 x i32]* @remainCourse, i32 0, i32 %199
  %201 = load i32, i32* %200
  %202 = getelementptr [100 x [6 x i8]], [100 x [6 x i8]]* @name, i32 0, i32 %201
  %203 = getelementptr [6 x i8], [6 x i8]* %202, i32 0, i32 %198
  %204 = load i8, i8* %203
  %205 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.8, i32 0, i32 0), i8 %204)
  br label %.merge101

.merge101:                                        ; preds = %.else100
  br label %BBForStart94

.merge103:                                        ; preds = %BBForEnd96, %.merge90
  br label %.merge104

.merge104:                                        ; preds = %.merge103
  br label %BBForStart27

.merge106:                                        ; preds = %BBForEnd29, %.then24
  ret i32 0
}
