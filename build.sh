#!/bin/bash

function compile() 
{
rm -rf AnyKernel
source ~/.bashrc && source ~/.profile
export LC_ALL=C && export USE_CCACHE=1
export ARCH=arm64
export KBUILD_BUILD_HOST=neolit
export KBUILD_BUILD_USER="sarthakroy2002"
mkdir clang && cd clang
bash <(curl -s https://raw.githubusercontent.com/Neutron-Toolchains/antman/main/antman) -S=latest
bash <(curl -s https://raw.githubusercontent.com/Neutron-Toolchains/antman/main/antman) --patch=glibc
cd ..

[ -d "out" ] && rm -rf out || mkdir -p out

make O=out ARCH=arm64 RMX2020_defconfig

PATH="${PWD}/clang/bin:${PATH}" \
make -j$(nproc --all) O=out \
                      ARCH=arm64 \
                      CC="clang" \
                      CROSS_COMPILE=aarch64-linux-gnu- \
                      CROSS_COMPILE_ARM32=arm-linux-gnueabi- \
                      LLVM=1 \
                      LD=ld.lld \
                      AS=llvm-as \
		              AR=llvm-ar \
			          NM=llvm-nm \
			          OBJCOPY=llvm-objcopy \
                      OBJDUMP=llvm-objdump \
                      STRIP=llvm-strip \
                      CONFIG_NO_ERROR_ON_MISMATCH=y
}

function zipping()
{
git clone --depth=1 https://github.com/sarthakroy2002/AnyKernel3.git AnyKernel
cp out/arch/arm64/boot/Image.gz-dtb AnyKernel
cd AnyKernel
zip -r9 Test-OSS-KERNEL-RMX2020-NEOLIT.zip *
}

compile
zipping
