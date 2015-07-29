; ModuleID = '/tmp/tmp1u3130eg/5-linked-path'
target datalayout = "e-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-n32"
target triple = "hsail64-pc-unknown-amdopencl"

@_hsapy_smem = internal addrspace(3) global [2 x i64] zeroinitializer
@__wg_scratch = common addrspace(3) global [4 x i64] zeroinitializer, align 8

; Function Attrs: nounwind
define spir_kernel void @hsaPy_hsapy_devfn__5F__5F_main_5F__5F__2E_foo_24_1_2E_array_28_int64_2C__20_1d_2C__20_C_29_(i8 addrspace(1)* nocapture readnone %_dot_.1, i8 addrspace(1)* nocapture readnone %_dot_.2, i64 %_dot_.3, i64 %_dot_.4, i64 addrspace(1)* nocapture %_dot_.5, i64 %_dot_.6, i64 %_dot_.7) #0 {
.9:
  %0 = tail call spir_func i32 @__hsail_get_local_id(i32 0) #3
  %1 = zext i32 %0 to i64
  %_dot_.12835.i = getelementptr [2 x i64] addrspace(3)* @_hsapy_smem, i64 0, i64 %1
  %_dot_.128.i = addrspacecast i64 addrspace(3)* %_dot_.12835.i to i64 addrspace(4)*
  store i64 666, i64 addrspace(4)* %_dot_.128.i, align 8
  tail call spir_func void @__hsail_barrier() #0
  %_dot_.161.i = icmp eq i32 %0, 0
  br i1 %_dot_.161.i, label %B74.i, label %B97.i

B74.i:                                            ; preds = %.9
  store i64 123, i64 addrspace(4)* addrspacecast (i64 addrspace(3)* getelementptr inbounds ([2 x i64] addrspace(3)* @_hsapy_smem, i64 0, i64 0) to i64 addrspace(4)*), align 8
  store i64 321, i64 addrspace(4)* getelementptr ([2 x i64] addrspace(4)* addrspacecast ([2 x i64] addrspace(3)* @_hsapy_smem to [2 x i64] addrspace(4)*), i64 0, i64 1), align 8
  br label %B97.i

B97.i:                                            ; preds = %B74.i, %.9
  tail call spir_func void @__hsail_barrier() #0
  tail call spir_func void @__hsail_barrier() #0
  %_dot_.294.i = icmp eq i32 %0, 1
  br i1 %_dot_.294.i, label %B135.i, label %hsapy_devfn__5F__5F_main_5F__5F__2E_foo_24_1_2E_array_28_int64_2C__20_1d_2C__20_C_29_.exit

B135.i:                                           ; preds = %B97.i
  %_dot_.325.i = load i64 addrspace(4)* %_dot_.128.i, align 8
  br label %hsapy_devfn__5F__5F_main_5F__5F__2E_foo_24_1_2E_array_28_int64_2C__20_1d_2C__20_C_29_.exit

hsapy_devfn__5F__5F_main_5F__5F__2E_foo_24_1_2E_array_28_int64_2C__20_1d_2C__20_C_29_.exit: ; preds = %B97.i, %B135.i
  %storemerge.i = phi i64 [ %_dot_.325.i, %B135.i ], [ 456, %B97.i ]
  tail call spir_func void @__hsail_barrier() #0
  %_dot_.396.i1 = getelementptr i64 addrspace(1)* %_dot_.5, i64 %1
  %_dot_.396.i = addrspacecast i64 addrspace(1)* %_dot_.396.i1 to i64 addrspace(4)*
  store i64 %storemerge.i, i64 addrspace(4)* %_dot_.396.i, align 8
  ret void
}

declare spir_func void @__hsail_barrier() #1

; Function Attrs: nounwind readnone
declare spir_func i32 @__hsail_get_local_id(i32) #2

attributes #0 = { nounwind }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-realign-stack" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-realign-stack" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readnone }

!opencl.spir.version = !{!0}
!opencl.kernels = !{!1}
!opencl.ocl.version = !{!0}

!0 = metadata !{i32 2, i32 0}
!1 = metadata !{void (i8 addrspace(1)*, i8 addrspace(1)*, i64, i64, i64 addrspace(1)*, i64, i64)* @hsaPy_hsapy_devfn__5F__5F_main_5F__5F__2E_foo_24_1_2E_array_28_int64_2C__20_1d_2C__20_C_29_, metadata !2, metadata !3, metadata !4, metadata !5, metadata !6}
!2 = metadata !{metadata !"kernel_arg_addr_space", i32 1, i32 1, i32 0, i32 0, i32 1, i32 0, i32 0}
!3 = metadata !{metadata !"kernel_arg_access_qual", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none"}
!4 = metadata !{metadata !"kernel_arg_type", metadata !"i8 addrspace(1)* ", metadata !"i8 addrspace(1)* ", metadata !"i64", metadata !"i64", metadata !"i64 addrspace(1)* ", metadata !"i64", metadata !"i64"}
!5 = metadata !{metadata !"kernel_arg_type_qual", metadata !"", metadata !"", metadata !"", metadata !"", metadata !"", metadata !"", metadata !""}
!6 = metadata !{metadata !"kernel_arg_base_type", metadata !"i8 addrspace(1)* ", metadata !"i8 addrspace(1)* ", metadata !"i64", metadata !"i64", metadata !"i64 addrspace(1)* ", metadata !"i64", metadata !"i64"}
