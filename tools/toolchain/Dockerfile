FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo \
    wget nasm xorriso grub-pc-bin grub-common mtools \
    && rm -rf /var/lib/apt/lists/*

ENV PREFIX="/usr/local/cross"
ENV TARGET="x86_64-elf"
ENV PATH="$PREFIX/bin:$PATH"
WORKDIR /tmp/src

RUN wget https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.gz && \
    tar xf binutils-2.40.tar.gz && \
    mkdir build-binutils && cd build-binutils && \
    ../binutils-2.40/configure --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror && \
    make -j$(nproc) && make install && \
    cd /tmp/src && rm -rf build-binutils binutils-2.40*

RUN wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz && \
    tar xf gcc-13.2.0.tar.gz && \
    mkdir build-gcc && cd build-gcc && \
    ../gcc-13.2.0/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers && \
    make all-gcc -j$(nproc) && make all-target-libgcc -j$(nproc) && \
    make install-gcc && make install-target-libgcc && \
    cd / && rm -rf /tmp/src

WORKDIR /strix
