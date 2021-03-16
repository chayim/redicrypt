# Redicrypt

## Encrypted data storage and retrieval for Redis

<img src="https://oss.redislabs.com/redisearch/img/logo.svg" alt="logo" width="300"/>

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

Building this module depends on [go 1.14](https://www.golang.org), [gcc 8](https://www.gnu.org/software/gcc) and [make 3.0 or higher](https://www.gnu.org/software/make). 

## Building

1. Clone the repo.

2. Run *make*

## Usage

Set the **REDICRYPT_KEY** environment variable, to the 32-character key/passpharse you want to use for encrypting your data as per the example below. Please do not use the sample key below!

```
REDICRYPT_KEY=12345678901234567890123456789012 redis-server
```

### Cluster support

Redicrypt is cluster agnostic and does not maintain state.  Ensure the module is available on all cluste rhosts, and that thee **REDICRYPT_KEY** environment variable is set accordingly.

### License

Redicrypt is licensed under the [MIT License Agreement](LICENSE).