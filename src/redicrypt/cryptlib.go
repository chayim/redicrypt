package redicrypt

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"crypto/sha1"
	"crypto/sha256"
	"crypto/sha512"
	"encoding/base64"
	"io"

	"github.com/jzelinskie/whirlpool"
	"golang.org/x/crypto/blake2b"
	"golang.org/x/crypto/blake2s"
	"golang.org/x/crypto/sha3"
)

// Wrapper for hashing functions, returns base64encoded hashes
func Hash(hashName string, hashVal []byte) string {
	switch {

	case hashName == "sha1":
		hv := sha1.Sum(hashVal)
		return B64Encode(hv[:])

	case hashName == "sha224":
		hv := sha256.Sum224(hashVal)
		return B64Encode(hv[:])

	case hashName == "sha256":
		hv := sha256.Sum256(hashVal)
		return B64Encode(hv[:])

	case hashName == "sha3-224":
		hv := sha3.Sum224(hashVal)
		return B64Encode(hv[:])

	case hashName == "sha3-256":
		hv := sha3.Sum256(hashVal)
		return B64Encode(hv[:])

	case hashName == "sha3-384":
		hv := sha3.Sum384(hashVal)
		return B64Encode(hv[:])

	case hashName == "sha3-512":
		hv := sha3.Sum512(hashVal)
		return B64Encode(hv[:])

	case hashName == "sha512-224":
		hv := sha512.Sum512_224(hashVal)
		return B64Encode(hv[:])

	case hashName == "sha512-256":
		hv := sha512.Sum512_256(hashVal)
		return B64Encode(hv[:])

	case hashName == "blake2s-256":
		hv := blake2s.Sum256(hashVal)
		return B64Encode(hv[:])

	case hashName == "blake2b-256":
		hv := blake2b.Sum256(hashVal)
		return B64Encode(hv[:])

	case hashName == "blake2b-384":
		hv := blake2b.Sum384(hashVal)
		return B64Encode(hv[:])

	case hashName == "blake2b-512":
		hv := blake2b.Sum512(hashVal)
		return B64Encode(hv[:])

	case hashName == "whirlpool":
		h := whirlpool.New()
		io.WriteString(h, string(hashVal))
		return B64Encode(h.Sum(nil))
	}
	return ""

}

// Wrapper for encoding, specifically so that we can unit test to go code.
func B64Encode(encoded []byte) string {
	cstr := base64.URLEncoding.EncodeToString(encoded)

	return cstr
}

// Wrapper for decoding, specifically so that we can unit test to go code.
func B64Decode(encoded string) []byte {
	cstr, err := base64.URLEncoding.DecodeString(encoded)
	if err != nil {
		return nil
	}

	return cstr
}

func Encrypt(secret, plaintext []byte) string {

	block, err := aes.NewCipher(secret)
	if err != nil {
		return ""
	}

	ciphertext := make([]byte, aes.BlockSize+len(plaintext))
	iv := ciphertext[:aes.BlockSize]
	if _, err := io.ReadFull(rand.Reader, iv); err != nil {
		return ""
	}

	stream := cipher.NewCFBEncrypter(block, iv)
	stream.XORKeyStream(ciphertext[aes.BlockSize:], plaintext)

	return B64Encode(ciphertext)
}

func Decrypt(secret, cipherText []byte) string {
	block, err := aes.NewCipher(secret)
	if err != nil {
		return ""
	}

	if len(cipherText) < aes.BlockSize {
		return ""
	}

	cstr := B64Decode(string(cipherText))

	iv := cstr[:aes.BlockSize]
	cstr = cstr[aes.BlockSize:]

	stream := cipher.NewCFBDecrypter(block, iv)

	// XORKeyStream can work in-place if the two arguments are the same.
	stream.XORKeyStream(cstr, cstr)
	return string(cstr)

}
