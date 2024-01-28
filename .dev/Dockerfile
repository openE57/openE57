FROM ubuntu:22.04

ARG CONAN_VERSION=2.0.17
ARG USERID=1000

RUN apt-get update && \
    apt-get install -y \
      build-essential \
      cmake \
      clang-format \
      curl \
      cppcheck && \
    curl -L https://github.com/conan-io/conan/releases/download/${CONAN_VERSION}/conan-ubuntu-64.deb -o /tmp/conan.deb && \
    dpkg -i /tmp/conan.deb && \
    apt-get install --fix-broken -y && \
    apt-get autoremove --purge -y && \
    apt-get autoclean -y && \
    rm -rf /var/apt/cache/* && \
    useradd -rm -d /home/user -s /bin/bash -g root -G sudo -u ${USERID} user

USER user

WORKDIR /home/user

RUN conan profile detect
