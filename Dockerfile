FROM redis:6.2 as builder

ARG REDICRYPT_KEY=default
ENV REDICRYPT_KEY ${REDICRYPT_KEY}

ADD dist/redicrypt.so /usr/local/lib/redicrypt.so
ENTRYPOINT REDICRYPT_KEY=${REDICRYPT_KEY} /usr/local/bin/redis-server  --load-module  /usr/local/lib/redicrypt.so
