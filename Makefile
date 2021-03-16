DISTDIR:= dist
PROGROOT=redicrypt

.PHONY: all

all: clean ${DISTDIR}/${PROGROOT}.so

deps/redismodule.h:
	@mkdir deps
	@wget -q -O $@ https://raw.githubusercontent.com/redis/redis/unstable/src/redismodule.h

${DISTDIR}/${PROGROOT}_go.a: ${PROGROOT}.go
	go build -buildmode=c-archive -o $@ $?

${DISTDIR}/${PROGROOT}_go.h: ${PROGROOT}.go
	go build -buildmode=c-shared -o ${DISTDIR}/${PROGROOT}_go.so $?

${DISTDIR}/${PROGROOT}.o: ${PROGROOT}.c ${DISTDIR}/${PROGROOT}_go.h deps/redismodule.h
	gcc -fPIC -std=gnu99 -c -static -o $@ $<

${DISTDIR}/${PROGROOT}.so: ${DISTDIR}/${PROGROOT}_go.a ${DISTDIR}/${PROGROOT}.o
	gcc -shared -Bsymbolic -lc -lffi -nostdlib -o $@ -Wl,--whole-archive $?

clean:
	rm -rf ${DISTDIR}
