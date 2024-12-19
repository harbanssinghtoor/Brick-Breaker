# SJ2-C Getting Started

This guide provides step-by-step instructions to set up your development environment to give you to ability to develop firmware, build firmware, flash firmware onto an attached board, and monitor an attached board over serial.

## Target Outcome

Throughout this guide, we will:

1. Install software to set up your development environment
2. Build the Docker image
3. Build the firmware
4. Flash the firmware

By the end of this guide, you will have the following software installed on your computer:

* Docker
* Python
* Driver for USB-serial
* Visual Studio Code (a.k.a. vscode)
* Clang Format

Additionally by the end of this guide, you will be able to do the following:

* Modify source code
* Build Docker images
* Build firmware using a Docker container
* Flash firmware over a USB to serial connection
* Monitor firmware activity over a USB to serial connection

# Step 1. Installation

## Install Docker

1. Follow this [Docker Engine Installation Guide](https://docs.docker.com/engine/install/).
2. After installation, start Docker and leave it running in the background.
3. Verify Docker is installed correctly:

```bash
# This command should work
docker

# This should not produce a warning that the Docker daemon is not running.
docker info
```

## Install Python

1. For Windows, follow this [Python Installation Guide](installs/README.md#python). Mac and Linux should already have it preinstalled.

2. Verify Python is installed correctly:

```bash
# Ensure this command works on Windows
python --version

# Alternatively, this is fine too for Linux and Mac
python3 --version
```

## Install USB-Serial Driver

1. Install the driver:
    * For Mac, install `installs/drivers/mac/SiLabsUSBDriverDisk.dmg`.
    * For Windows, extract `installs/drivers/windows/CP210x_Windows_Drivers.zip` then run `CP210xVCPInstaller_x64.exe`.
    * For Linux, the driver comes bundled in the kernel, so you don't need to install it separately.

## Install Visual Studio Code

This step is highly recommended; install Visual Studio Code (i.e. VSCode) IDE for code editing.

1. Download and install [Visual Studio Code](https://code.visualstudio.com/download).

### (Optional) Install VSCode Extensions

2. Install Visual Code extensions:
    * [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
    * [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)

#### Install Clang Format

1. For Windows, follow this [Clang Format Dependency Installation Guide](installs/README.md#clang-format-windows-only).
2. Install Clang Format:

```bash
# Alternatively, use 'python3'
python -m pip install clang-format
```

3. Verify Clang Format is installed correctly:

```bash
# This should produce a version message
clang-format --version
```

# Step 2. Setup

## Build Docker Image

1. Build the development Docker image:

```bash
python ./run setup
```

# Step 3. Build and Flash

At this point, the set up is complete. The steps below describe your typical daily workflow.

1. Build the development Docker image:

```bash
python ./run build
```

2. Ensure your target device is attached via USB-serial connection.

3. Flash the compiled firmware `.bin` file:

```bash
python ./run flash
```

# Step 4. Familiarize Yourself With the `run` Script

At this point you should be ready for development. This step is optional. In this step, learn more about the `run` script, a multi-purpose tool for development with the ability to build, flash, etc.

Here are some other ways to use the `run` script:

```bash
# Linux or Mac
./run

# Windows
python run
```

The `run` script takes a subcommand to perform specific operations. See the `run` help message for more information:

```bash
# `run` usage format
./run {subcommand} {arguments} -- {passthrough arguments}

# To list available subcommands, see its help message
./run -h
```

Here are some other use cases:

```bash
# Build using only a single CPU core
./run build -- -j1

# Build with verbose information
./run build -- --verbose

# Clean (delete) your last build
./run build -- -c

# Skip unit tests
./run build -- --no-unit-test

# See other available build options
./run build -- -h

# Flash using a specific port
./run flash -- --port COM1

# See other available flash options
./run flash -- -h
```

## Other Workflows

Here are some other use cases beyond building and flashing firmware.

**Developing Executables**

```bash
# Build a native executable
./run build -- --project=x86_freertos

# Run an executable in the Docker container
./run exe -- _build_x86_freertos/x86_freertos.exe
```

**Docker Maintenance and Experimentation**

```bash
# Open a shell session in the Docker container
./run shell
```
