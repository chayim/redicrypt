package redicrypt

import (
	"testing"
)

func TestBase64Encoding(t *testing.T) {
	charthing := []byte("these are the days of our lives")
	res := B64Encode(charthing)
	benc := "dGhlc2UgYXJlIHRoZSBkYXlzIG9mIG91ciBsaXZlcw=="
	if res != benc {
		t.Errorf("received %s instead of %s", res, benc)
	}
}

func TestBase64Decoding(t *testing.T) {
	phrase := "these are the days of our lives"
	benc := "dGhlc2UgYXJlIHRoZSBkYXlzIG9mIG91ciBsaXZlcw=="
	res := string(B64Decode(benc))
	if res != phrase {
		t.Errorf("received %s instead of %s", res, phrase)
	}
}

func TestHashing(t *testing.T) {
	phrase := []byte("these are the days of our lives")

	// sha1
	expected := "_2-CY4CRhEmH1o9mOCCpNgbONX4="
	sha1 := Hash("sha1", phrase)
	if expected != sha1 {
		t.Errorf("received %s instead of %s", sha1, expected)
	}

	// sha224
	sha224 := Hash("sha224", phrase)
	expected = "JOUSOU6XViAkX-nMUgte0FU7huRqXag3ls431A=="
	if expected != sha224 {
		t.Errorf("received %s instead of %s", sha224, expected)
	}

	sha256 := Hash("sha256", phrase)
	expected = "Se_qTlwCwsvDSKRoR3VB7faouqfG2Bnd1Afq1g5csZI="
	if expected != sha256 {
		t.Errorf("received %s instead of %s", sha256, expected)
	}

	// sha3-224
	sha3224 := Hash("sha3-224", phrase)
	expected = "7WXdRxLyl1cNlWQ-IIFVPNtgY4h-XqmRKPZ8CA=="
	if expected != sha3224 {
		t.Errorf("received %s instead of %s", sha3224, expected)
	}

	// sha3-256
	sha3256 := Hash("sha3-256", phrase)
	expected = "qNdzthgF18r3a30BIUsCwQXhwELbbu6rqb3AXrekyHQ="
	if expected != sha3256 {
		t.Errorf("received %s instead of %s", sha3256, expected)
	}

	// sha3-384
	sha3384 := Hash("sha3-384", phrase)
	expected = "FdN3K5SiDFXhhLDjJxK9snJVw36eLziRybS7c3wN3OypypbevWrNNSYorEW5GSdm"
	if expected != sha3384 {
		t.Errorf("received %s instead of %s", sha3384, expected)
	}

	// sha3-512
	sha3512 := Hash("sha3-512", phrase)
	expected = "NlOyTm-9c9fwC5PTnEIXJNjZ7We95aW9CiBsrJqnfdp9Eg2-_uUT2gRYRTM1vAXg2XBpz0Pm29uyW6ig1gK2_A=="
	if expected != sha3512 {
		t.Errorf("received %s instead of %s", sha3512, expected)
	}

	// sha512-224
	sha512224 := Hash("sha512-224", phrase)
	expected = "LYzdr3ded2-W8-KFNUuPsLS-99j60bfGIo_8yw=="
	if expected != sha512224 {
		t.Errorf("received %s instead of %s", sha512224, expected)
	}

	// sha512-256
	sha512256 := Hash("sha512-256", phrase)
	expected = "U9B4gMIwdtFVUOMK_FuRGcfyccN3ru8_41xFvk0iyM8="
	if expected != sha512256 {
		t.Errorf("received %s instead of %s", sha512256, expected)
	}

	// blake2s-256
	blake2s256 := Hash("blake2s-256", phrase)
	expected = "rP1TU5-14M6TC1WSE9uHpCfLJpM_GlS-Xevwl0WZYtI="
	if expected != blake2s256 {
		t.Errorf("received %s instead of %s", blake2s256, expected)
	}

	// blake2b-256
	blake2b256 := Hash("blake2b-256", phrase)
	expected = "bmmce1rHuCyz_aqTPuIpLMfe0LV0DAwPYGkExSugKHQ="
	if expected != blake2b256 {
		t.Errorf("received %s instead of %s", blake2b256, expected)
	}

	// blake2b-384
	blake2b384 := Hash("blake2b-384", phrase)
	expected = "LhT_TcaGAXgCVPUMzjoCKuUYXhz9r3SUYS5VNIT_Dmiqcaq7yKJgN7D0WllzbGYq"
	if expected != blake2b384 {
		t.Errorf("received %s instead of %s", blake2b384, expected)
	}

	// blake2b-512
	blake2b512 := Hash("blake2b-512", phrase)
	expected = "6lvBMu_bOviGDzOGZWv1_yWaWJsEItvEpS9hOoROkd0-EAmZu00_N6cEOsRfxhkjRCzfRboRLRYZVLz9rokx2w=="
	if expected != blake2b512 {
		t.Errorf("received %s instead of %s", blake2b512, expected)
	}

	// blake3-256
	blake3256 := Hash("blake3-256", phrase)
	expected = "VmlSQtBhaFTut67AfgnpdbF5U_djohhVvqxYgF5Li1o="
	if expected != blake3256 {
		t.Errorf("received %s instead of %s", blake3256, expected)
	}

	// blake3-512
	blake3512 := Hash("blake3-512", phrase)
	expected = "VmlSQtBhaFTut67AfgnpdbF5U_djohhVvqxYgF5Li1ofqSzVvqc7geX-vCd7FbfccAsT01LNgRn41GDR-iCr_w=="
	if expected != blake3512 {
		t.Errorf("received %s instead of %s", blake3512, expected)
	}

	// whirlpool
	wp := Hash("whirlpool", phrase)
	expected = "deuZC3eAkN0lsktV94r_FP0VBdb6ZW_Y5q2qr6g0KGGMuUdDW3zYAOlSWtoW4-DEfbZSkLxHg-iJZl0dV_vZpw=="
	if expected != wp {
		t.Errorf("received %s instead of %s", wp, expected)
	}

	// BOGUS
	ssd := Hash("IAMABOGUS", phrase)
	expected = ""
	if expected != ssd {
		t.Errorf("received %s instead of %s", ssd, expected)
	}

}

func TestEncrypt(t *testing.T) {
	plaintext := []byte("these are the days of our lives")
	secret := []byte("12345678901234567890123456789012")

	enc := Encrypt(secret, plaintext)
	if enc == string(secret) {
		t.Errorf("received decrypted text")
	}
}

func TestDecrypt(t *testing.T) {
	plaintext := []byte("these are the days of our lives")
	secret := []byte("12345678901234567890123456789012")
	ciphered := []byte(Encrypt(secret, plaintext))

	dec := Decrypt(secret, ciphered)
	if dec != string(plaintext) {
		t.Errorf("failed decrypting")
	}

}
