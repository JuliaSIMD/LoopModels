; ModuleID = './depth2boundsSLrefsDN/llvmir.ll'
source_filename = "foo"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128-ni:10:11:12:13"
target triple = "x86_64-unknown-linux-gnu"

define i64 @julia_foo_126({} addrspace(10)* nocapture nonnull readonly align 16 dereferenceable(40) %0) local_unnamed_addr #0 !dbg !5 {
top:
  %1 = tail call {}*** @julia.get_pgcstack()
  %2 = bitcast {} addrspace(10)* %0 to {} addrspace(10)* addrspace(10)*, !dbg !7
  %3 = addrspacecast {} addrspace(10)* addrspace(10)* %2 to {} addrspace(10)* addrspace(11)*, !dbg !7
  %4 = getelementptr inbounds {} addrspace(10)*, {} addrspace(10)* addrspace(11)* %3, i64 3, !dbg !7
  %5 = bitcast {} addrspace(10)* addrspace(11)* %4 to i64 addrspace(11)*, !dbg !7
  %6 = load i64, i64 addrspace(11)* %5, align 8, !dbg !7, !tbaa !11, !range !15, !invariant.load !4
  %.not.not = icmp eq i64 %6, 0, !dbg !16
  br i1 %.not.not, label %L63, label %L14.preheader, !dbg !10

L14.preheader:                                    ; preds = %top
  %7 = getelementptr inbounds {} addrspace(10)*, {} addrspace(10)* addrspace(11)* %3, i64 4
  %8 = bitcast {} addrspace(10)* addrspace(11)* %7 to i64 addrspace(11)*
  %9 = load i64, i64 addrspace(11)* %8, align 8, !tbaa !11, !range !15, !invariant.load !4
  %10 = tail call i64 @llvm.umax.i64(i64 %9, i64 3), !dbg !27
  %11 = icmp ult i64 %9, 4
  %12 = bitcast {} addrspace(10)* %0 to i64 addrspace(13)* addrspace(10)*
  %13 = addrspacecast i64 addrspace(13)* addrspace(10)* %12 to i64 addrspace(13)* addrspace(11)*
  %14 = load i64 addrspace(13)*, i64 addrspace(13)* addrspace(11)* %13, align 8
  br label %L14, !dbg !28

L14:                                              ; preds = %L14.preheader, %L50
  %value_phi3 = phi i64 [ %25, %L50 ], [ 1, %L14.preheader ]
  %value_phi5 = phi i64 [ %value_phi15, %L50 ], [ 0, %L14.preheader ]
  br i1 %11, label %L50, label %L30.preheader, !dbg !28

L30.preheader:                                    ; preds = %L14
  br label %L30, !dbg !29

L30:                                              ; preds = %L30.preheader, %L30
  %value_phi9 = phi i64 [ %23, %L30 ], [ %value_phi5, %L30.preheader ]
  %value_phi10 = phi i64 [ %24, %L30 ], [ 4, %L30.preheader ]
  %15 = mul i64 %value_phi10, %value_phi3, !dbg !30
  %16 = xor i64 %value_phi10, -1, !dbg !32
  %17 = add nsw i64 %value_phi3, %16, !dbg !32
  %18 = add i64 %15, 7, !dbg !32
  %19 = mul i64 %18, %6, !dbg !32
  %20 = add i64 %17, %19, !dbg !32
  %21 = getelementptr inbounds i64, i64 addrspace(13)* %14, i64 %20, !dbg !32
  %22 = load i64, i64 addrspace(13)* %21, align 8, !dbg !32, !tbaa !34
  %23 = add i64 %22, %value_phi9, !dbg !37
  %.not.not6 = icmp eq i64 %value_phi10, %10, !dbg !39
  %24 = add nuw nsw i64 %value_phi10, 1, !dbg !42
  br i1 %.not.not6, label %L50.loopexit, label %L30, !dbg !29

L50.loopexit:                                     ; preds = %L30
  %.lcssa = phi i64 [ %23, %L30 ], !dbg !37
  br label %L50, !dbg !39

L50:                                              ; preds = %L50.loopexit, %L14
  %value_phi15 = phi i64 [ %value_phi5, %L14 ], [ %.lcssa, %L50.loopexit ]
  %.not = icmp eq i64 %value_phi3, %6, !dbg !39
  %25 = add nuw nsw i64 %value_phi3, 1, !dbg !42
  br i1 %.not, label %L63.loopexit, label %L14, !dbg !29

L63.loopexit:                                     ; preds = %L50
  %value_phi15.lcssa = phi i64 [ %value_phi15, %L50 ]
  br label %L63, !dbg !43

L63:                                              ; preds = %L63.loopexit, %top
  %value_phi19 = phi i64 [ 0, %top ], [ %value_phi15.lcssa, %L63.loopexit ]
  ret i64 %value_phi19, !dbg !43
}

define nonnull {} addrspace(10)* @jfptr_foo_127({} addrspace(10)* nocapture readnone %0, {} addrspace(10)** nocapture readonly %1, i32 %2) local_unnamed_addr #1 {
top:
  %3 = tail call {}*** @julia.get_pgcstack()
  %4 = load {} addrspace(10)*, {} addrspace(10)** %1, align 8, !nonnull !4, !dereferenceable !44, !align !45
  %5 = tail call i64 @julia_foo_126({} addrspace(10)* %4) #0
  %6 = tail call nonnull {} addrspace(10)* @jl_box_int64(i64 signext %5)
  ret {} addrspace(10)* %6
}

declare {}*** @julia.get_pgcstack() local_unnamed_addr

declare nonnull {} addrspace(10)* @jl_box_int64(i64 signext) local_unnamed_addr

; Function Attrs: nocallback nofree nosync nounwind readnone speculatable willreturn
declare i64 @llvm.umax.i64(i64, i64) #2

attributes #0 = { "probe-stack"="inline-asm" }
attributes #1 = { "probe-stack"="inline-asm" "thunk" }
attributes #2 = { nocallback nofree nosync nounwind readnone speculatable willreturn }

!llvm.module.flags = !{!0, !1}
!llvm.dbg.cu = !{!2}

!0 = !{i32 2, !"Dwarf Version", i32 4}
!1 = !{i32 2, !"Debug Info Version", i32 3}
!2 = distinct !DICompileUnit(language: DW_LANG_Julia, file: !3, producer: "julia", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, nameTableKind: GNU)
!3 = !DIFile(filename: "/home/sumiya11/loops/try2/LoopModels/experiments/depth2boundsSLrefsDN/source.jl", directory: ".")
!4 = !{}
!5 = distinct !DISubprogram(name: "foo", linkageName: "julia_foo_126", scope: null, file: !3, line: 3, type: !6, scopeLine: 3, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!6 = !DISubroutineType(types: !4)
!7 = !DILocation(line: 150, scope: !8, inlinedAt: !10)
!8 = distinct !DISubprogram(name: "size;", linkageName: "size", scope: !9, file: !9, type: !6, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!9 = !DIFile(filename: "array.jl", directory: ".")
!10 = !DILocation(line: 5, scope: !5)
!11 = !{!12, !12, i64 0, i64 1}
!12 = !{!"jtbaa_const", !13, i64 0}
!13 = !{!"jtbaa", !14, i64 0}
!14 = !{!"jtbaa"}
!15 = !{i64 0, i64 9223372036854775807}
!16 = !DILocation(line: 83, scope: !17, inlinedAt: !19)
!17 = distinct !DISubprogram(name: "<;", linkageName: "<", scope: !18, file: !18, type: !6, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!18 = !DIFile(filename: "int.jl", directory: ".")
!19 = !DILocation(line: 378, scope: !20, inlinedAt: !22)
!20 = distinct !DISubprogram(name: ">;", linkageName: ">", scope: !21, file: !21, type: !6, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!21 = !DIFile(filename: "operators.jl", directory: ".")
!22 = !DILocation(line: 609, scope: !23, inlinedAt: !25)
!23 = distinct !DISubprogram(name: "isempty;", linkageName: "isempty", scope: !24, file: !24, type: !6, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!24 = !DIFile(filename: "range.jl", directory: ".")
!25 = !DILocation(line: 833, scope: !26, inlinedAt: !10)
!26 = distinct !DISubprogram(name: "iterate;", linkageName: "iterate", scope: !24, file: !24, type: !6, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!27 = !DILocation(line: 0, scope: !5)
!28 = !DILocation(line: 6, scope: !5)
!29 = !DILocation(line: 7, scope: !5)
!30 = !DILocation(line: 88, scope: !31, inlinedAt: !29)
!31 = distinct !DISubprogram(name: "*;", linkageName: "*", scope: !18, file: !18, type: !6, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!32 = !DILocation(line: 862, scope: !33, inlinedAt: !29)
!33 = distinct !DISubprogram(name: "getindex;", linkageName: "getindex", scope: !9, file: !9, type: !6, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!34 = !{!35, !35, i64 0}
!35 = !{!"jtbaa_arraybuf", !36, i64 0}
!36 = !{!"jtbaa_data", !13, i64 0}
!37 = !DILocation(line: 87, scope: !38, inlinedAt: !29)
!38 = distinct !DISubprogram(name: "+;", linkageName: "+", scope: !18, file: !18, type: !6, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!39 = !DILocation(line: 468, scope: !40, inlinedAt: !42)
!40 = distinct !DISubprogram(name: "==;", linkageName: "==", scope: !41, file: !41, type: !6, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!41 = !DIFile(filename: "promotion.jl", directory: ".")
!42 = !DILocation(line: 837, scope: !26, inlinedAt: !29)
!43 = !DILocation(line: 10, scope: !5)
!44 = !{i64 40}
!45 = !{i64 16}
