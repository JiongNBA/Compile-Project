; ModuleID = 'flySCC'
source_filename = "../test/test1.fly"
target triple = "x86_64-pc-linux-gnu"

@arr = common global [10000 x i32] zeroinitializer
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

declare i32 @scanf(i8*, ...)

declare i32 @getchar(...)

define void @swap(i32 %0, i32 %1) {
entryswap:
  %pos1 = alloca i32
  store i32 %0, i32* %pos1
  %pos2 = alloca i32
  store i32 %1, i32* %pos2
  %temp = alloca i32
  %2 = load i32, i32* %pos1
  %3 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %2
  %4 = load i32, i32* %3
  store i32 %4, i32* %temp
  %5 = load i32, i32* %pos1
  %6 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %5
  %7 = load i32, i32* %pos2
  %8 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %7
  %9 = load i32, i32* %8
  store i32 %9, i32* %6
  %10 = load i32, i32* %pos2
  %11 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %10
  %12 = load i32, i32* %temp
  store i32 %12, i32* %11
  ret void
}

define void @InsertionSort(i32 %0, i32 %1) {
entryInsertionSort:
  %offset = alloca i32
  store i32 %0, i32* %offset
  %N = alloca i32
  store i32 %1, i32* %N
  %temp = alloca i32
  %i = alloca i32
  %j = alloca i32
  %2 = load i32, i32* %offset
  %addtmp = add i32 1, %2
  store i32 %addtmp, i32* %i
  br label %BBForCond

BBForCond:                                        ; preds = %BBForStart, %entryInsertionSort
  %3 = load i32, i32* %i
  %4 = load i32, i32* %N
  %5 = load i32, i32* %offset
  %addtmp1 = add i32 %4, %5
  %6 = icmp slt i32 %3, %addtmp1
  %7 = zext i1 %6 to i32
  %.cmpneqi = icmp ne i32 0, %7
  br i1 %.cmpneqi, label %BBForBody, label %BBForEnd

BBForStart:                                       ; preds = %BBForEnd5
  %8 = load i32, i32* %i
  %addtmp9 = add i32 %8, 1
  store i32 %addtmp9, i32* %i
  br label %BBForCond

BBForBody:                                        ; preds = %BBForCond
  %9 = load i32, i32* %i
  %10 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %9
  %11 = load i32, i32* %10
  store i32 %11, i32* %temp
  %12 = load i32, i32* %i
  store i32 %12, i32* %j
  br label %BBForCond2

BBForEnd:                                         ; preds = %BBForCond
  ret void

BBForCond2:                                       ; preds = %BBForStart3, %BBForBody
  %13 = load i32, i32* %j
  %14 = icmp sgt i32 %13, 0
  %15 = zext i1 %14 to i32
  %16 = load i32, i32* %j
  %subtmp = sub i32 %16, 1
  %17 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %subtmp
  %18 = load i32, i32* %17
  %19 = load i32, i32* %temp
  %20 = icmp sgt i32 %18, %19
  %21 = zext i1 %20 to i32
  %22 = and i32 %15, %21
  %cmpneqi = icmp ne i32 0, %22
  %23 = zext i1 %cmpneqi to i32
  %.cmpneqi6 = icmp ne i32 0, %23
  br i1 %.cmpneqi6, label %BBForBody4, label %BBForEnd5

BBForStart3:                                      ; preds = %BBForBody4
  %24 = load i32, i32* %j
  %subtmp8 = sub i32 %24, 1
  store i32 %subtmp8, i32* %j
  br label %BBForCond2

BBForBody4:                                       ; preds = %BBForCond2
  %25 = load i32, i32* %j
  %26 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %25
  %27 = load i32, i32* %j
  %subtmp7 = sub i32 %27, 1
  %28 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %subtmp7
  %29 = load i32, i32* %28
  store i32 %29, i32* %26
  br label %BBForStart3

BBForEnd5:                                        ; preds = %BBForCond2
  %30 = load i32, i32* %j
  %31 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %30
  %32 = load i32, i32* %temp
  store i32 %32, i32* %31
  br label %BBForStart
}

define i32 @Median3(i32 %0, i32 %1) {
entryMedian3:
  %Left = alloca i32
  store i32 %0, i32* %Left
  %Right = alloca i32
  store i32 %1, i32* %Right
  %Center = alloca i32
  %2 = load i32, i32* %Left
  %3 = load i32, i32* %Right
  %addtmp = add i32 %2, %3
  %divtmp = sdiv i32 %addtmp, 2
  store i32 %divtmp, i32* %Center
  %4 = load i32, i32* %Left
  %5 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %4
  %6 = load i32, i32* %5
  %7 = load i32, i32* %Center
  %8 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %7
  %9 = load i32, i32* %8
  %10 = icmp sgt i32 %6, %9
  %11 = zext i1 %10 to i32
  %.cmpneqi = icmp ne i32 0, %11
  br i1 %.cmpneqi, label %.then, label %.merge

.then:                                            ; preds = %entryMedian3
  %12 = load i32, i32* %Left
  %13 = load i32, i32* %Center
  call void @swap(i32 %12, i32 %13)
  br label %.merge

.merge:                                           ; preds = %.then, %entryMedian3
  %14 = load i32, i32* %Left
  %15 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %14
  %16 = load i32, i32* %15
  %17 = load i32, i32* %Right
  %18 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %17
  %19 = load i32, i32* %18
  %20 = icmp sgt i32 %16, %19
  %21 = zext i1 %20 to i32
  %.cmpneqi1 = icmp ne i32 0, %21
  br i1 %.cmpneqi1, label %.then2, label %.merge3

.then2:                                           ; preds = %.merge
  %22 = load i32, i32* %Left
  %23 = load i32, i32* %Right
  call void @swap(i32 %22, i32 %23)
  br label %.merge3

.merge3:                                          ; preds = %.then2, %.merge
  %24 = load i32, i32* %Center
  %25 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %24
  %26 = load i32, i32* %25
  %27 = load i32, i32* %Right
  %28 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %27
  %29 = load i32, i32* %28
  %30 = icmp sgt i32 %26, %29
  %31 = zext i1 %30 to i32
  %.cmpneqi4 = icmp ne i32 0, %31
  br i1 %.cmpneqi4, label %.then5, label %.merge6

.then5:                                           ; preds = %.merge3
  %32 = load i32, i32* %Center
  %33 = load i32, i32* %Right
  call void @swap(i32 %32, i32 %33)
  br label %.merge6

.merge6:                                          ; preds = %.then5, %.merge3
  %34 = load i32, i32* %Center
  %35 = load i32, i32* %Right
  %subtmp = sub i32 %35, 1
  call void @swap(i32 %34, i32 %subtmp)
  %36 = load i32, i32* %Right
  %subtmp7 = sub i32 %36, 1
  %37 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %subtmp7
  %38 = load i32, i32* %37
  ret i32 %38
}

define void @QSort(i32 %0, i32 %1) {
entryQSort:
  %Left = alloca i32
  store i32 %0, i32* %Left
  %Right = alloca i32
  store i32 %1, i32* %Right
  %i = alloca i32
  %j = alloca i32
  %Pivot = alloca i32
  %2 = load i32, i32* %Left
  %addtmp = add i32 %2, 3
  %3 = load i32, i32* %Right
  %4 = icmp sle i32 %addtmp, %3
  %5 = zext i1 %4 to i32
  %.cmpneqi = icmp ne i32 0, %5
  br i1 %.cmpneqi, label %.then, label %.else20

.then:                                            ; preds = %entryQSort
  %6 = load i32, i32* %Left
  %7 = load i32, i32* %Right
  %8 = call i32 @Median3(i32 %6, i32 %7)
  store i32 %8, i32* %Pivot
  %9 = load i32, i32* %Left
  store i32 %9, i32* %i
  %10 = load i32, i32* %Right
  %subtmp = sub i32 %10, 1
  store i32 %subtmp, i32* %j
  br label %whileStart

whileStart:                                       ; preds = %.merge16, %.then
  br i1 true, label %whileBody, label %whileEnd

whileBody:                                        ; preds = %whileStart
  br label %whileStart1

whileEnd:                                         ; preds = %.else, %whileStart
  %11 = load i32, i32* %i
  %12 = load i32, i32* %Right
  %subtmp17 = sub i32 %12, 1
  call void @swap(i32 %11, i32 %subtmp17)
  %13 = load i32, i32* %Left
  %14 = load i32, i32* %i
  %subtmp18 = sub i32 %14, 1
  call void @QSort(i32 %13, i32 %subtmp18)
  %15 = load i32, i32* %i
  %addtmp19 = add i32 %15, 1
  %16 = load i32, i32* %Right
  call void @QSort(i32 %addtmp19, i32 %16)
  br label %.merge23

whileStart1:                                      ; preds = %.merge, %whileBody
  br i1 true, label %whileBody2, label %whileEnd3

whileBody2:                                       ; preds = %whileStart1
  %17 = load i32, i32* %i
  %addtmp4 = add i32 %17, 1
  store i32 %addtmp4, i32* %i
  %18 = load i32, i32* %Pivot
  %19 = load i32, i32* %i
  %20 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %19
  %21 = load i32, i32* %20
  %22 = icmp sle i32 %18, %21
  %23 = zext i1 %22 to i32
  %.cmpneqi5 = icmp ne i32 0, %23
  br i1 %.cmpneqi5, label %.then6, label %.merge

whileEnd3:                                        ; preds = %.then6, %whileStart1
  br label %whileStart7

.then6:                                           ; preds = %whileBody2
  br label %whileEnd3

.merge:                                           ; preds = %whileBody2
  br label %whileStart1

whileStart7:                                      ; preds = %.merge13, %whileEnd3
  br i1 true, label %whileBody8, label %whileEnd9

whileBody8:                                       ; preds = %whileStart7
  %24 = load i32, i32* %j
  %subtmp10 = sub i32 %24, 1
  store i32 %subtmp10, i32* %j
  %25 = load i32, i32* %j
  %26 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %25
  %27 = load i32, i32* %26
  %28 = load i32, i32* %Pivot
  %29 = icmp sle i32 %27, %28
  %30 = zext i1 %29 to i32
  %.cmpneqi11 = icmp ne i32 0, %30
  br i1 %.cmpneqi11, label %.then12, label %.merge13

whileEnd9:                                        ; preds = %.then12, %whileStart7
  %31 = load i32, i32* %i
  %32 = load i32, i32* %j
  %33 = icmp slt i32 %31, %32
  %34 = zext i1 %33 to i32
  %.cmpneqi14 = icmp ne i32 0, %34
  br i1 %.cmpneqi14, label %.then15, label %.else

.then12:                                          ; preds = %whileBody8
  br label %whileEnd9

.merge13:                                         ; preds = %whileBody8
  br label %whileStart7

.then15:                                          ; preds = %whileEnd9
  %35 = load i32, i32* %i
  %36 = load i32, i32* %j
  call void @swap(i32 %35, i32 %36)
  br label %.merge16

.else:                                            ; preds = %whileEnd9
  br label %whileEnd

.merge16:                                         ; preds = %.then15
  br label %whileStart

.else20:                                          ; preds = %entryQSort
  %37 = load i32, i32* %Left
  %38 = load i32, i32* %Right
  %39 = load i32, i32* %Left
  %subtmp21 = sub i32 %38, %39
  %addtmp22 = add i32 %subtmp21, 1
  call void @InsertionSort(i32 %37, i32 %addtmp22)
  br label %.merge23

.merge23:                                         ; preds = %.else20, %whileEnd
  ret void
}

define void @QuickSort(i32 %0) {
entryQuickSort:
  %N = alloca i32
  store i32 %0, i32* %N
  %1 = load i32, i32* %N
  %subtmp = sub i32 %1, 1
  call void @QSort(i32 0, i32 %subtmp)
  ret void
}

define i32 @fly() {
entryfly:
  %N = alloca i32
  %temp = alloca i32
  %0 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* %N)
  %i = alloca i32
  store i32 0, i32* %i
  br label %BBForCond

BBForCond:                                        ; preds = %BBForStart, %entryfly
  %1 = load i32, i32* %i
  %2 = load i32, i32* %N
  %3 = icmp slt i32 %1, %2
  %4 = zext i1 %3 to i32
  %.cmpneqi = icmp ne i32 0, %4
  br i1 %.cmpneqi, label %BBForBody, label %BBForEnd

BBForStart:                                       ; preds = %BBForBody
  %5 = load i32, i32* %i
  %addtmp = add i32 %5, 1
  store i32 %addtmp, i32* %i
  br label %BBForCond

BBForBody:                                        ; preds = %BBForCond
  %6 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* %temp)
  %7 = load i32, i32* %i
  %8 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %7
  %9 = load i32, i32* %temp
  store i32 %9, i32* %8
  br label %BBForStart

BBForEnd:                                         ; preds = %BBForCond
  %10 = load i32, i32* %N
  call void @QuickSort(i32 %10)
  %j = alloca i32
  store i32 0, i32* %j
  br label %BBForCond1

BBForCond1:                                       ; preds = %BBForStart2, %BBForEnd
  %11 = load i32, i32* %j
  %12 = load i32, i32* %N
  %13 = icmp slt i32 %11, %12
  %14 = zext i1 %13 to i32
  %.cmpneqi5 = icmp ne i32 0, %14
  br i1 %.cmpneqi5, label %BBForBody3, label %BBForEnd4

BBForStart2:                                      ; preds = %BBForBody3
  %15 = load i32, i32* %j
  %addtmp6 = add i32 %15, 1
  store i32 %addtmp6, i32* %j
  br label %BBForCond1

BBForBody3:                                       ; preds = %BBForCond1
  %16 = load i32, i32* %j
  %17 = getelementptr [10000 x i32], [10000 x i32]* @arr, i32 0, i32 %16
  %18 = load i32, i32* %17
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), i32 %18)
  br label %BBForStart2

BBForEnd4:                                        ; preds = %BBForCond1
  ret i32 0
}
