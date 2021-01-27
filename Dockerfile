FROM ubuntu:20.04

WORKDIR /home

ENV TZ=Europe/Paris
# RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt update && \
apt install -y apt-transport-https ca-certificates \
gnupg software-properties-common wget

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null 

RUN apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main' 

RUN apt update

RUN apt install -y cmake && \
apt install -y build-essential git curl protobuf-compiler pkg-config \
libprotobuf-dev protobuf-compiler libode-dev libboost-all-dev

RUN git clone https://github.com/NAELIC/proton.git && cd proton && \
cmake -B build && cd build && make

WORKDIR /home/proton

CMD ["./build/proton"]