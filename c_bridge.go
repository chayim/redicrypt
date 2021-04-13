package main

import "C"

import (
	"github.com/chayim/redicrypt/src/redicrypt"
)

//export Hash
func Hash(hashType, value *C.char) *C.char {
	hashName := string(C.GoString(hashType))
	hashVal := []byte(C.GoString(value))
	return C.CString(redicrypt.Hash(hashName, hashVal))
}

//export Base64Encode
func Base64Encode(value *C.char) *C.char {
	cstr := redicrypt.B64Encode([]byte(C.GoString(value)))
	return C.CString(cstr)
}

//export Base64Decode
func Base64Decode(encodedText *C.char) *C.char {
	encoded := string(C.GoString(encodedText))
	cstr := redicrypt.B64Decode(encoded)
	return C.CString(string(cstr))
}

//export Encrypt
func Encrypt(encKey *C.char, value *C.char) *C.char {
	secret := []byte(C.GoString(encKey))
	plaintext := []byte(C.GoString(value))
	cstr := redicrypt.Encrypt(secret, plaintext)
	return C.CString(cstr)
}

//export Decrypt
func Decrypt(encKey *C.char, value *C.char) *C.char {
	secret := []byte(C.GoString(encKey))
	encryptedText := []byte(C.GoString(value))
	return C.CString(redicrypt.Decrypt(secret, encryptedText))
}

func main() {}
