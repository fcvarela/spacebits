#!/bin/sh

export NDK="/Users/fcvarela/Downloads/android-ndk-r5c"
export SYSROOT="$NDK/platforms/android-8/arch-arm"
export CC="$NDK/toolchains/arm-linux-androideabi-4.4.3/prebuilt/darwin-x86/bin/arm-linux-androideabi-gcc --sysroot=$SYSROOT"

