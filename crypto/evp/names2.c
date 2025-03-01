/*
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <stdio.h>
#include "internal/cryptlib.h"
#include <openssl/evp.h>
#include <internal/objects.h>
#include <openssl/x509.h>
#include "internal/evp_int.h"

/*
 * use MD5 as default:
 *	X509_REQ_to_X509		x509_r2x.c
 *	X509_issuer_and_serial_hash	x509_cmp.c
 *	X509_NAME_hash_old		x509_cmp.c
 *	PEM_ASN1_write_bio		pem_lib.c
 */
const EVP_MD *EVP_get_default_digest(void)
{
#if !defined(OPENSSL_NO_MD5)
	return EVP_md5();
#elif !defined(OPENSSL_NO_SHA)
	return EVP_sha1();
#elif !defined(OPENSSL_NO_SM3)
	return EVP_sm3();
#elif !defined(OPENSSL_NO_RIPEMD)
	return EVP_rmd160();
#else
	return NULL;
#endif
}

static void cipher_name_len(const EVP_CIPHER *cipher, const char *from,
	const char *to, void *x)
{
	*((int *)x) += strlen(EVP_CIPHER_name(cipher));
}

static void cipher_name(const EVP_CIPHER *cipher, const char *from,
	const char *to, void *x)
{
	strcat((char *)x, EVP_CIPHER_name(cipher));
}

char *EVP_get_ciphernames(int aliases)
{
	char *ret = NULL;
	int len = 0;
	EVP_CIPHER_do_all_sorted(cipher_name_len, &len);

	ret = OPENSSL_zalloc(len);
	if (!ret) {
		return NULL;
	}

	EVP_CIPHER_do_all_sorted(cipher_name, ret);
	return ret;
}

char *EVP_get_digestnames(int aliases)
{
	return "sm3:sha1:sha256";
}

