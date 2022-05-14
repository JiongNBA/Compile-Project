; ModuleID = 'flySCC'
source_filename = "../test/test2.fly"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [6 x i8] c"%d %d\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"%d %d\00", align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.4 = private unnamed_addr constant [25 x i8] c"Incompatible Dimensions\0A\00", align 1
@.str.5 = private unnamed_addr constant [5 x i8] c"%10d\00", align 1
@.str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

declare i32 @scanf(i8*, ...)

declare i32 @getchar(...)

define i32 @fly() {
entryfly:
  %Matrix_A = alloca [25 x [25 x i32]]
  %Matrix_B = alloca [25 x [25 x i32]]
  %Ma = alloca i32
  %Mb = alloca i32
  %Na = alloca i32
  %Nb = alloca i32
  %temp = alloca i32
  %i = alloca i32
  %j = alloca i32
  %k = alloca i32
  %0 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str, i32 0, i32 0), i32* %Ma, i32* %Na)
  store i32 0, i32* %i
  br label %BBForCond

BBForCond:                                        ; preds = %BBForStart, %entryfly
  %1 = load i32, i32* %i
  %2 = load i32, i32* %Ma
  %3 = icmp slt i32 %1, %2
  %4 = zext i1 %3 to i32
  %.cmpneqi = icmp ne i32 0, %4
  br i1 %.cmpneqi, label %BBForBody, label %BBForEnd

BBForStart:                                       ; preds = %BBForEnd4
  %5 = load i32, i32* %i
  %addtmp6 = add i32 %5, 1
  store i32 %addtmp6, i32* %i
  br label %BBForCond

BBForBody:                                        ; preds = %BBForCond
  store i32 0, i32* %j
  br label %BBForCond1

BBForEnd:                                         ; preds = %BBForCond
  %6 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.2, i32 0, i32 0), i32* %Mb, i32* %Nb)
  store i32 0, i32* %i
  br label %BBForCond7

BBForCond1:                                       ; preds = %BBForStart2, %BBForBody
  %7 = load i32, i32* %j
  %8 = load i32, i32* %Na
  %9 = icmp slt i32 %7, %8
  %10 = zext i1 %9 to i32
  %.cmpneqi5 = icmp ne i32 0, %10
  br i1 %.cmpneqi5, label %BBForBody3, label %BBForEnd4

BBForStart2:                                      ; preds = %BBForBody3
  %11 = load i32, i32* %j
  %addtmp = add i32 %11, 1
  store i32 %addtmp, i32* %j
  br label %BBForCond1

BBForBody3:                                       ; preds = %BBForCond1
  %12 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* %temp)
  %13 = load i32, i32* %j
  %14 = load i32, i32* %i
  %15 = getelementptr [25 x [25 x i32]], [25 x [25 x i32]]* %Matrix_A, i32 0, i32 %14
  %16 = getelementptr [25 x i32], [25 x i32]* %15, i32 0, i32 %13
  %17 = load i32, i32* %temp
  store i32 %17, i32* %16
  br label %BBForStart2

BBForEnd4:                                        ; preds = %BBForCond1
  br label %BBForStart

BBForCond7:                                       ; preds = %BBForStart8, %BBForEnd
  %18 = load i32, i32* %i
  %19 = load i32, i32* %Mb
  %20 = icmp slt i32 %18, %19
  %21 = zext i1 %20 to i32
  %.cmpneqi11 = icmp ne i32 0, %21
  br i1 %.cmpneqi11, label %BBForBody9, label %BBForEnd10

BBForStart8:                                      ; preds = %BBForEnd15
  %22 = load i32, i32* %i
  %addtmp18 = add i32 %22, 1
  store i32 %addtmp18, i32* %i
  br label %BBForCond7

BBForBody9:                                       ; preds = %BBForCond7
  store i32 0, i32* %j
  br label %BBForCond12

BBForEnd10:                                       ; preds = %BBForCond7
  %23 = load i32, i32* %Na
  %24 = load i32, i32* %Mb
  %25 = icmp ne i32 %23, %24
  %26 = zext i1 %25 to i32
  %.cmpneqi19 = icmp ne i32 0, %26
  br i1 %.cmpneqi19, label %.then, label %.merge

BBForCond12:                                      ; preds = %BBForStart13, %BBForBody9
  %27 = load i32, i32* %j
  %28 = load i32, i32* %Nb
  %29 = icmp slt i32 %27, %28
  %30 = zext i1 %29 to i32
  %.cmpneqi16 = icmp ne i32 0, %30
  br i1 %.cmpneqi16, label %BBForBody14, label %BBForEnd15

BBForStart13:                                     ; preds = %BBForBody14
  %31 = load i32, i32* %j
  %addtmp17 = add i32 %31, 1
  store i32 %addtmp17, i32* %j
  br label %BBForCond12

BBForBody14:                                      ; preds = %BBForCond12
  %32 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.3, i32 0, i32 0), i32* %temp)
  %33 = load i32, i32* %j
  %34 = load i32, i32* %i
  %35 = getelementptr [25 x [25 x i32]], [25 x [25 x i32]]* %Matrix_B, i32 0, i32 %34
  %36 = getelementptr [25 x i32], [25 x i32]* %35, i32 0, i32 %33
  %37 = load i32, i32* %temp
  store i32 %37, i32* %36
  br label %BBForStart13

BBForEnd15:                                       ; preds = %BBForCond12
  br label %BBForStart8

.then:                                            ; preds = %BBForEnd10
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @.str.4, i32 0, i32 0))
  ret i32 0

.merge:                                           ; preds = %BBForEnd10
  %Mc = alloca i32
  %39 = load i32, i32* %Ma
  store i32 %39, i32* %Mc
  %Nc = alloca i32
  %40 = load i32, i32* %Nb
  store i32 %40, i32* %Nc
  %sum = alloca i32
  store i32 0, i32* %i
  br label %BBForCond20

BBForCond20:                                      ; preds = %BBForStart21, %.merge
  %41 = load i32, i32* %i
  %42 = load i32, i32* %Mc
  %43 = icmp slt i32 %41, %42
  %44 = zext i1 %43 to i32
  %.cmpneqi24 = icmp ne i32 0, %44
  br i1 %.cmpneqi24, label %BBForBody22, label %BBForEnd23

BBForStart21:                                     ; preds = %BBForEnd28
  %45 = load i32, i32* %i
  %addtmp38 = add i32 %45, 1
  store i32 %addtmp38, i32* %i
  br label %BBForCond20

BBForBody22:                                      ; preds = %BBForCond20
  store i32 0, i32* %j
  br label %BBForCond25

BBForEnd23:                                       ; preds = %BBForCond20
  ret i32 0

BBForCond25:                                      ; preds = %BBForStart26, %BBForBody22
  %46 = load i32, i32* %j
  %47 = load i32, i32* %Nc
  %48 = icmp slt i32 %46, %47
  %49 = zext i1 %48 to i32
  %.cmpneqi29 = icmp ne i32 0, %49
  br i1 %.cmpneqi29, label %BBForBody27, label %BBForEnd28

BBForStart26:                                     ; preds = %BBForEnd33
  %50 = load i32, i32* %j
  %addtmp37 = add i32 %50, 1
  store i32 %addtmp37, i32* %j
  br label %BBForCond25

BBForBody27:                                      ; preds = %BBForCond25
  store i32 0, i32* %sum
  store i32 0, i32* %k
  br label %BBForCond30

BBForEnd28:                                       ; preds = %BBForCond25
  %51 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0))
  br label %BBForStart21

BBForCond30:                                      ; preds = %BBForStart31, %BBForBody27
  %52 = load i32, i32* %k
  %53 = load i32, i32* %Na
  %54 = icmp slt i32 %52, %53
  %55 = zext i1 %54 to i32
  %.cmpneqi34 = icmp ne i32 0, %55
  br i1 %.cmpneqi34, label %BBForBody32, label %BBForEnd33

BBForStart31:                                     ; preds = %BBForBody32
  %56 = load i32, i32* %k
  %addtmp36 = add i32 %56, 1
  store i32 %addtmp36, i32* %k
  br label %BBForCond30

BBForBody32:                                      ; preds = %BBForCond30
  %57 = load i32, i32* %sum
  %58 = load i32, i32* %k
  %59 = load i32, i32* %i
  %60 = getelementptr [25 x [25 x i32]], [25 x [25 x i32]]* %Matrix_A, i32 0, i32 %59
  %61 = getelementptr [25 x i32], [25 x i32]* %60, i32 0, i32 %58
  %62 = load i32, i32* %61
  %63 = load i32, i32* %j
  %64 = load i32, i32* %k
  %65 = getelementptr [25 x [25 x i32]], [25 x [25 x i32]]* %Matrix_B, i32 0, i32 %64
  %66 = getelementptr [25 x i32], [25 x i32]* %65, i32 0, i32 %63
  %67 = load i32, i32* %66
  %multmp = mul i32 %62, %67
  %addtmp35 = add i32 %57, %multmp
  store i32 %addtmp35, i32* %sum
  br label %BBForStart31

BBForEnd33:                                       ; preds = %BBForCond30
  %68 = load i32, i32* %sum
  %69 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.5, i32 0, i32 0), i32 %68)
  br label %BBForStart26
}
