package main

import "C"

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"encoding/base64"
	"io"
)

//export Encrypt
func Encrypt(encKey *C.char, value *C.char) *C.char {
	secret := []byte(C.GoString(encKey))
	plaintext := []byte(C.GoString(value))

	block, err := aes.NewCipher(secret)
	if err != nil {
		panic(err)
	}

	ciphertext := make([]byte, aes.BlockSize+len(plaintext))
	iv := ciphertext[:aes.BlockSize]
	if _, err := io.ReadFull(rand.Reader, iv); err != nil {
		panic(err)
	}

	stream := cipher.NewCFBEncrypter(block, iv)
	stream.XORKeyStream(ciphertext[aes.BlockSize:], plaintext)
	cstr := base64.URLEncoding.EncodeToString(ciphertext)
	return C.CString(cstr)
}

//export Decrypt
func Decrypt(encKey *C.char, value *C.char) *C.char {

	secret := []byte(C.GoString(encKey))
	encryptedText := C.GoString(value)

	block, err := aes.NewCipher(secret)
	if err != nil {
		panic(err)
	}

	if len(encryptedText) < aes.BlockSize {
		panic("ciphertext too short")
	}

	cstr, err := base64.URLEncoding.DecodeString(encryptedText)
	if err != nil {
		panic(err)
	}

	iv := cstr[:aes.BlockSize]
	cstr = cstr[aes.BlockSize:]

	stream := cipher.NewCFBDecrypter(block, iv)

	// XORKeyStream can work in-place if the two arguments are the same.
	stream.XORKeyStream(cstr, cstr)

	return C.CString(string(cstr))

}

func main() {}
