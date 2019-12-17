#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Encrypt p into c and write authentication tag to t
void pcm_siv_encrypt(unsigned kbytes, unsigned char k[kbytes],  // 16 or 32
                     unsigned nbytes, unsigned char n[nbytes],  // 0 to 16
                     unsigned pbytes, unsigned char p[pbytes],
                     unsigned char c[pbytes], unsigned char t[16]);

int pcm_siv_decrypt(unsigned kbytes, unsigned char k[kbytes],  // 16 or 32
                    unsigned nbytes, unsigned char n[nbytes],  // 0 to 16
                    unsigned cbytes, unsigned char c[cbytes],
                    unsigned char p[cbytes], unsigned char t[16]);

int main() {
    unsigned char n[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    unsigned char p[1024] = {0};
    unsigned char c[1024];
    unsigned char r[1024];
    unsigned char t[16];
    
    unsigned res, klen, nlen, plen;
    
    klen=16, nlen=0, plen=0;
    pcm_siv_encrypt(klen,n,nlen,n,plen,p,c,t);
    res = pcm_siv_decrypt(klen,n,nlen,n,plen,c,r,t) &&
          !memcmp(t,"\x62\xC6\xA0\x0A\x0D\xCC\xE7\x34\x04\xCA\xEC\x8C\xCC\x39\x41\x4F",16) &&
          !memcmp(p,r,plen);
    printf("%d\n",res);
    
    klen=16, nlen=1, plen=1;
    pcm_siv_encrypt(klen,n,nlen,n,plen,p,c,t);
    res = pcm_siv_decrypt(klen,n,nlen,n,plen,c,r,t) &&
        !memcmp(t,"\x03\x87\xC3\x31\x7C\xB5\xD1\x4C\xD0\x85\x62\xD1\x53\xCE\xCD\xDA",16) &&
        !memcmp(p,r,plen);
    printf("%d\n",res);
    
    klen=16, nlen=15, plen=15;
    pcm_siv_encrypt(klen,n,nlen,n,plen,p,c,t);
    res = pcm_siv_decrypt(klen,n,nlen,n,plen,c,r,t) &&
        !memcmp(t,"\x4F\x67\x39\xE7\x26\xC6\x18\xBD\x11\xE1\x8F\x07\x88\x73\x3B\xE2",16) &&
        !memcmp(p,r,plen);
    printf("%d\n",res);
    
    klen=16, nlen=16, plen=16;
    pcm_siv_encrypt(klen,n,nlen,n,plen,p,c,t);
    res = pcm_siv_decrypt(klen,n,nlen,n,plen,c,r,t) &&
        !memcmp(t,"\x94\xAE\x76\xD7\x12\x53\x7F\xF6\x5B\xD0\xAD\x46\x86\xB7\xE3\xA3",16) &&
        !memcmp(p,r,plen);
    printf("%d\n",res);
    
    klen=16, nlen=16, plen=1024;
    pcm_siv_encrypt(klen,n,nlen,n,plen,p,c,t);
    res = pcm_siv_decrypt(klen,n,nlen,n,plen,c,r,t) &&
        !memcmp(t,"\x3B\x6C\x59\x93\x0A\x99\x77\xFA\x7D\xA6\x61\x6A\x32\x0D\x79\x68",16) &&
        !memcmp(p,r,plen);
    printf("%d\n",res);
}