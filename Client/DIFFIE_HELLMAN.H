/* Konstantin Mitish, 23.05.2019 */
#ifndef __DIFFIE_HELLMAN_H_
#define __DIFFIE_HELLMAN_H_
#include <windows.h>

#define DIFFIE_HELLMAN_G 7
#define DIFFIE_HELLMAN_P 239 
#define DIFFIE_HELLMAN_KEY_LEN 256
#define DIFFIE_HELLMAN_ID_LEN 30

extern BYTE *DiffieHellmanKey;
extern SIZE_T DiffieHellmanKeyLen;

extern BYTE DiffieHellmanId[DIFFIE_HELLMAN_ID_LEN];

extern BYTE *DiffieHellmanSecret;
extern SIZE_T DiffieHellmanSecretLen;

VOID DiffieHellman( const BYTE *Base, const BYTE *Secret, size_t Len, BYTE *OutPut, size_t *OutPutLen);
BOOL DiffieHellmanInit( size_t KeyLen, BYTE *OutPut, size_t *OutPutLen );
INT DiffieHellmanStep( BYTE *InPut, size_t InPutLen );

BYTE DiffieHellmanX( BYTE Base, BYTE Secret );

#endif /* __DIFFIE_HELLMAN_H_ */
