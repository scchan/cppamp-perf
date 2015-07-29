; ModuleID = '/tmp/tmp1u3130eg/2-dump-llvm-ir'
target datalayout = "e-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-n32"
target triple = "hsail64-pc-unknown-amdopencl"

@_hsapy_smem = internal addrspace(3) global [2 x i64] zeroinitializer

define internal spir_func i32 @hsapy_devfn__5F__5F_main_5F__5F__2E_foo_24_1_2E_array_28_int64_2C__20_1d_2C__20_C_29_(i8** %_dot_.ret, i8* %arg.out.0, i8* %arg.out.1, i64 %arg.out.2, i64 %arg.out.3, i64 addrspace(4)* %arg.out.4, i64 %arg.out.5.0, i64 %arg.out.6.0) {
entry:
  %inserted.meminfo = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } undef, i8* %arg.out.0, 0
  %inserted.parent = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.meminfo, i8* %arg.out.1, 1
  %inserted.nitems = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.parent, i64 %arg.out.2, 2
  %inserted.itemsize = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.nitems, i64 %arg.out.3, 3
  %inserted.data = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.itemsize, i64 addrspace(4)* %arg.out.4, 4
  %_dot_.10 = insertvalue [1 x i64] undef, i64 %arg.out.5.0, 0
  %inserted.shape = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.data, [1 x i64] %_dot_.10, 5
  %_dot_.11 = insertvalue [1 x i64] undef, i64 %arg.out.6.0, 0
  %inserted.strides = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.shape, [1 x i64] %_dot_.11, 6
  %out = alloca { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %out
  %"$0.1" = alloca i8*
  store i8* null, i8** %"$0.1"
  %"$0.2" = alloca i8*
  store i8* null, i8** %"$0.2"
  %"$0.3" = alloca i8*
  store i8* null, i8** %"$0.3"
  %"$const0.4" = alloca i32
  store i32 0, i32* %"$const0.4"
  %"$0.6" = alloca i8*
  store i8* null, i8** %"$0.6"
  %_dot_.37 = alloca { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.37
  %"$0.7" = alloca { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %"$0.7"
  %sm = alloca { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  %"$0.8" = alloca i8*
  store i8* null, i8** %"$0.8"
  %"$0.9" = alloca i8*
  store i8* null, i8** %"$0.9"
  %"$const0.10" = alloca i32
  store i32 0, i32* %"$const0.10"
  %"$0.11" = alloca i64
  store i64 0, i64* %"$0.11"
  %tid = alloca i64
  store i64 0, i64* %tid
  %"$const0.12" = alloca i32
  store i32 0, i32* %"$const0.12"
  %_dot_.110 = alloca { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.110
  %"$0.15" = alloca i8*
  store i8* null, i8** %"$0.15"
  %"$0.16" = alloca i8*
  store i8* null, i8** %"$0.16"
  %"$const0.17" = alloca i32
  store i32 0, i32* %"$const0.17"
  %"$0.18" = alloca i8*
  store i8* null, i8** %"$0.18"
  %"$const0.20" = alloca i32
  store i32 0, i32* %"$const0.20"
  %"$0.21" = alloca i1
  store i1 false, i1* %"$0.21"
  %"$const74.1" = alloca i32
  store i32 0, i32* %"$const74.1"
  %"$const74.3" = alloca i32
  store i32 0, i32* %"$const74.3"
  %_dot_.180 = alloca { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.180
  %"$const74.4" = alloca i32
  store i32 0, i32* %"$const74.4"
  %"$const74.6" = alloca i32
  store i32 0, i32* %"$const74.6"
  %_dot_.215 = alloca { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.215
  %"$97.1" = alloca i8*
  store i8* null, i8** %"$97.1"
  %"$97.2" = alloca i8*
  store i8* null, i8** %"$97.2"
  %"$const97.3" = alloca i32
  store i32 0, i32* %"$const97.3"
  %"$97.4" = alloca i8*
  store i8* null, i8** %"$97.4"
  %"$97.5" = alloca i8*
  store i8* null, i8** %"$97.5"
  %"$97.6" = alloca i8*
  store i8* null, i8** %"$97.6"
  %"$const97.7" = alloca i32
  store i32 0, i32* %"$const97.7"
  %"$97.8" = alloca i8*
  store i8* null, i8** %"$97.8"
  %"$const97.10" = alloca i32
  store i32 0, i32* %"$const97.10"
  %"$97.11" = alloca i1
  store i1 false, i1* %"$97.11"
  %_dot_.306 = alloca { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.306
  %"$135.3" = alloca i64
  store i64 0, i64* %"$135.3"
  %val = alloca i64
  store i64 0, i64* %val
  %"$const148.1" = alloca i32
  store i32 0, i32* %"$const148.1"
  %"$154.1" = alloca i8*
  store i8* null, i8** %"$154.1"
  %"$154.2" = alloca i8*
  store i8* null, i8** %"$154.2"
  %"$const154.3" = alloca i32
  store i32 0, i32* %"$const154.3"
  %"$154.4" = alloca i8*
  store i8* null, i8** %"$154.4"
  %_dot_.378 = alloca { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.378
  %"$const154.8" = alloca i8*
  store i8* null, i8** %"$const154.8"
  %"$154.9" = alloca i8*
  store i8* null, i8** %"$154.9"
  br label %B0

B0:                                               ; preds = %entry
  %_dot_.13 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %out
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.strides, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %out
  %_dot_.16 = load i8** %"$0.1"
  store i8* null, i8** %"$0.1"
  %_dot_.18 = load i8** %"$0.1"
  %_dot_.20 = load i8** %"$0.2"
  store i8* null, i8** %"$0.2"
  %_dot_.22 = load i8** %"$0.1"
  store i8* null, i8** %"$0.1"
  %_dot_.24 = load i8** %"$0.2"
  %_dot_.26 = load i8** %"$0.3"
  store i8* null, i8** %"$0.3"
  %_dot_.28 = load i8** %"$0.2"
  store i8* null, i8** %"$0.2"
  %_dot_.31 = load i32* %"$const0.4"
  store i32 2, i32* %"$const0.4"
  %_dot_.34 = load i8** %"$0.6"
  store i8* null, i8** %"$0.6"
  %_dot_.36 = addrspacecast [2 x i64] addrspace(3)* @_hsapy_smem to [2 x i64] addrspace(4)*
  %_dot_.39 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.37, i32 0, i32 4
  %_dot_.40 = load i64 addrspace(4)** %_dot_.39
  %_dot_.41 = bitcast [2 x i64] addrspace(4)* %_dot_.36 to i64 addrspace(4)*
  %_dot_.42 = insertvalue [1 x i64] undef, i64 2, 0
  %_dot_.43 = insertvalue [1 x i64] undef, i64 8, 0
  %_dot_.44 = extractvalue [1 x i64] %_dot_.42, 0
  %_dot_.45 = mul i64 1, %_dot_.44
  %_dot_.46 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.37, i32 0, i32 0
  store i8* null, i8** %_dot_.46
  %_dot_.48 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.37, i32 0, i32 2
  store i64 %_dot_.45, i64* %_dot_.48
  %_dot_.50 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.37, i32 0, i32 6
  store [1 x i64] %_dot_.43, [1 x i64]* %_dot_.50
  %_dot_.52 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.37, i32 0, i32 5
  store [1 x i64] %_dot_.42, [1 x i64]* %_dot_.52
  %_dot_.54 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.37, i32 0, i32 4
  store i64 addrspace(4)* %_dot_.41, i64 addrspace(4)** %_dot_.54
  %_dot_.56 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.37, i32 0, i32 3
  store i64 8, i64* %_dot_.56
  %_dot_.58 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.37, i32 0, i32 1
  store i8* null, i8** %_dot_.58
  %_dot_.60 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.37
  %_dot_.62 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %"$0.7"
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %_dot_.60, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %"$0.7"
  %_dot_.64 = load i32* %"$const0.4"
  store i32 0, i32* %"$const0.4"
  %_dot_.66 = load i8** %"$0.6"
  store i8* null, i8** %"$0.6"
  %_dot_.68 = load i8** %"$0.3"
  store i8* null, i8** %"$0.3"
  %_dot_.70 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %"$0.7"
  %_dot_.72 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %_dot_.70, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  %_dot_.74 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %"$0.7"
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %"$0.7"
  %_dot_.77 = load i8** %"$0.8"
  store i8* null, i8** %"$0.8"
  %_dot_.79 = load i8** %"$0.8"
  %_dot_.81 = load i8** %"$0.9"
  store i8* null, i8** %"$0.9"
  %_dot_.83 = load i8** %"$0.8"
  store i8* null, i8** %"$0.8"
  %_dot_.86 = load i32* %"$const0.10"
  store i32 0, i32* %"$const0.10"
  %_dot_.88 = load i32* %"$const0.10"
  %_dot_.89 = call spir_func i64 @_Z12get_local_idj(i32 %_dot_.88)
  %_dot_.91 = load i64* %"$0.11"
  store i64 %_dot_.89, i64* %"$0.11"
  %_dot_.93 = load i32* %"$const0.10"
  store i32 0, i32* %"$const0.10"
  %_dot_.95 = load i8** %"$0.9"
  store i8* null, i8** %"$0.9"
  %_dot_.97 = load i64* %"$0.11"
  %_dot_.99 = load i64* %tid
  store i64 %_dot_.97, i64* %tid
  %_dot_.101 = load i64* %"$0.11"
  store i64 0, i64* %"$0.11"
  %_dot_.104 = load i32* %"$const0.12"
  store i32 666, i32* %"$const0.12"
  %_dot_.106 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  %_dot_.107 = load i32* %"$const0.12"
  %_dot_.108 = load i64* %tid
  %_dot_.109 = sext i32 %_dot_.107 to i64
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %_dot_.106, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.110
  %_dot_.113 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.110, i32 0, i32 5
  %_dot_.114 = getelementptr inbounds [1 x i64]* %_dot_.113, i32 0, i32 0
  %_dot_.115 = load i64* %_dot_.114, !range !7
  %_dot_.116 = insertvalue [1 x i64] undef, i64 %_dot_.115, 0
  %_dot_.117 = extractvalue [1 x i64] %_dot_.116, 0
  %_dot_.118 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.110, i32 0, i32 6
  %_dot_.119 = load [1 x i64]* %_dot_.118
  %_dot_.120 = extractvalue [1 x i64] %_dot_.119, 0
  %_dot_.121 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.110, i32 0, i32 4
  %_dot_.122 = load i64 addrspace(4)** %_dot_.121
  %_dot_.123 = icmp slt i64 %_dot_.108, 0
  %_dot_.124 = add i64 %_dot_.117, %_dot_.108
  %_dot_.125 = select i1 %_dot_.123, i64 %_dot_.124, i64 %_dot_.108
  %_dot_.126 = mul i64 %_dot_.125, 1
  %_dot_.127 = add i64 0, %_dot_.126
  %_dot_.128 = getelementptr i64 addrspace(4)* %_dot_.122, i64 %_dot_.127
  store i64 %_dot_.109, i64 addrspace(4)* %_dot_.128
  %_dot_.130 = load i32* %"$const0.12"
  store i32 0, i32* %"$const0.12"
  %_dot_.133 = load i8** %"$0.15"
  store i8* null, i8** %"$0.15"
  %_dot_.135 = load i8** %"$0.15"
  %_dot_.137 = load i8** %"$0.16"
  store i8* null, i8** %"$0.16"
  %_dot_.139 = load i8** %"$0.15"
  store i8* null, i8** %"$0.15"
  %_dot_.142 = load i32* %"$const0.17"
  store i32 0, i32* %"$const0.17"
  %_dot_.144 = load i32* %"$const0.17"
  %_dot_.145 = call spir_func i8* @_Z7barrierj(i32 %_dot_.144)
  %_dot_.147 = load i8** %"$0.18"
  store i8* %_dot_.145, i8** %"$0.18"
  %_dot_.149 = load i32* %"$const0.17"
  store i32 0, i32* %"$const0.17"
  %_dot_.151 = load i8** %"$0.18"
  store i8* null, i8** %"$0.18"
  %_dot_.153 = load i8** %"$0.16"
  store i8* null, i8** %"$0.16"
  %_dot_.156 = load i32* %"$const0.20"
  store i32 0, i32* %"$const0.20"
  %_dot_.158 = load i64* %tid
  %_dot_.159 = load i32* %"$const0.20"
  %_dot_.160 = sext i32 %_dot_.159 to i64
  %_dot_.161 = icmp eq i64 %_dot_.158, %_dot_.160
  %_dot_.163 = load i1* %"$0.21"
  store i1 %_dot_.161, i1* %"$0.21"
  %_dot_.165 = load i32* %"$const0.20"
  store i32 0, i32* %"$const0.20"
  %_dot_.167 = load i1* %"$0.21"
  br i1 %_dot_.167, label %B74, label %B97

B74:                                              ; preds = %B0
  %_dot_.170 = load i32* %"$const74.1"
  store i32 123, i32* %"$const74.1"
  %_dot_.173 = load i32* %"$const74.3"
  store i32 0, i32* %"$const74.3"
  %_dot_.175 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  %_dot_.176 = load i32* %"$const74.1"
  %_dot_.177 = load i32* %"$const74.3"
  %_dot_.178 = sext i32 %_dot_.177 to i64
  %_dot_.179 = sext i32 %_dot_.176 to i64
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %_dot_.175, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.180
  %_dot_.183 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.180, i32 0, i32 5
  %_dot_.184 = getelementptr inbounds [1 x i64]* %_dot_.183, i32 0, i32 0
  %_dot_.185 = load i64* %_dot_.184, !range !7
  %_dot_.186 = insertvalue [1 x i64] undef, i64 %_dot_.185, 0
  %_dot_.187 = extractvalue [1 x i64] %_dot_.186, 0
  %_dot_.188 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.180, i32 0, i32 6
  %_dot_.189 = load [1 x i64]* %_dot_.188
  %_dot_.190 = extractvalue [1 x i64] %_dot_.189, 0
  %_dot_.191 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.180, i32 0, i32 4
  %_dot_.192 = load i64 addrspace(4)** %_dot_.191
  %_dot_.193 = icmp slt i64 %_dot_.178, 0
  %_dot_.194 = add i64 %_dot_.187, %_dot_.178
  %_dot_.195 = select i1 %_dot_.193, i64 %_dot_.194, i64 %_dot_.178
  %_dot_.196 = mul i64 %_dot_.195, 1
  %_dot_.197 = add i64 0, %_dot_.196
  %_dot_.198 = getelementptr i64 addrspace(4)* %_dot_.192, i64 %_dot_.197
  store i64 %_dot_.179, i64 addrspace(4)* %_dot_.198
  %_dot_.200 = load i32* %"$const74.3"
  store i32 0, i32* %"$const74.3"
  %_dot_.202 = load i32* %"$const74.1"
  store i32 0, i32* %"$const74.1"
  %_dot_.205 = load i32* %"$const74.4"
  store i32 321, i32* %"$const74.4"
  %_dot_.208 = load i32* %"$const74.6"
  store i32 1, i32* %"$const74.6"
  %_dot_.210 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  %_dot_.211 = load i32* %"$const74.4"
  %_dot_.212 = load i32* %"$const74.6"
  %_dot_.213 = sext i32 %_dot_.212 to i64
  %_dot_.214 = sext i32 %_dot_.211 to i64
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %_dot_.210, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.215
  %_dot_.218 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.215, i32 0, i32 5
  %_dot_.219 = getelementptr inbounds [1 x i64]* %_dot_.218, i32 0, i32 0
  %_dot_.220 = load i64* %_dot_.219, !range !7
  %_dot_.221 = insertvalue [1 x i64] undef, i64 %_dot_.220, 0
  %_dot_.222 = extractvalue [1 x i64] %_dot_.221, 0
  %_dot_.223 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.215, i32 0, i32 6
  %_dot_.224 = load [1 x i64]* %_dot_.223
  %_dot_.225 = extractvalue [1 x i64] %_dot_.224, 0
  %_dot_.226 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.215, i32 0, i32 4
  %_dot_.227 = load i64 addrspace(4)** %_dot_.226
  %_dot_.228 = icmp slt i64 %_dot_.213, 0
  %_dot_.229 = add i64 %_dot_.222, %_dot_.213
  %_dot_.230 = select i1 %_dot_.228, i64 %_dot_.229, i64 %_dot_.213
  %_dot_.231 = mul i64 %_dot_.230, 1
  %_dot_.232 = add i64 0, %_dot_.231
  %_dot_.233 = getelementptr i64 addrspace(4)* %_dot_.227, i64 %_dot_.232
  store i64 %_dot_.214, i64 addrspace(4)* %_dot_.233
  %_dot_.235 = load i32* %"$const74.6"
  store i32 0, i32* %"$const74.6"
  %_dot_.237 = load i32* %"$const74.4"
  store i32 0, i32* %"$const74.4"
  br label %B97

B97:                                              ; preds = %B74, %B0
  %_dot_.240 = load i1* %"$0.21"
  store i1 false, i1* %"$0.21"
  %_dot_.243 = load i8** %"$97.1"
  store i8* null, i8** %"$97.1"
  %_dot_.245 = load i8** %"$97.1"
  %_dot_.247 = load i8** %"$97.2"
  store i8* null, i8** %"$97.2"
  %_dot_.249 = load i8** %"$97.1"
  store i8* null, i8** %"$97.1"
  %_dot_.252 = load i32* %"$const97.3"
  store i32 0, i32* %"$const97.3"
  %_dot_.254 = load i32* %"$const97.3"
  %_dot_.255 = call spir_func i8* @_Z7barrierj(i32 %_dot_.254)
  %_dot_.257 = load i8** %"$97.4"
  store i8* %_dot_.255, i8** %"$97.4"
  %_dot_.259 = load i32* %"$const97.3"
  store i32 0, i32* %"$const97.3"
  %_dot_.261 = load i8** %"$97.4"
  store i8* null, i8** %"$97.4"
  %_dot_.263 = load i8** %"$97.2"
  store i8* null, i8** %"$97.2"
  %_dot_.266 = load i8** %"$97.5"
  store i8* null, i8** %"$97.5"
  %_dot_.268 = load i8** %"$97.5"
  %_dot_.270 = load i8** %"$97.6"
  store i8* null, i8** %"$97.6"
  %_dot_.272 = load i8** %"$97.5"
  store i8* null, i8** %"$97.5"
  %_dot_.275 = load i32* %"$const97.7"
  store i32 0, i32* %"$const97.7"
  %_dot_.277 = load i32* %"$const97.7"
  %_dot_.278 = call spir_func i8* @_Z7barrierj(i32 %_dot_.277)
  %_dot_.280 = load i8** %"$97.8"
  store i8* %_dot_.278, i8** %"$97.8"
  %_dot_.282 = load i32* %"$const97.7"
  store i32 0, i32* %"$const97.7"
  %_dot_.284 = load i8** %"$97.8"
  store i8* null, i8** %"$97.8"
  %_dot_.286 = load i8** %"$97.6"
  store i8* null, i8** %"$97.6"
  %_dot_.289 = load i32* %"$const97.10"
  store i32 1, i32* %"$const97.10"
  %_dot_.291 = load i64* %tid
  %_dot_.292 = load i32* %"$const97.10"
  %_dot_.293 = sext i32 %_dot_.292 to i64
  %_dot_.294 = icmp eq i64 %_dot_.291, %_dot_.293
  %_dot_.296 = load i1* %"$97.11"
  store i1 %_dot_.294, i1* %"$97.11"
  %_dot_.298 = load i32* %"$const97.10"
  store i32 0, i32* %"$const97.10"
  %_dot_.300 = load i1* %"$97.11"
  br i1 %_dot_.300, label %B135, label %B148

B135:                                             ; preds = %B97
  %_dot_.302 = load i1* %"$97.11"
  store i1 false, i1* %"$97.11"
  %_dot_.304 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  %_dot_.305 = load i64* %tid
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %_dot_.304, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.306
  %_dot_.309 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.306, i32 0, i32 5
  %_dot_.310 = getelementptr inbounds [1 x i64]* %_dot_.309, i32 0, i32 0
  %_dot_.311 = load i64* %_dot_.310, !range !7
  %_dot_.312 = insertvalue [1 x i64] undef, i64 %_dot_.311, 0
  %_dot_.313 = extractvalue [1 x i64] %_dot_.312, 0
  %_dot_.314 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.306, i32 0, i32 6
  %_dot_.315 = load [1 x i64]* %_dot_.314
  %_dot_.316 = extractvalue [1 x i64] %_dot_.315, 0
  %_dot_.317 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.306, i32 0, i32 4
  %_dot_.318 = load i64 addrspace(4)** %_dot_.317
  %_dot_.319 = icmp slt i64 %_dot_.305, 0
  %_dot_.320 = add i64 %_dot_.313, %_dot_.305
  %_dot_.321 = select i1 %_dot_.319, i64 %_dot_.320, i64 %_dot_.305
  %_dot_.322 = mul i64 %_dot_.321, 1
  %_dot_.323 = add i64 0, %_dot_.322
  %_dot_.324 = getelementptr i64 addrspace(4)* %_dot_.318, i64 %_dot_.323
  %_dot_.325 = load i64 addrspace(4)* %_dot_.324
  %_dot_.327 = load i64* %"$135.3"
  store i64 %_dot_.325, i64* %"$135.3"
  %_dot_.329 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  %_dot_.331 = load i64* %"$135.3"
  %_dot_.333 = load i64* %val
  store i64 %_dot_.331, i64* %val
  %_dot_.335 = load i64* %"$135.3"
  store i64 0, i64* %"$135.3"
  br label %B154

B148:                                             ; preds = %B97
  %_dot_.338 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %sm
  %_dot_.340 = load i1* %"$97.11"
  store i1 false, i1* %"$97.11"
  %_dot_.343 = load i32* %"$const148.1"
  store i32 456, i32* %"$const148.1"
  %_dot_.345 = load i32* %"$const148.1"
  %_dot_.346 = sext i32 %_dot_.345 to i64
  %_dot_.347 = load i64* %val
  store i64 %_dot_.346, i64* %val
  %_dot_.349 = load i32* %"$const148.1"
  store i32 0, i32* %"$const148.1"
  br label %B154

B154:                                             ; preds = %B148, %B135
  %_dot_.353 = load i8** %"$154.1"
  store i8* null, i8** %"$154.1"
  %_dot_.355 = load i8** %"$154.1"
  %_dot_.357 = load i8** %"$154.2"
  store i8* null, i8** %"$154.2"
  %_dot_.359 = load i8** %"$154.1"
  store i8* null, i8** %"$154.1"
  %_dot_.362 = load i32* %"$const154.3"
  store i32 0, i32* %"$const154.3"
  %_dot_.364 = load i32* %"$const154.3"
  %_dot_.365 = call spir_func i8* @_Z7barrierj(i32 %_dot_.364)
  %_dot_.367 = load i8** %"$154.4"
  store i8* %_dot_.365, i8** %"$154.4"
  %_dot_.369 = load i32* %"$const154.3"
  store i32 0, i32* %"$const154.3"
  %_dot_.371 = load i8** %"$154.4"
  store i8* null, i8** %"$154.4"
  %_dot_.373 = load i8** %"$154.2"
  store i8* null, i8** %"$154.2"
  %_dot_.375 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %out
  %_dot_.376 = load i64* %val
  %_dot_.377 = load i64* %tid
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %_dot_.375, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.378
  %_dot_.381 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.378, i32 0, i32 5
  %_dot_.382 = getelementptr inbounds [1 x i64]* %_dot_.381, i32 0, i32 0
  %_dot_.383 = load i64* %_dot_.382, !range !7
  %_dot_.384 = insertvalue [1 x i64] undef, i64 %_dot_.383, 0
  %_dot_.385 = extractvalue [1 x i64] %_dot_.384, 0
  %_dot_.386 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.378, i32 0, i32 6
  %_dot_.387 = load [1 x i64]* %_dot_.386
  %_dot_.388 = extractvalue [1 x i64] %_dot_.387, 0
  %_dot_.389 = getelementptr inbounds { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %_dot_.378, i32 0, i32 4
  %_dot_.390 = load i64 addrspace(4)** %_dot_.389
  %_dot_.391 = icmp slt i64 %_dot_.377, 0
  %_dot_.392 = add i64 %_dot_.385, %_dot_.377
  %_dot_.393 = select i1 %_dot_.391, i64 %_dot_.392, i64 %_dot_.377
  %_dot_.394 = mul i64 %_dot_.393, 1
  %_dot_.395 = add i64 0, %_dot_.394
  %_dot_.396 = getelementptr i64 addrspace(4)* %_dot_.390, i64 %_dot_.395
  store i64 %_dot_.376, i64 addrspace(4)* %_dot_.396
  %_dot_.398 = load i64* %val
  store i64 0, i64* %val
  %_dot_.400 = load i64* %tid
  store i64 0, i64* %tid
  %_dot_.402 = load { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %out
  store { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } zeroinitializer, { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] }* %out
  %_dot_.405 = load i8** %"$const154.8"
  store i8* null, i8** %"$const154.8"
  %_dot_.407 = load i8** %"$const154.8"
  %_dot_.409 = load i8** %"$154.9"
  store i8* %_dot_.407, i8** %"$154.9"
  %_dot_.411 = load i8** %"$const154.8"
  store i8* null, i8** %"$const154.8"
  %_dot_.413 = load i8** %"$154.9"
  store i8* %_dot_.413, i8** %_dot_.ret
  ret i32 0
}

declare spir_func i64 @_Z12get_local_idj(i32)

declare spir_func i8* @_Z7barrierj(i32)

define spir_kernel void @hsaPy_hsapy_devfn__5F__5F_main_5F__5F__2E_foo_24_1_2E_array_28_int64_2C__20_1d_2C__20_C_29_(i8 addrspace(1)* %_dot_.1, i8 addrspace(1)* %_dot_.2, i64 %_dot_.3, i64 %_dot_.4, i64 addrspace(1)* %_dot_.5, i64 %_dot_.6, i64 %_dot_.7) {
.9:
  %_dot_.10 = addrspacecast i8 addrspace(1)* %_dot_.1 to i8*
  %_dot_.11 = addrspacecast i8 addrspace(1)* %_dot_.2 to i8*
  %_dot_.12 = addrspacecast i64 addrspace(1)* %_dot_.5 to i64 addrspace(4)*
  %inserted.meminfo = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } undef, i8* %_dot_.10, 0
  %inserted.parent = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.meminfo, i8* %_dot_.11, 1
  %inserted.nitems = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.parent, i64 %_dot_.3, 2
  %inserted.itemsize = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.nitems, i64 %_dot_.4, 3
  %inserted.data = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.itemsize, i64 addrspace(4)* %_dot_.12, 4
  %_dot_.13 = insertvalue [1 x i64] undef, i64 %_dot_.6, 0
  %inserted.shape = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.data, [1 x i64] %_dot_.13, 5
  %_dot_.14 = insertvalue [1 x i64] undef, i64 %_dot_.7, 0
  %inserted.strides = insertvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.shape, [1 x i64] %_dot_.14, 6
  %_dot_.15 = alloca i8*
  store i8* null, i8** %_dot_.15
  %extracted.meminfo = extractvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.strides, 0
  %extracted.parent = extractvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.strides, 1
  %extracted.nitems = extractvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.strides, 2
  %extracted.itemsize = extractvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.strides, 3
  %extracted.data = extractvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.strides, 4
  %extracted.shape = extractvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.strides, 5
  %_dot_.17 = extractvalue [1 x i64] %extracted.shape, 0
  %extracted.strides = extractvalue { i8*, i8*, i64, i64, i64 addrspace(4)*, [1 x i64], [1 x i64] } %inserted.strides, 6
  %_dot_.18 = extractvalue [1 x i64] %extracted.strides, 0
  %_dot_.19 = call spir_func i32 @hsapy_devfn__5F__5F_main_5F__5F__2E_foo_24_1_2E_array_28_int64_2C__20_1d_2C__20_C_29_(i8** %_dot_.15, i8* %extracted.meminfo, i8* %extracted.parent, i64 %extracted.nitems, i64 %extracted.itemsize, i64 addrspace(4)* %extracted.data, i64 %_dot_.17, i64 %_dot_.18)
  %_dot_.20 = icmp eq i32 %_dot_.19, 0
  %_dot_.21 = icmp eq i32 %_dot_.19, -2
  %_dot_.22 = or i1 %_dot_.20, %_dot_.21
  %_dot_.23 = xor i1 %_dot_.22, true
  %_dot_.24 = icmp eq i32 %_dot_.19, -1
  %_dot_.25 = icmp eq i32 %_dot_.19, -3
  %_dot_.26 = icmp sge i32 %_dot_.19, 1
  %_dot_.27 = load i8** %_dot_.15
  ret void
}

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
!7 = metadata !{i64 0, i64 9223372036854775807}
