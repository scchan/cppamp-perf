; ModuleID = '/tmp/oo.bc'
target datalayout = "e-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024"
target triple = "spir64-unknown-unknown"

@n = addrspace(1) global i32 10, align 4
@__wg_scratch = common addrspace(3) global [4 x i64] zeroinitializer, align 8

; Function Attrs: nounwind uwtable
define spir_kernel void @ZZ4mainEN3_EC__019__cxxamp_trampolineEPi(i32 addrspace(1)* nocapture) #0 align 2 {
  %2 = tail call spir_func i32 @__hsail_get_global_id(i32 0) #2
  %3 = load i32 addrspace(1)* @n, align 4, !tbaa !8
  %4 = add nsw i32 %3, 1
  %5 = sext i32 %2 to i64
  %6 = getelementptr inbounds i32 addrspace(1)* %0, i64 %5
  %7 = load i32 addrspace(1)* %6, align 4, !tbaa !8
  %8 = add nsw i32 %4, %7
  store i32 %8, i32 addrspace(1)* %6, align 4, !tbaa !8
  ret void
}

; Function Attrs: nounwind readnone
declare spir_func i32 @__hsail_get_global_id(i32) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-realign-stack" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone }

!opencl.enable.FP_CONTRACT = !{}
!opencl.spir.version = !{!0}
!opencl.ocl.version = !{!0}
!opencl.used.extensions = !{!0}
!opencl.used.optional.core.features = !{!0}
!opencl.compiler.options = !{!0}
!opencl.kernels = !{!1}
!llvm.ident = !{!7}

!0 = metadata !{}
!1 = metadata !{void (i32 addrspace(1)*)* @ZZ4mainEN3_EC__019__cxxamp_trampolineEPi, metadata !2, metadata !3, metadata !4, metadata !5, metadata !6}
!2 = metadata !{metadata !"kernel_arg_addr_space", i32 0}
!3 = metadata !{metadata !"kernel_arg_access_qual", metadata !"none"}
!4 = metadata !{metadata !"kernel_arg_type", metadata !"int*"}
!5 = metadata !{metadata !"kernel_arg_type_qual", metadata !""}
!6 = metadata !{metadata !"kernel_arg_name", metadata !""}
!7 = metadata !{metadata !"Kalmar clang version 3.5.0 (tags/RELEASE_350/final) (based on Kalmar 0.6.0-5127583-02e68c9 LLVM 3.5.0svn)"}
!8 = metadata !{metadata !9, metadata !9, i64 0}
!9 = metadata !{metadata !"int", metadata !10, i64 0}
!10 = metadata !{metadata !"omnipotent char", metadata !11, i64 0}
!11 = metadata !{metadata !"Simple C/C++ TBAA"}
