FROM redis:7.2.1 as builder

ARG GO_VER=1.21.2

ADD ./ /build
RUN apt update -qq && apt-get install -qqy build-essential wget libffi-dev
RUN wget -q https://golang.org/dl/go${GO_VER}.linux-amd64.tar.gz -O /tmp/go.tgz
WORKDIR /usr/local
RUN tar xpf /tmp/go.tgz
RUN ln -s /usr/local/go/bin/go /usr/local/bin/go

WORKDIR /build
RUN make all

# -------------------------------------------------------- #

FROM redis:7.2.1 as runner
ARG REDICRYPT_KEY=default
ENV REDICRYPT_KEY ${REDICRYPT_KEY}
COPY --from=builder /build/dist/redicrypt.so /usr/local/lib/redicrypt.so
ENTRYPOINT REDICRYPT_KEY=${REDICRYPT_KEY} /usr/local/bin/redis-server  --loadmodule  /usr/local/lib/redicrypt.so
