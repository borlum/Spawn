FROM centos:7

ENV LANG en_US.UTF-8
ENV LC_ALL en_US.UTF-8
ENV LANGUAGE en_US:en

RUN yum install wget -y
RUN yum install bzip2 -y
RUN yum install gcc -y
RUN yum install gcc-c++ -y
RUN yum install make -y
RUN yum install python3 -y

RUN wget https://github.com/Kitware/CMake/releases/download/v3.25.0-rc4/cmake-3.25.0-rc4-linux-x86_64.tar.gz
RUN tar xzf cmake-3.25.0-rc4-linux-x86_64.tar.gz
RUN export PATH=/cmake-3.25.0-rc4-linux-x86_64/bin:$PATH

RUN wget https://ftp.gnu.org/gnu/gcc/gcc-9.1.0/gcc-9.1.0.tar.gz --no-check-certificate
RUN tar xzf gcc-9.1.0.tar.gz
RUN cd gcc-9.1.0 && ./contrib/download_prerequisites
RUN mkdir gcc-9.1.0-build
RUN cd gcc-9.1.0-build && ../gcc-9.1.0/configure --enable-languages=c,c++ --disable-multilib
RUN cd gcc-9.1.0-build && make
RUN cd gcc-9.1.0-build && make install

RUN python3 -m pip install conan

RUN wget https://github.com/graalvm/graalvm-ce-builds/releases/download/vm-22.3.0/graalvm-ce-java11-linux-amd64-22.3.0.tar.gz
RUN tar xzf graalvm-ce-java11-linux-amd64-22.3.0.tar.gz
RUN export PATH=/graalvm-ce-java11-22.3.0/bin:$PATH