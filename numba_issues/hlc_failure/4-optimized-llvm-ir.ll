; ModuleID = '/tmp/tmp7f8ixwcv/3-linked-path'
target datalayout = "e-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-n32"
target triple = "hsail64-pc-unknown-amdopencl"

@__wg_scratch = common addrspace(3) global [4 x i64] zeroinitializer, align 8

; Function Attrs: nounwind
define spir_kernel void @copy(float addrspace(1)* nocapture readonly %input, float addrspace(1)* nocapture %output) #0 {
  %1 = load float addrspace(1)* %input, align 4, !tbaa !8
  store float %1, float addrspace(1)* %output, align 4, !tbaa !8
  ret void
}

attributes #0 = { nounwind }

!opencl.kernels = !{!0}
!opencl.enable.FP_CONTRACT = !{}
!opencl.spir.version = !{!6}
!opencl.ocl.version = !{!6}
!opencl.used.extensions = !{!7}
!opencl.used.optional.core.features = !{!7}
!opencl.compiler.options = !{!7}

!0 = metadata !{void (float addrspace(1)*, float addrspace(1)*)* @copy, metadata !1, metadata !2, metadata !3, metadata !4, metadata !5}
!1 = metadata !{metadata !"kernel_arg_addr_space", i32 1, i32 1}
!2 = metadata !{metadata !"kernel_arg_access_qual", metadata !"none", metadata !"none"}
!3 = metadata !{metadata !"kernel_arg_type", metadata !"float*", metadata !"float*"}
!4 = metadata !{metadata !"kernel_arg_type_qual", metadata !"", metadata !""}
!5 = metadata !{metadata !"kernel_arg_base_type", metadata !"float*", metadata !"float*"}
!6 = metadata !{i32 1, i32 2}
!7 = metadata !{}
!8 = metadata !{metadata !9, metadata !9, i64 0}
!9 = metadata !{metadata !"float", metadata !10}
!10 = metadata !{metadata !"omnipotent char", metadata !11}
!11 = metadata !{metadata !"Simple C/C++ TBAA"}
