#include <stdlib.h>
#include <string.h>
#include "dist/redicrypt_go.h"
#include "deps/redismodule.h"

const RedisModuleString* _encrypt(RedisModuleCtx *ctx, char* ptr) {
    const char *res = Encrypt(getenv("REDICRYPT_KEY"), ptr);
    RedisModuleString *encrypted = RedisModule_CreateString(ctx, res, strlen(res));
    return encrypted;
}

/* Set an encrypted value */
int SetEncCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);

    size_t s;
    const char *str = RedisModule_StringPtrLen(argv[2], &s);

    //const char *ptr = Encrypt(getenv("REDICRYPT_KEY"), str);
    RedisModuleString *ciphered = _encrypt(ctx, str);
    RedisModuleCallReply *srep = RedisModule_Call(ctx, "SET", "ss", argv[1], ciphered);

    if (RedisModule_CallReplyType(srep) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, srep);
    }
    RedisModule_ReplyWithSimpleString(ctx, "OK");
    return REDISMODULE_OK;
}

const RedisModuleString* _decrypt(RedisModuleCtx *ctx, const char* key, char* ptr) {
    const char *res = Decrypt(key, ptr);
    RedisModuleString *decrypted = RedisModule_CreateString(ctx, res, strlen(res));
    return decrypted;
}

/* Decrypt an existing redis key, returning the existing string. */
int GetDecCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);

    RedisModuleCallReply *reply = RedisModule_Call(ctx, "GET", "s", argv[1]);
    if (RedisModule_CallReplyType(reply) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, reply);
    }
    size_t len;
    char *ptr = RedisModule_CallReplyStringPtr(reply,&len);

    RedisModuleString *decrypted = _decrypt(ctx, getenv("REDICRYPT_KEY"), ptr);
    RedisModule_ReplyWithString(ctx, decrypted);

    return REDISMODULE_OK;
}

/* Decrypt existing encrypted key, and recrypt */
int RecryptCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc >= 2) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);

    RedisModuleCallReply *reply = RedisModule_Call(ctx, "GET", "s", argv[1]);
    if (RedisModule_CallReplyType(reply) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, reply);
    }
    size_t len;
    char *ptr = RedisModule_CallReplyStringPtr(reply,&len);

    // decrypted
    RedisModuleString *decrypted = _decrypt(ctx, getenv("OLD_REDICRYPT_KEY"), ptr);


    size_t *enc_len;
    const char *dechar = RedisModule_StringPtrLen(decrypted, enc_len);

    // recrypt
    RedisModuleString *ciphered = _encrypt(ctx, dechar);
    RedisModuleCallReply *srep = RedisModule_Call(ctx, "SET", "ss", argv[1], ciphered);

    if (RedisModule_CallReplyType(srep) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, srep);
    }
    RedisModule_ReplyWithSimpleString(ctx, "OK");
    return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (RedisModule_Init(ctx,"redicrypt",1,REDISMODULE_APIVER_1)
        == REDISMODULE_ERR) return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"SETENC",
        SetEncCommand, "write",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"DECGET",
        GetDecCommand, "readonly",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"RECRYPT",
        RecryptCommand, "write",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    return REDISMODULE_OK;
}
