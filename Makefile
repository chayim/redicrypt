DISTDIR:= dist
PROGROOT=redicrypt

.PHONY: all

all: clean ${DISTDIR}/${PROGROOT}.so

${DISTDIR}/${PROGROOT}_go.a: ${PROGROOT}.go
	go build -buildmode=c-archive -o $@ $?

${DISTDIR}/redismodule.h: ~/depos/redis/redis/src/redismodule.h
	@mkdir -p ${DISTDIR}
	@cp $? $@

${DISTDIR}/${PROGROOT}_go.h: ${PROGROOT}.go
	go build -buildmode=c-shared -o ${DISTDIR}/${PROGROOT}_go.so $?

${DISTDIR}/${PROGROOT}.o: ${PROGROOT}.c ${DISTDIR}/${PROGROOT}_go.h ${DISTDIR}/redismodule.h
	gcc -fPIC -std=gnu99 -c -static -o $@ $<

${DISTDIR}/${PROGROOT}.so: ${DISTDIR}/${PROGROOT}_go.a ${DISTDIR}/${PROGROOT}.o
	gcc -shared -Bsymbolic -lc -lffi -nostdlib -o $@ -Wl,--whole-archive $?

clean:
	rm -rf ${DISTDIR}
