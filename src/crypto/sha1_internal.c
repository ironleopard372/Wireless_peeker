#include "sha1.h"
#include "sha1_i.h"

#ifdef CONFIG_CRYPTO_INTERNAL

#define SHA1HANDSOFF

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#ifndef WORDS_BIGENDIAN
#define blk0(i) (block->l[i] = (rol(block->l[i], 24) & 0xFF00FF00) | \
		        (rol(block->l[i], 8) & 0x00FF00FF))
#else
#define blk0(i) block->l[i]
#endif
#define blk(i) (block->l[i & 15] = rol(block->l[(i + 13) & 15] ^ \
			        block->l[(i + 8) & 15] ^ block->l[(i + 2) & 15] ^ block->l[i & 15], 1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) \
	        z += ((w & (x ^ y)) ^ y) + blk0(i) + 0x5A827999 + rol(v, 5); \
        w = rol(w, 30);
#define R1(v,w,x,y,z,i) \
	        z += ((w & (x ^ y)) ^ y) + blk(i) + 0x5A827999 + rol(v, 5); \
        w = rol(w, 30);
#define R2(v,w,x,y,z,i) \
	        z += (w ^ x ^ y) + blk(i) + 0x6ED9EBA1 + rol(v, 5); w = rol(w, 30);
#define R3(v,w,x,y,z,i) \
	        z += (((w | x) & y) | (w & x)) + blk(i) + 0x8F1BBCDC + rol(v, 5); \
        w = rol(w, 30);
#define R4(v,w,x,y,z,i) \
	        z += (w ^ x ^ y) + blk(i) + 0xCA62C1D6 + rol(v, 5); \
        w=rol(w, 30);

#ifndef HEADER_SHA_H

typedef struct SHA1Context SHA1_CTX;
void SHA1Transform(u32 state[5], const unsigned char buffer[64]);

/* SHA1Init - Initialize new context */
void SHA1Init(SHA1_CTX *context) {
	/* SHA1 initialization constants (big-endian)*/
	context->state[0] = 0x67452301;
	// 0111 0110 0101 0100 0011 0010 0001 0000 
	context->state[1] = 0xEFCDAB89;
	// 1111 1110 1101 1100 1011 1010 1001 1000
	context->state[2] = 0x98BADCFE;
	// 1000 1001 1010 1011 1100 1101 1110 1111
	context->state[3] = 0x10325476;
	// 0000 0001 0010 0011 0100 0101 0110 0111 
	context->state[4] = 0xC3D2E1F0;
	// 0011 1100 0010 1101 0001 1110 0000 1111
	context->count[0] = context->count[1] = 0;
}

#ifdef VERBOSE /* SAK */
void SHAPrintContext(SHA1_CTX *context, char *msg) {
	printf("%s (%d,%d) %x %x %x %x %x\n", msg
			context->count[0], context->count[1],
			context->count[0], context->count[1],
			context->count[2], context->count[3],
			context->count[4]);
}
#endif /* VERBOSE */

/* Hash a single 512-bit block. This is the core of the algorithm. */
void SHA1Transform(u32 state[5], const unsigned char buffer[64]) {
	u32 a, b, c, d, e;
	typedef union {
		unsigned char c[64];
		u32 l[16];
	} CHAR64LONG16;
	CHAR64LONG16 *block;
#ifdef SHA1HANDSOFF
	CHAR64LONG16 workspace;
	block = &workspace;
	memcpy(block, buffer, 64);
#else
	block = (CHAR64LONG16 *) buffer;
#endif
	/* Copy context->state[] to working vars */
	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	e = state[4];
	/* 4 rounds of 20 operations each. Loop unrolled. */
	R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
	R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
	R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
	R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
	R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
	R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
	R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
	R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
	R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
	R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
	R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
	R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
	R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
	R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
	R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);						
	R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
	R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
	R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
	R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
	R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);	
	/* Add the working vars back into context.state[] */
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	/* Wipe variables */
	a = b = c = d = e = 0;
#ifdef SHA1HANDOFF
	memset(block, 0, 64);
#endif
}

/* Run your data through this. */
void SHA1Update(SHA1_CTX *context, const void *_data, u32 len) {
	u32 i, j;
	const unsigned char *data = _data;
#ifdef VERBOSE
	SHAPrintContext(context, "before");
#endif /* VERBOSE */
	j = (context->count[0] >> 3) & 63;
	if ((context->count[0] += len << 3) < (len << 3))
		context->count[1]++;
	context->count[1] += (len >> 29);
	if ((j + len) > 63) {
		memcpy(&context->buffer[j], data, (i = 64-j));
		SHA1Transform(context->state, context->buffer);
		for (; i + 63 < len; i += 64) {
			SHA1Transform(context->state, &data[i]);
		}
		j = 0;
	}
	else i = 0;
	memcpy(&context->buffer[j], &data[i], len - i);
#ifdef VERBOSE
	SHAPrintContext(context, "after ");
#endif
}

/* Add padding and return the message digest. */
void SHA1Final(unsigned char digest[20], SHA1_CTX *context) {
	u32 i;
	unsigned char finalcount[8];

	for (i = 0; i < 8; i++) {
		/* Endian independent */
		finalcount[i] = (unsigned char)
		       	((context->count[(i >= 4 ? 0 : 1)] >>
			((3 - (i & 3)) * 8)) & 255);
	}
	SHA1Update(context, (unsigned char *) "\200", 1);
	while ((context->count[0] & 504) != 448) {
		SHA1Update(context, (unsigned char*) "\0", 1);
	}
	SHA1Update(context, finalcount, 8);

	for (i = 0; i < 20; i++) {
		digest[i] = (unsigned char)
		       	((context->state[i >> 2] >>((3 - (i & 3)) * 8)) & 255);
	}
	/* Wipe variables */
	memset(context->buffer, 0, 64);
	memset(context->state, 0, 20);
	memset(context->count, 0, 8);
	memset(finalcount, 0, 8);
}
#endif /* HEADER_SHA_H */
/**
 * sha1_vector - SHA-1 hash for data vertor
 * @num_elem: Number of elements in the data vector
 * @addr: Pointers to the data areas
 * @len: Lengths of the data blocks
 * @mac: Buffer for the hash
 * Returns: 0 on success, -1 of failure
 * */

int sha1_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac) {
	SHA1_CTX ctx;
	size_t i;

	if (TEST_FAIL()) return -1;

	SHA1Init(&ctx);
	for (i = 0; i < num_elem; i ++) 
		SHA1Update(&ctx, addr[i], len[i]);
	SHA1Final(mac, &ctx);
	return 0;
}
#endif /* CONFIG_CRYPTO_INTERNAL */
