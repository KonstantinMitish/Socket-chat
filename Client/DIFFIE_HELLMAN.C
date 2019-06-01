#include <math.h>
#include "DIFFIE_HELLMAN.H"


BYTE *DiffieHellmanKey = NULL;
SIZE_T DiffieHellmanKeyLen = 0;
#define ID_LEN DIFFIE_HELLMAN_ID_LEN

BYTE DiffieHellmanId[ID_LEN];

BYTE *DiffieHellmanSecret = NULL;
SIZE_T DiffieHellmanSecretLen = 0;

unsigned powmod(unsigned base, unsigned exp, unsigned modulo)
{
  unsigned res = 1;

  while (exp != 0)
  {
    if ((exp & 1) != 0)
    {
      res = (1ll * res * base) % modulo;
    }

    base = (1ll * base * base) % modulo;
    exp >>= 1;
  }

  return res;
}

BYTE DiffieHellmanX( BYTE Base, BYTE Secret )
{
  return powmod(Base, Secret, DIFFIE_HELLMAN_P);
}

VOID DiffieHellman( const BYTE *Base, const BYTE *Secret, size_t Len, BYTE *OutPut, size_t *OutPutLen )
{
  size_t i;
  for (i = 0; i < Len && i < *OutPutLen; i++)
    OutPut[i] = DiffieHellmanX(Base[i], Secret[i]);
  *OutPutLen = i;
}

VOID DiffieHellmanRand( BYTE *OutPut, size_t Lenght )
{
  size_t i;
  for (i = 0; i < Lenght; i++)
    OutPut[i] = rand() & 0xFF;
}

VOID DiffieHellmanGen()
{
  size_t i;
  for (i = 0; i < DiffieHellmanKeyLen; i++)
    DiffieHellmanKey[i] = DIFFIE_HELLMAN_G;
}

BOOL DiffieHellmanInit( size_t KeyLen, BYTE *OutPut, size_t *OutPutLen )
{
  if (*OutPutLen < KeyLen + ID_LEN)
    return FALSE;
  *OutPutLen = KeyLen + ID_LEN;
  if (DiffieHellmanKey != NULL)
    free(DiffieHellmanKey);
  if (DiffieHellmanSecret != NULL)
    free(DiffieHellmanSecret);
  DiffieHellmanKey = malloc(KeyLen);
  DiffieHellmanSecret = malloc(KeyLen);
  DiffieHellmanKeyLen = KeyLen;
  DiffieHellmanSecretLen = KeyLen;
  DiffieHellmanRand(DiffieHellmanId, ID_LEN);
  DiffieHellmanRand(DiffieHellmanSecret, DiffieHellmanSecretLen);
  DiffieHellmanGen();
  memcpy(OutPut, DiffieHellmanId, ID_LEN);
  memcpy(OutPut + ID_LEN, DiffieHellmanKey, DiffieHellmanKeyLen);
  return TRUE;
}


INT DiffieHellmanStep( BYTE *InPut, size_t InPutLen )
{
  if (InPutLen < ID_LEN + DiffieHellmanKeyLen ||
    DiffieHellmanSecret == NULL)
    return 0;
  memcpy(DiffieHellmanKey, InPut + ID_LEN, DiffieHellmanKeyLen);
  DiffieHellman(DiffieHellmanKey, DiffieHellmanSecret, DiffieHellmanKeyLen, DiffieHellmanKey, &DiffieHellmanKeyLen);
  if (strncmp(InPut, DiffieHellmanId, ID_LEN) == 0)
    return 2;
  memcpy(InPut + ID_LEN, DiffieHellmanKey, DiffieHellmanKeyLen);
  return 1;
}