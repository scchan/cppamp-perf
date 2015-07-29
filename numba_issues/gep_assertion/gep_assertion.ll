; ModuleID = '/tmp/tmp0831yp_b/9-linked-path'
target datalayout = "e-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-n32"
target triple = "hsail64-pc-unknown-amdopencl"

@_hsapy_smem = internal addrspace(3) global [64 x i64] zeroinitializer
@__wg_scratch = common addrspace(3) global [4 x i64] zeroinitializer, align 8

; Function Attrs: nounwind
define spir_kernel void @hsaPy_hsapy_devfn__5F__5F_main_5F__5F__2E_TestScan_2E_test_5F_single_5F_block_2E__3C_locals_3E__2E_scan_5F_block_24_4_2E_array_28_int64_2C__20_1d_2C__20_C_29__2E_array_28_int64_2C__20_1d_2C__20_C_29_(i8 addrspace(1)* nocapture readnone %_dot_.2, i64 %_dot_.3, i64 %_dot_.4, i64 addrspace(1)* nocapture %_dot_.5, i64 %_dot_.6, i64 %_dot_.7, i8 addrspace(1)* nocapture readnone %_dot_.8, i64 %_dot_.9, i64 %_dot_.10, i64 addrspace(1)* nocapture %_dot_.11, i64 %_dot_.12, i64 %_dot_.13) #0 {
.14:
  %_dot_.122.i = alloca i64, align 8
  %0 = bitcast i64* %_dot_.122.i to i8*
  call void @llvm.lifetime.start(i64 8, i8* %0)
  %1 = tail call spir_func i32 @__hsail_get_local_id(i32 0) #3
  %2 = zext i32 %1 to i64
  %3 = tail call spir_func i32 @__hsail_get_global_id(i32 0) #3
  %4 = zext i32 %3 to i64
  %5 = tail call spir_func i32 @__hsail_get_group_id(i32 0) #3
  %6 = zext i32 %5 to i64
  %_dot_.96.i1 = getelementptr i64 addrspace(1)* %_dot_.5, i64 %4
  %_dot_.96.i = addrspacecast i64 addrspace(1)* %_dot_.96.i1 to i64 addrspace(4)*
  %_dot_.97.i = load i64 addrspace(4)* %_dot_.96.i, align 8
  %_dot_.11717.i = getelementptr [64 x i64] addrspace(3)* @_hsapy_smem, i64 0, i64 %2
  %_dot_.117.i = addrspacecast i64 addrspace(3)* %_dot_.11717.i to i64 addrspace(4)*
  store i64 %_dot_.97.i, i64 addrspace(4)* %_dot_.117.i, align 8
  store i64 0, i64* %_dot_.122.i, align 8
  %_dot_.126.i = call spir_func i32 @hsapy_devfn__5F__5F_main_5F__5F__2E_device_5F_scan_5F_generic_24_1_2E_int64_2E_array_28_int64_2C__20_1d_2C__20_C_29_(i64* %_dot_.122.i, i64 %2, i8* null, i64 64, i64 8, i64 addrspace(4)* addrspacecast (i64 addrspace(3)* getelementptr inbounds ([64 x i64] addrspace(3)* @_hsapy_smem, i64 0, i64 0) to i64 addrspace(4)*), i64 64, i64 8) #0
  switch i32 %_dot_.126.i, label %B0.if.i [
    i32 -2, label %B0.endif.i
    i32 0, label %B0.endif.i
  ]

B124.i:                                           ; preds = %B0.endif.i
  %_dot_.206.i2 = getelementptr i64 addrspace(1)* %_dot_.11, i64 %6
  %_dot_.206.i = addrspacecast i64 addrspace(1)* %_dot_.206.i2 to i64 addrspace(4)*
  store i64 %_dot_.134.i, i64 addrspace(4)* %_dot_.206.i, align 8
  br label %B137.i

B137.i:                                           ; preds = %B0.endif.i, %B124.i
  call void @llvm.lifetime.end(i64 8, i8* %0)
  br label %hsapy_devfn__5F__5F_main_5F__5F__2E_TestScan_2E_test_5F_single_5F_block_2E__3C_locals_3E__2E_scan_5F_block_24_4_2E_array_28_int64_2C__20_1d_2C__20_C_29__2E_array_28_int64_2C__20_1d_2C__20_C_29_.exit

B0.if.i:                                          ; preds = %.14
  call void @llvm.lifetime.end(i64 8, i8* %0)
  br label %hsapy_devfn__5F__5F_main_5F__5F__2E_TestScan_2E_test_5F_single_5F_block_2E__3C_locals_3E__2E_scan_5F_block_24_4_2E_array_28_int64_2C__20_1d_2C__20_C_29__2E_array_28_int64_2C__20_1d_2C__20_C_29_.exit

B0.endif.i:                                       ; preds = %.14, %.14
  %_dot_.134.i = load i64* %_dot_.122.i, align 8
  %_dot_.158.i = load i64 addrspace(4)* %_dot_.117.i, align 8
  store i64 %_dot_.158.i, i64 addrspace(4)* %_dot_.96.i, align 8
  %_dot_.184.i = icmp eq i32 %1, 0
  br i1 %_dot_.184.i, label %B124.i, label %B137.i

hsapy_devfn__5F__5F_main_5F__5F__2E_TestScan_2E_test_5F_single_5F_block_2E__3C_locals_3E__2E_scan_5F_block_24_4_2E_array_28_int64_2C__20_1d_2C__20_C_29__2E_array_28_int64_2C__20_1d_2C__20_C_29_.exit: ; preds = %B0.if.i, %B137.i
  ret void
}

; Function Attrs: nounwind
define linkonce_odr spir_func i32 @hsapy_devfn__5F__5F_main_5F__5F__2E_device_5F_scan_5F_generic_24_1_2E_int64_2E_array_28_int64_2C__20_1d_2C__20_C_29_(i64* nocapture %_dot_.ret, i64 %arg.tid, i8* %arg.data.0, i64 %arg.data.1, i64 %arg.data.2, i64 addrspace(4)* %arg.data.3, i64 %arg.data.4.0, i64 %arg.data.5.0) #0 {
entry:
  %_dot_.32 = sdiv i64 %arg.data.1, 2
  %_dot_.33 = srem i64 %arg.data.1, 2
  %_dot_.33.lobit = ashr i64 %_dot_.33, 63
  %_dot_.42 = add i64 %_dot_.33.lobit, %_dot_.32
  %_dot_.5371 = icmp sgt i64 %_dot_.42, 0
  tail call spir_func void @__hsail_barrier() #0
  br i1 %_dot_.5371, label %B40.lr.ph, label %B154

B40.lr.ph:                                        ; preds = %entry
  %_dot_.74 = shl i64 %arg.tid, 1
  %_dot_.802 = or i64 %_dot_.74, 1
  %_dot_.104 = add i64 %_dot_.74, 2
  br label %B40

B40:                                              ; preds = %B40.lr.ph, %B130
  %d.0.load6872 = phi i64 [ %_dot_.42, %B40.lr.ph ], [ %_dot_.208, %B130 ]
  %0 = phi i64 [ 1, %B40.lr.ph ], [ %_dot_.187, %B130 ]
  %_dot_.66 = icmp sgt i64 %d.0.load6872, %arg.tid
  br i1 %_dot_.66, label %B65, label %B130

B65:                                              ; preds = %B40
  %_dot_.84 = mul i64 %0, %_dot_.802
  %_dot_.90 = add i64 %_dot_.84, -1
  %_dot_.108 = mul i64 %0, %_dot_.104
  %_dot_.114 = add i64 %_dot_.108, -1
  %_dot_.130 = icmp slt i64 %_dot_.114, 0
  %_dot_.131 = select i1 %_dot_.130, i64 %arg.data.4.0, i64 0
  %_dot_.134 = add i64 %_dot_.131, %_dot_.114
  %_dot_.135 = getelementptr i64 addrspace(4)* %arg.data.3, i64 %_dot_.134
  %_dot_.136 = load i64 addrspace(4)* %_dot_.135, align 8
  %_dot_.150 = icmp slt i64 %_dot_.90, 0
  %_dot_.151 = select i1 %_dot_.150, i64 %arg.data.4.0, i64 0
  %_dot_.154 = add i64 %_dot_.151, %_dot_.90
  %_dot_.155 = getelementptr i64 addrspace(4)* %arg.data.3, i64 %_dot_.154
  %_dot_.156 = load i64 addrspace(4)* %_dot_.155, align 8
  %_dot_.160 = add i64 %_dot_.156, %_dot_.136
  store i64 %_dot_.160, i64 addrspace(4)* %_dot_.135, align 8
  br label %B130

B130:                                             ; preds = %B65, %B40
  %_dot_.187 = shl i64 %0, 1
  %_dot_.198 = sdiv i64 %d.0.load6872, 2
  %_dot_.199 = srem i64 %d.0.load6872, 2
  %_dot_.199.lobit = ashr i64 %_dot_.199, 63
  %_dot_.208 = add i64 %_dot_.199.lobit, %_dot_.198
  %_dot_.53 = icmp sgt i64 %_dot_.208, 0
  tail call spir_func void @__hsail_barrier() #0
  br i1 %_dot_.53, label %B40, label %B154.loopexit

B154.loopexit:                                    ; preds = %B130
  br label %B154

B154:                                             ; preds = %B154.loopexit, %entry
  %_dot_.226 = add i64 %arg.data.1, -1
  %_dot_.240 = icmp slt i64 %_dot_.226, 0
  %_dot_.241 = select i1 %_dot_.240, i64 %arg.data.4.0, i64 0
  %_dot_.244 = add i64 %_dot_.241, %_dot_.226
  %_dot_.245 = getelementptr i64 addrspace(4)* %arg.data.3, i64 %_dot_.244
  %_dot_.246 = load i64 addrspace(4)* %_dot_.245, align 8
  tail call spir_func void @__hsail_barrier() #0
  %_dot_.261 = icmp eq i64 %arg.tid, 0
  br i1 %_dot_.261, label %B206, label %B238.preheader

B206:                                             ; preds = %B154
  store i64 0, i64 addrspace(4)* %_dot_.245, align 8
  br label %B238.preheader

B238.preheader:                                   ; preds = %B206, %B154
  %_dot_.30469 = icmp sgt i64 %arg.data.1, 1
  br i1 %_dot_.30469, label %B250.lr.ph, label %B384

B250.lr.ph:                                       ; preds = %B238.preheader
  %_dot_.347 = shl i64 %arg.tid, 1
  %_dot_.3531 = or i64 %_dot_.347, 1
  %_dot_.377 = add i64 %_dot_.347, 2
  br label %B250

B250:                                             ; preds = %B250.lr.ph, %B370
  %n.0.load176770 = phi i64 [ %arg.data.1, %B250.lr.ph ], [ %_dot_.325, %B370 ]
  %1 = phi i64 [ 1, %B250.lr.ph ], [ %_dot_.502, %B370 ]
  %_dot_.315 = sdiv i64 %n.0.load176770, 2
  %_dot_.316 = srem i64 %n.0.load176770, 2
  %_dot_.316.lobit = ashr i64 %_dot_.316, 63
  %_dot_.325 = add i64 %_dot_.316.lobit, %_dot_.315
  tail call spir_func void @__hsail_barrier() #0
  %_dot_.339 = icmp sgt i64 %1, %arg.tid
  br i1 %_dot_.339, label %B285, label %B370

B285:                                             ; preds = %B250
  %_dot_.357 = mul i64 %_dot_.325, %_dot_.3531
  %_dot_.363 = add i64 %_dot_.357, -1
  %_dot_.381 = mul i64 %_dot_.325, %_dot_.377
  %_dot_.387 = add i64 %_dot_.381, -1
  %_dot_.403 = icmp slt i64 %_dot_.363, 0
  %_dot_.404 = select i1 %_dot_.403, i64 %arg.data.4.0, i64 0
  %_dot_.407 = add i64 %_dot_.404, %_dot_.363
  %_dot_.408 = getelementptr i64 addrspace(4)* %arg.data.3, i64 %_dot_.407
  %_dot_.409 = load i64 addrspace(4)* %_dot_.408, align 8
  %_dot_.425 = icmp slt i64 %_dot_.387, 0
  %_dot_.426 = select i1 %_dot_.425, i64 %arg.data.4.0, i64 0
  %_dot_.429 = add i64 %_dot_.426, %_dot_.387
  %_dot_.430 = getelementptr i64 addrspace(4)* %arg.data.3, i64 %_dot_.429
  %_dot_.431 = load i64 addrspace(4)* %_dot_.430, align 8
  store i64 %_dot_.431, i64 addrspace(4)* %_dot_.408, align 8
  %_dot_.471 = load i64 addrspace(4)* %_dot_.430, align 8
  %_dot_.475 = add i64 %_dot_.471, %_dot_.409
  store i64 %_dot_.475, i64 addrspace(4)* %_dot_.430, align 8
  br label %B370

B370:                                             ; preds = %B250, %B285
  %_dot_.502 = shl i64 %1, 1
  %_dot_.304 = icmp slt i64 %_dot_.502, %arg.data.1
  br i1 %_dot_.304, label %B250, label %B384.loopexit

B384.loopexit:                                    ; preds = %B370
  br label %B384

B384:                                             ; preds = %B384.loopexit, %B238.preheader
  tail call spir_func void @__hsail_barrier() #0
  store i64 %_dot_.246, i64* %_dot_.ret, align 8
  ret i32 0
}

declare spir_func void @__hsail_barrier() #1

; Function Attrs: nounwind readnone
declare spir_func i32 @__hsail_get_global_id(i32) #2

; Function Attrs: nounwind readnone
declare spir_func i32 @__hsail_get_local_id(i32) #2

; Function Attrs: nounwind readnone
declare spir_func i32 @__hsail_get_group_id(i32) #2

; Function Attrs: nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #0

; Function Attrs: nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #0

attributes #0 = { nounwind }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-realign-stack" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-realign-stack" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readnone }

!opencl.ocl.version = !{!0}
!opencl.spir.version = !{!0}
!opencl.kernels = !{!1}

!0 = metadata !{i32 2, i32 0}
!1 = metadata !{void (i8 addrspace(1)*, i64, i64, i64 addrspace(1)*, i64, i64, i8 addrspace(1)*, i64, i64, i64 addrspace(1)*, i64, i64)* @hsaPy_hsapy_devfn__5F__5F_main_5F__5F__2E_TestScan_2E_test_5F_single_5F_block_2E__3C_locals_3E__2E_scan_5F_block_24_4_2E_array_28_int64_2C__20_1d_2C__20_C_29__2E_array_28_int64_2C__20_1d_2C__20_C_29_, metadata !2, metadata !3, metadata !4, metadata !5, metadata !6}
!2 = metadata !{metadata !"kernel_arg_addr_space", i32 1, i32 0, i32 0, i32 1, i32 0, i32 0, i32 1, i32 0, i32 0, i32 1, i32 0, i32 0}
!3 = metadata !{metadata !"kernel_arg_access_qual", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none", metadata !"none"}
!4 = metadata !{metadata !"kernel_arg_type", metadata !"i8 addrspace(1)* ", metadata !"i64", metadata !"i64", metadata !"i64 addrspace(1)* ", metadata !"i64", metadata !"i64", metadata !"i8 addrspace(1)* ", metadata !"i64", metadata !"i64", metadata !"i64 addrspace(1)* ", metadata !"i64", metadata !"i64"}
!5 = metadata !{metadata !"kernel_arg_type_qual", metadata !"", metadata !"", metadata !"", metadata !"", metadata !"", metadata !"", metadata !"", metadata !"", metadata !"", metadata !"", metadata !"", metadata !""}
!6 = metadata !{metadata !"kernel_arg_base_type", metadata !"i8 addrspace(1)* ", metadata !"i64", metadata !"i64", metadata !"i64 addrspace(1)* ", metadata !"i64", metadata !"i64", metadata !"i8 addrspace(1)* ", metadata !"i64", metadata !"i64", metadata !"i64 addrspace(1)* ", metadata !"i64", metadata !"i64"}
