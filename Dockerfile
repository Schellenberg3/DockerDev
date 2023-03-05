# This specific Dockerfile is used in the DevContainers examples.
FROM mcr.microsoft.com/devcontainers/base:ubuntu

RUN apt update && \
    apt install -y --no-install-recommends \
        build-essential \
        libeigen3-dev \
        python3 \
        clang \
        git
