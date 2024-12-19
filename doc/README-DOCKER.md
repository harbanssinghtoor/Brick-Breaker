# SJ2-C Docker

This README is for maintainers only.

This guide provides instructions for using and maintaining the SJ2-C Docker image. For additional information about Docker, please refer to the [Docker Overview](https://docs.docker.com/get-started/overview/). Docker's operation is facilitated by [Docker Engine](https://docs.docker.com/engine/), a background daemon that manages Docker `containers` and `images` in your native environment.

## Background

Who is Docker? What is Docker? Why is Docker?

One challenge with maintaining this SJ2-C development project is to ensure it's compatible with computers running varying operating systems such as Windows, macOS, and the many Linux distributions out there. A simple approach to this problem is to prepackage executables (e.g. the ARM embedded toolchain) for the common operating systems to minimize dependency with software installed on your environment. A better approach is to rely on Docker, a platform that facilitates virtualized environments. This gives you the ability to build firmware on a consistent environment regardless of your OS, better eliminating the "but it works on my machine" problem.

In more technical details about Docker, there are two important terminologies to understand to better follow this guide: `containers` and `images`. Docker is a platform that allows applications to run in `containers`, which are virtualized environment instances similar to virtual machines but significantly more efficient because they do not virtualize the OS kernel. Containers ensure that applications operate consistently across different operating systems. Furthermore, Docker `images` are analogous to container snapshots; they act as templates for creating new containers. Images are derived from configuration files that describe the desired environment which set up the user space dependencies such a libraries, filesystem layout, environment variables, daemons, and executables. As an example, you can create this simple Docker image: Bare bone Ubuntu 22.04 environment, a few apt packages installed, and your custom web server application that runs on startup. You can use this image, for example, on your Windows computer by creating a container off of this image and have that container run in the background indefinitely; your web server will be live in this case.

# Docker Image

This section describes how this project's Docker image is set up. This Docker image is based off of Ubuntu 22.04 and includes the necessary toolchains and dependencies, such as the ARM embedded toolchain and SCons, to build firmware images.

The SJ2-C Docker image is defined in `Dockerfile`. To better understand this file or to learn how to modify this file, see [Dockerfile Guide](https://docs.docker.com/reference/dockerfile/). As part of the building firmware workflow, SCons (e.g. `scons`) and toolchain executables (e.g. `arm-none-eabi-gcc`) are executed in the Ubuntu environment.

Docker mounts this project directory `sjtwo-c` (your filesystem) to  `/root/sjtwo-c` (container's filesystem). Meaning that as you build firmware in the container, SCons and the toolchains interacts with the files and directories in your native filesystem. As an example after you build firmware, you should find compiled artifacts placed in `./sjtwo-c/_build_lpc40xx_freertos/`

# Docker CLI Workflows

These workflows are intended for project maintainers only, but feel free to try these out for educational purposes. These workflows will use the [Docker CLI](https://docs.docker.com/reference/cli/docker/).

Visit these references for more information:

- [Docker Image](https://docs.docker.com/reference/cli/docker/image/)
- [Docker Container](https://docs.docker.com/reference/cli/docker/container/)
- [Docker Run](https://docs.docker.com/reference/cli/docker/container/run/)

```bash
# List all images
docker image ls

# Build a named image
docker build -t sjtwo-c .

# Delete the image
docker image rm sjtwo-c
```

```bash
# List all active containers
docker container ls

# Create a new named container based off of the `sjtwo-c` image
# - Run bash, creating a new shell session in that environment
# - Mount current directory to its filesystem `/root/sjtwo-c`
# - Auto delete container on exit
docker run --rm -it --mount src=.,target=/root/sjtwo-c,type=bind --name=sjtwo-c sjtwo-c /bin/bash

# Delete the container
docker container rm sjtwo-c
```

## Managing Dependencies

Using the `/bin/bash` shell session attached to the container, you can manage `apt` and `python` dependencies. You can find all publicly available `apt` packages [here](https://packages.ubuntu.com/) and Python `pip` packages [here](https://pypi.org/).

```bash
# List all installed apt packages
apt list

# Print information about a specific apt package
apt show gcc-arm-none-eabi

# Install an apt package (latest)
apt install gcc-arm-none-eabi

# List all installed Python pip packages
pip list

# Install a Python pip package (latest)
pip install scons
```
