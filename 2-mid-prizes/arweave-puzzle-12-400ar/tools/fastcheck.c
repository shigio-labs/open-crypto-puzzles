// Fast pre-filter for Arweave Puzzle Weave #12, same pipeline as oracle.py:
// candidate -> SHA-512 x11513 -> hex -> EvpKDF (MD5, 10,000 iterations, 144 bytes)
// -> Rijndael Nk=32/Nr=38 (CryptoJS keySize=32 quirk), first ciphertext block only.
// A candidate is reported when the first plaintext block starts with {"kty":"RSA"
// (JWK) or is a printable block opening with '{' (PRINTABLE); every report must then
// be confirmed with oracle.py, which decrypts everything and compares the address.
//
// Build:   gcc -O3 -march=native -fopenmp -o fastcheck fastcheck.c -lcrypto
// Run:     ./fastcheck <salt_hex> <first_block_hex> < candidates.txt
//   #12:   ./fastcheck 3945d43884fd6125 3069bb3ea92a31f24be698e972c55801
//   #8:    ./fastcheck ab7c5aa4d8826e3d 288eec64c230a06de1786f067847c595
// Witness: run the #8 parameters on the same list with RasputinWilhelmAlekhine
// inserted at head, middle and tail; all three must come back as JWK.
// About 117 candidates per second on 4 CPU cores (2026-10-02).
#define OPENSSL_SUPPRESS_DEPRECATED
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>

static const uint8_t SBOX[256]={
0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16};
static uint8_t INV[256];
static const uint8_t RCON[15]={0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36,0x6C,0xD8,0xAB,0x4D};
static uint8_t salt[8], ct0[16];

static uint32_t subw(uint32_t w){return ((uint32_t)SBOX[w>>24]<<24)|((uint32_t)SBOX[(w>>16)&255]<<16)|((uint32_t)SBOX[(w>>8)&255]<<8)|SBOX[w&255];}
static uint8_t xt(uint8_t a){return (uint8_t)((a<<1)^((a&0x80)?0x1B:0));}
static uint8_t gm(uint8_t a,uint8_t b){uint8_t p=0;while(b){if(b&1)p^=a;a=xt(a);b>>=1;}return p;}

static void decrypt_first(const uint8_t key[128], uint8_t out[16]){
  const int nk=32,nr=38,tw=4*(nr+1); uint32_t w[156];
  for(int i=0;i<nk;i++) w[i]=((uint32_t)key[4*i]<<24)|((uint32_t)key[4*i+1]<<16)|((uint32_t)key[4*i+2]<<8)|key[4*i+3];
  for(int o=nk;o<tw;o++){uint32_t s=w[o-1];
    if(o%nk==0){s=((s<<8)|(s>>24)); s=subw(s)^((uint32_t)RCON[o/nk]<<24);} else if(o%nk==4) s=subw(s);
    w[o]=w[o-nk]^s;}
  uint8_t st[16],t[16]; memcpy(st,ct0,16);
  #define ARK(r) for(int c=0;c<4;c++){uint32_t k=w[(r)*4+c];st[4*c]^=k>>24;st[4*c+1]^=(k>>16)&255;st[4*c+2]^=(k>>8)&255;st[4*c+3]^=k&255;}
  ARK(nr)
  for(int rnd=nr-1;rnd>=0;rnd--){
    for(int r=0;r<4;r++)for(int c=0;c<4;c++)t[r+4*c]=st[r+4*((c-r+4)%4)];
    for(int i=0;i<16;i++)st[i]=INV[t[i]];
    ARK(rnd)
    if(rnd>0){for(int c=0;c<4;c++){uint8_t a=st[4*c],b=st[4*c+1],d=st[4*c+2],e=st[4*c+3];
      st[4*c]=gm(a,14)^gm(b,11)^gm(d,13)^gm(e,9); st[4*c+1]=gm(a,9)^gm(b,14)^gm(d,11)^gm(e,13);
      st[4*c+2]=gm(a,13)^gm(b,9)^gm(d,14)^gm(e,11); st[4*c+3]=gm(a,11)^gm(b,13)^gm(d,9)^gm(e,14);}}
  }
  memcpy(out,st,16);
}

static int test(const char *cand, uint8_t pt[16]){
  uint8_t h[64]; SHA512((const uint8_t*)cand,strlen(cand),h);
  for(int i=1;i<11513;i++) SHA512(h,64,h);
  char hex[129]; static const char *hx="0123456789abcdef";
  for(int i=0;i<64;i++){hex[2*i]=hx[h[i]>>4];hex[2*i+1]=hx[h[i]&15];}
  uint8_t der[144]; uint8_t prev[16]; int have=0, dl=0;
  uint8_t buf[16+128+8];
  while(dl<144){ int n=0; if(have){memcpy(buf,prev,16);n=16;} memcpy(buf+n,hex,128);n+=128; memcpy(buf+n,salt,8);n+=8;
    uint8_t b[16]; MD5(buf,n,b); for(int i=1;i<10000;i++) MD5(b,16,b);
    memcpy(der+dl,b,16); dl+=16; memcpy(prev,b,16); have=1; }
  uint8_t o[16]; decrypt_first(der,o);
  for(int i=0;i<16;i++) pt[i]=o[i]^der[128+i];
  if(memcmp(pt,"{\"kty\":\"RSA\"",12)==0) return 2;
  if(pt[0]!='{') return 0;
  for(int i=0;i<16;i++) if(pt[i]<32||pt[i]>126) return 0;
  return 1;
}

static void unhex(const char*s,uint8_t*o,int n){for(int i=0;i<n;i++){unsigned v;sscanf(s+2*i,"%2x",&v);o[i]=v;}}

int main(int argc,char**argv){
  for(int i=0;i<256;i++) INV[SBOX[i]]=i;
  if(argc<3){fprintf(stderr,"usage\n");return 2;}
  unhex(argv[1],salt,8); unhex(argv[2],ct0,16);
  size_t cap=1<<16; char **c=malloc(cap*sizeof(char*)); char line[4096]; long total=0, hits=0;
  for(;;){ size_t n=0;
    while(n<cap && fgets(line,sizeof line,stdin)){ size_t L=strlen(line); while(L&&(line[L-1]=='\n'||line[L-1]=='\r')) line[--L]=0; if(!L) continue; c[n++]=strdup(line);} 
    if(!n) break;
    #pragma omp parallel for schedule(dynamic,4) reduction(+:hits)
    for(long i=0;i<(long)n;i++){ uint8_t pt[16]; int r=test(c[i],pt);
      if(r){ hits++;
        #pragma omp critical
        { printf("%s\t%s\t",r==2?"JWK":"PRINTABLE",c[i]); fwrite(pt,1,16,stdout); printf("\n"); fflush(stdout);} } }
    for(size_t i=0;i<n;i++) free(c[i]); total+=n;
    if(n<cap) break;
  }
  fprintf(stderr,"tested %ld hits %ld\n",total,hits);
  return 0;
}
