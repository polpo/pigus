FROM debian:stable

ADD gcc-arm-10.3-2021.07-aarch64-arm-none-eabi.tar.xz /toolchain/

RUN apt-get update && apt-get install -y \
	build-essential \
	gdb-multiarch

ENV PATH="/toolchain/gcc-arm-10.3-2021.07-aarch64-arm-none-eabi/bin:${PATH}"

CMD bash
