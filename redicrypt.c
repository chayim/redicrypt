#include <stdlib.h>
#include <string.h>
#include "dist/redicrypt_go.h"
#include "deps/redismodule.h"

const RedisModuleString* _encrypt(RedisModuleCtx *ctx, char* ptr) {
    const char *res = Encrypt(getenv("REDICRYPT_KEY"), ptr);
    RedisModuleString *encrypted = RedisModule_CreateString(ctx, res, strlen(res));
    return encrypted;
}

const RedisModuleString* _decrypt(RedisModuleCtx *ctx, const char* key, char* ptr) {
    const char *res = Decrypt(key, ptr);
    RedisModuleString *decrypted = RedisModule_CreateString(ctx, res, strlen(res));
    return decrypted;
}

/* Set a redis key to a base64-encoded value */
int SetB64Command (RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);
    size_t s;
    const char *str = RedisModule_StringPtrLen(argv[2], &s);

    const char *res = Base64Encode(str);
    RedisModuleString *encoded = RedisModule_CreateString(ctx, res, strlen(res));
    RedisModuleCallReply *srep = RedisModule_Call(ctx, "SET", "ss", argv[1], encoded);

    if (RedisModule_CallReplyType(srep) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, srep);
    }

    RedisModule_ReplyWithSimpleString(ctx, "OK");

    return REDISMODULE_OK;
}

/* Unbase-64encode a redis key value */
int GetB64Command (RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 2) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);

    RedisModuleCallReply *reply = RedisModule_Call(ctx, "GET", "s", argv[1]);
    if (RedisModule_CallReplyType(reply) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, reply);
    }
    size_t len;
    char *ptr = RedisModule_CallReplyStringPtr(reply, &len);

    const char *res = Base64Decode(ptr);
    RedisModuleString *unhashed = RedisModule_CreateString(ctx, res, strlen(res));
    RedisModule_ReplyWithString(ctx, unhashed);

    return REDISMODULE_OK;
}

/* Set the encrypted value for a key, b64encoding the key */
int BSetEncCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);

    size_t s;
    const char *str = RedisModule_StringPtrLen(argv[2], &s);

    RedisModuleString *ciphered = _encrypt(ctx, str);
    const char *res = Base64Encode(argv[1]);
    RedisModuleString *key= RedisModule_CreateString(ctx, res, strlen(res));
    RedisModuleCallReply *srep = RedisModule_Call(ctx, "SET", "ss", key, ciphered);

    if (RedisModule_CallReplyType(srep) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, srep);
    }
    RedisModule_ReplyWithSimpleString(ctx, "OK");
    return REDISMODULE_OK;

}

/* Set an encrypted value */
int SetEncCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);

    size_t s;
    const char *str = RedisModule_StringPtrLen(argv[2], &s);

    RedisModuleString *ciphered = _encrypt(ctx, str);
    RedisModuleCallReply *srep = RedisModule_Call(ctx, "SET", "ss", argv[1], ciphered);

    if (RedisModule_CallReplyType(srep) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, srep);
    }
    RedisModule_ReplyWithSimpleString(ctx, "OK");
    return REDISMODULE_OK;
}

/* Set a value, hashed
 * [1] = Hash Type
 * [2] = Redis key to set
 * [3] = Redis value to hash */
int SetHashCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 4) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);

    size_t s;
    const char *hashType = RedisModule_StringPtrLen(argv[1], &s);
    const char *str = RedisModule_StringPtrLen(argv[3], &s);

    const char *res = Hash(hashType, str);
    RedisModuleString *hashed = RedisModule_CreateString(ctx, res, strlen(res));
    RedisModuleCallReply *srep = RedisModule_Call(ctx, "SET", "ss", argv[2], hashed);

    if (RedisModule_CallReplyType(srep) == REDISMODULE_REPLY_ERROR) {
      return RedisModule_ReplyWithCallReply(ctx, srep);
    }
    RedisModule_ReplyWithSimpleString(ctx, "OK");
    return REDISMODULE_OK;
}

/* Decrypt an existing redis key, returning the existing string. */
int GetEncCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

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

/* Decrypt an existing redis key, returning the existing string. */
int BGetEncCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2) return RedisModule_WrongArity(ctx);
    RedisModule_AutoMemory(ctx);

    const char *res = Base64Decode(argv[1]);
    RedisModuleString *key = RedisModule_CreateString(ctx, res, strlen(res));

    RedisModuleCallReply *reply = RedisModule_Call(ctx, "GET", "s", key);
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

    if (RedisModule_CreateCommand(ctx,"RC.SETENC",
        SetEncCommand, "write",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"RC.BSETENC",
        BSetEncCommand, "write",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"RC.SETB64",
        SetB64Command, "write",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"RC.SETHASH",
        SetHashCommand, "write",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"RC.GETB64",
        GetB64Command, "readonly",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"RC.BGETENC",
        BGetEncCommand, "readonly",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"RC.GETENC",
        GetEncCommand, "readonly",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx,"RC.RECRYPT",
        RecryptCommand, "write",
        0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;
    return REDISMODULE_OK;
}
