FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

ARG SCONS_ARGS="-j $(cat /proc/cpuinfo | grep processor | wc -l)"

# Install dependencies
RUN apt update && apt install -y \
    scons=4.0.1+dfsg-2 \
    python3-pip=22.0.2+dfsg-1ubuntu0.4 \
    python-is-python3=3.9.2-2 \
    ruby=1:3.0~exp1 \
    gcc-arm-none-eabi=15:10.3-2021.07-4

# Install Python dependencies
RUN pip3 install \
    setuptools==62.1.0 \
    cantools==39.4.0 \
    python-can==4.1.0

# Set the initial current directory to this directory
WORKDIR /root/sjtwo-c
VOLUME ["/root/sjtwo-c"]

# Set default command
CMD ["/bin/bash", "-c", "scons ${SCONS_ARGS}"]
