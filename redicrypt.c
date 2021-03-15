#include <stdlib.h>
#include <string.h>
#include "dist/redicrypt_go.h"
#include "dist/redismodule.h"

int HEncSetCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);

    size_t s;
    const char *str = RedisModule_StringPtrLen(argv[2], &s);

    const char *res = HEncSet(getenv("REDICRYPT_KEY"), str);

    RedisModuleString *ciphered = RedisModule_CreateString(ctx, res, strlen(res));
    RedisModuleCallReply *srep = RedisModule_Call(ctx, "SET", "ss", argv[1], ciphered);
    if (RedisModule_CallReplyType(srep) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, srep);
    }
    RedisModule_ReplyWithSimpleString(ctx, "OK");
    return REDISMODULE_OK;
}

int HDecGetCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);

    RedisModuleCallReply *reply= RedisModule_Call(ctx, "GET", "s", argv[1]);
    if (RedisModule_CallReplyType(reply) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, reply);
    }
    size_t len;
    char *ptr = RedisModule_CallReplyStringPtr(reply,&len);

    const char *res = HDecGet(getenv("REDICRYPT_KEY"), ptr);
    RedisModuleString *decrypted = RedisModule_CreateString(ctx, res, strlen(res));

    RedisModule_ReplyWithString(ctx, decrypted);

    return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (RedisModule_Init(ctx,"redicrypt",1,REDISMODULE_APIVER_1)
        == REDISMODULE_ERR) return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"HENCSET",
        HEncSetCommand, "write",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"HDECGET",
        HDecGetCommand, "readonly",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    return REDISMODULE_OK;
}
