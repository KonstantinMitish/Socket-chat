/* Konstantin Mitish, 11-6, 12.07.2016 */
#include <windows.h>
#include <stdlib.h>

#include "xor.h"

/* Random key generation function.
 * ARGUMENTS:
 *   - Key lenght:
 *       size_t Lenght;
 *   - Dest buffer:
 *       BYTE *OutPut;
 * RETURN: None.
 */
VOID RandKey( BYTE *OutPut, size_t Lenght )
{
  size_t i;

  for (i = 0; i < Lenght; i++)
    OutPut[i] = rand() & 0xFF;
}

/* Encryption/Decryption funciton.
 * ARGUMENTS:
 *   - String to be encrypted/decrypted:
 *       BYTE *String;
 *   - String lenght:
 *       size_t StrLen;
 *   - Encryption key:
 *       BYTE *Key;
 *   - Key lenght:
 *       size_t KeyLen;
 *   - Dest buffer:
 *       BYTE *OutPut;
 * RETURN: None.
 */
VOID EncryptN( const BYTE *String, size_t StrLen, const BYTE *Key, size_t KeyLen, BYTE *OutPut, size_t *OutPutLen )
{
  size_t i, j = 0;

  for (i = 0; i < StrLen && i < *OutPutLen; i++)
  {
    OutPut[i] = String[i] ^ Key[j];
    if(++j >= KeyLen)
      j = 0;
  }
  *OutPutLen = i;
}

/* Decryption funciton with random key.
 * ARGUMENTS:
 *   - String to be decrypted:
 *       BYTE *String;
 *   - String length:
 *       size_t StrLen;
 *   - Static key:
 *       BYTE *StaticKey;
 *   - Static key length:
 *       size_t KeyLen;
 *   - Dest buffer:
 *       BYTE *OutPut;
 * RETURN: None.
 */
VOID DecryptRNX( const BYTE *String, size_t StrLen, const BYTE *StaticKey, size_t KeyLen, BYTE *OutPut, size_t *OutPutLen )
{
    BYTE Key[DYNAMIC_KEY_LEN];
    size_t i;

    EncryptN(String, StrLen, StaticKey, KeyLen, OutPut, OutPutLen);
    for (i = 0; i < DYNAMIC_KEY_LEN; i++)
        Key[i] = OutPut[StrLen - DYNAMIC_KEY_LEN + i];
    EncryptN(OutPut, StrLen - DYNAMIC_KEY_LEN, Key, DYNAMIC_KEY_LEN, OutPut, OutPutLen);
}

/* Encryption funciton with random key.
 * ARGUMENTS:
 *   - String to be decrypted:
 *       CHAR *String;
 *   - String length:
 *       size_t StrLen;
 *   - Static key:
 *       CHAR *StaticKey;
 *   - Static key length:
 *       size_t KeyLen;
 *   - Dest buffer:
 *       CHAR *OutPut;
 * RETURN: None.
*/
VOID EncryptRNX( const BYTE *String, size_t StrLen, const BYTE *StaticKey, size_t KeyLen, BYTE *OutPut, size_t *OutPutLen )
{
  BYTE Key[DYNAMIC_KEY_LEN];
  size_t i, templen = *OutPutLen - DYNAMIC_KEY_LEN;

  RandKey(Key, DYNAMIC_KEY_LEN);
  EncryptN(String, StrLen, Key, DYNAMIC_KEY_LEN, OutPut, &templen);
  for (i = 0; i < DYNAMIC_KEY_LEN; i++)
      OutPut[templen + i] = Key[i];
  EncryptN(OutPut, templen + DYNAMIC_KEY_LEN, StaticKey, KeyLen, OutPut, OutPutLen);
}

/* Encryption funciton with random key.
 * ARGUMENTS:
 *   - String to be encrypted (End with \0):
 *       CHAR *String;
 *   - Dest buffer:
 *       CHAR *OutPut;
 * RETURN: None.
 */
VOID EncryptRN( const BYTE *String, size_t StrLen, BYTE *OutPut, size_t *OutPutLen )
{
    EncryptRNX(String, StrLen, STATIC_KEY, 239, OutPut, OutPutLen);
}

/* Decryption funciton with random key.
* ARGUMENTS:
*   - String to be decrypted (End with \0):
*       CHAR *String;
*   - Dest buffer:
*       CHAR *OutPut;
* RETURN: None.
*/
VOID DecryptRN( const BYTE *String, size_t StrLen, BYTE *OutPut, size_t *OutPutLen )
{
    DecryptRNX(String, StrLen, STATIC_KEY, 239, OutPut, OutPutLen);
}

