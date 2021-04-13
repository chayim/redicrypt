DISTDIR:= dist
PROGROOT=redicrypt

.PHONY: all

all: clean ${DISTDIR}/${PROGROOT}.so

deps/redismodule.h:
	@mkdir deps
	@wget -q -O $@ https://raw.githubusercontent.com/redis/redis/unstable/src/redismodule.h

${DISTDIR}/${PROGROOT}_go.a: c_bridge.go
	go mod download
	go build -buildmode=c-archive -o $@ $?

${DISTDIR}/${PROGROOT}_go.h: c_bridge.go
	go build -buildmode=c-shared -o ${DISTDIR}/${PROGROOT}_go.so $? src/redicrypt

${DISTDIR}/${PROGROOT}.o: ${PROGROOT}.c ${DISTDIR}/${PROGROOT}_go.h deps/redismodule.h
	gcc -w -fPIC -std=gnu99 -c -static -o $@ $<

${DISTDIR}/${PROGROOT}.so: ${DISTDIR}/${PROGROOT}_go.a ${DISTDIR}/${PROGROOT}.o
	gcc -w -shared -Bsymbolic -lc -lffi -nostdlib -o $@ -Wl,--whole-archive $?

clean:
	rm -rf ${DISTDIR}

docker::
	docker build -t ${PROGROOT}:latest .

test::
	cd src/redicrypt && go test

run:: ${DISTDIR}/${PROGROOT}.so
	REDICRYPT_KEY=12345678901234567890123456789012 redis-server --loadmodule ${DISTDIR}/${PROGROOT}.so

test_docker:: docker
	docker run --rm --name ${PROGROOT} ${PROGROOT}:latest
