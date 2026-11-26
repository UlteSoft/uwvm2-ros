# 私有 WasmMTE reactor 的生产签名更新

已安装的 `wroc` 和 `wrocxx` 可为 Objective-C 或 Objective-C++ 的静态数据指针生成 WasmMTE 重定位。以下命令使用私有内存编译和链接一代应用。

```sh
wroc --target wasm64_adm16 --core 3.0 --std c17 -O0 -g --mte --eh --no-threads -c sensor.m -o sensor.o
wroc --target wasm64_adm16 --core 3.0 --std c17 -O0 -g --mte --eh --no-threads --reactor --export _start --export value sensor.o -o sensor.wasm
```

Objective-C++ 的 `.mm` 文件使用 `wrocxx` 和 `--std c++20`。以相同的目标、内存策略和导出编译下一代业务代码。两代之间的函数更新只允许签名范围内的函数体发生变化。

用 Linux LLVM full 运行时启动第一代，启用匹配的 memory64、WasmADM、WasmMTE 解析与访问检查，并为 `--update-call-export` 指定业务导出。直接父进程通过已认证的 Unix 流套接字取得进程 nonce 和已安装映像摘要，签署 LAUNCH。PREPARE 的签名覆盖整份目标映像以及需要替换的**本地函数索引**。在同一进程完成 READY 和 COMMIT，再用包含导入的导出索引调用业务函数。[启动参数、签名字段和线协议](production-update.md)

冷初始化只接受一块私有带标签内存中、由本地无参无返回值 Start 描述的同宽 32 位或 64 位静态指针重定位。VM 完整解析函数体，验证每个不重叠的目标槽、活动数据中的原始指针以及静态分配范围。可选的有符号偏移允许指向对象内部或尾后位置。所有记录均通过后，VM 才按本次分配的实际标签写入指针。函数名不参与判定，通用 Start 和预加载模块仍由签名管理器拒绝。

签名 LAUNCH 后，独立导出的命令入口只运行一次。函数更新保留业务已更改的指针、指向的数据和内存标签。修改冷初始化器或数据布局时需重新授权部署。可用 `python3 test/0017.runtime/run_signed_memtag_relocations.py --vm PATH --out DIR` 检查两种地址宽度的签名切换、活状态和拒绝路径。
