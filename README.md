# Redicrypt - Encrypted data storage and retrieval for Redis

## Overview

Redicrypt stores strings, encrypted, in redis. Encryption takes place on the redis server, meaning redis can [be configured with TLS](https://redis.io/topics/encryption), and then data at rest, will be stored encrypted. Data is encrypted using 256-bit AES, and then stored base64 encoded.

It does this by introducing two new redis commands, one for storing an encrypted key value, and one for retrieving the decrypted value of the key. Key names are stored in plaintext, and only their values are decrypted.

* SETENC - Sets a key to an encrypted value
     - eg: SETENC *somekey* *myvalue*

* GETDEC - Gets the decrypted value of a key
    - eg: GETDEC *somekey*
----------------------

## Building

### Dependencies

Building this module depends on [go 1.14](https://www.golang.org), [gcc 8](https://www.gnu.org/software/gcc) and [make 3.0 or higher](https://www.gnu.org/software/make). You also need a copy of wget on your system, so that the latest copy of the [redis modules sdk](https://raw.githubusercontent.com/redis/redis/unstable/src/redismodule.h) can be downloaded.

## Building

1. Clone the repo.

2. Run *make*

### Why it works this way

Redicrypt started as a random project with the goal of building a redis module, using go. It uses the go compiler to generate a header file, and static archive. The header file is used by the [redismodule wrapper](redicrypt.c) which in turns calls the go code for all encryption and decryption. The static archive is combined with the object built from the wrapper, into a shared object, which redis loads. For more information [read the Makefile](Makefile).

## Usage

Set the **REDICRYPT_KEY** environment variable, to the 32-character key/passpharse you want to use for encrypting your data as per the example below. Please do not use the sample key below!

```
REDICRYPT_KEY=12345678901234567890123456789012 redis-server
```

### Cluster support

Redicrypt is cluster agnostic and does not maintain state.  Ensure the module is available on all cluste rhosts, and that thee **REDICRYPT_KEY** environment variable is set accordingly.

### License

Redicrypt is licensed under the [MIT License Agreement](LICENSE).
