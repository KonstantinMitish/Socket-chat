/* Konstantin Mitish, 11-6, 12.07.2016 */
#include <windows.h>
#include <stdlib.h>

#include "xor.h"

/* Random key generation function.
 * ARGUMENTS:
 *   - Key lenght:
 *       int Lenght;
 *   - Dest buffer:
 *       char *OutPut;
 * RETURN: None.
 */
void RandKey( size_t Lenght, char *OutPut )
{
  int i;

  for (i = 0; i < Lenght;)
  {
    char c = rand() & 0xFF;

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
      OutPut[i] = c, i++;
  }

}

/* Encryption/Decryption funciton.
 * ARGUMENTS:
 *   - String to be encrypted/decrypted:
 *       char *String;
 *   - String lenght:
 *       int StrLen;
 *   - Encryption key:
 *       char *Key;
 *   - Key lenght:
 *       int KeyLen;
 *   - Dest buffer:
 *       char *OutPut;
 * RETURN: None.
 */
void EncryptN( char *String, size_t StrLen, char *Key, size_t KeyLen, char *OutPut )
{
  int i, j = 0;

  for (i = 0; i < StrLen; i++)
  {
    OutPut[i] = String[i] ^ Key[j];
    if(++j >= KeyLen)
      j = 0;
  }
  OutPut[i] = 0;
}

/* Encryption/Decryption funciton.
 * ARGUMENTS:
 *   - String to be encrypted/decrypted (End with \0): 
 *       char *String;
 *   - Encryption key  (End with \0):
 *       char *Key;
 *   - Dest buffer:
 *       char *OutPut;
 * RETURN: (int)Encrypted string leght.
 */
int Encrypt( char *String, char *Key, char *OutPut )
{
  int StrLen = 0, KeyLen = 0;

  while (String[StrLen] != 0)
    StrLen++;
  while (Key[KeyLen] != 0)
    KeyLen++;
  EncryptN(String, StrLen, Key, KeyLen, OutPut);
  return StrLen;
}

/* Encryption funciton with random key.
* ARGUMENTS:
*   - String to be encrypted (End with \0):
*       char *String;
*   - Dest buffer:
*       char *OutPut;
* RETURN: None.
*/
void EncryptR(char *String, char *OutPut)
{
  char Key[30];
  int l, i;

  RandKey(30, Key);
  l = Encrypt(String, Key, OutPut);
  for (i = 0; i < 30; i++)
    OutPut[l + i] = Key[i];
  OutPut[l + 30] = 0;
  Encrypt(OutPut, STATIC_KEY, OutPut);
}

/* Encryption funciton with random key.
* ARGUMENTS:
*   - String to be encrypted (End with \0):
*       char *String;
*   - Dest buffer:
*       char *OutPut;
* RETURN: None.
*/
void EncryptRN(char *String, size_t StrLen, char *OutPut)
{
  char Key[30];
  int i;

  RandKey(30, Key);
  EncryptN(String, StrLen, Key, 30, OutPut);
  for (i = 0; i < 30; i++)
    OutPut[StrLen + i] = Key[i];
  EncryptN(OutPut, StrLen + 30, STATIC_KEY, 239, OutPut);
}

/* Decryption funciton with random key.
* ARGUMENTS:
*   - String to be decrypted (End with \0):
*       char *String;
*   - Dest buffer:
*       char *OutPut;
* RETURN: None.
*/
void DecryptR(char *String, char *OutPut)
{
  char Key[30];
  int StrLen = 0, KeyLen = 0, i;

  Encrypt(String, STATIC_KEY, String);
  while (String[StrLen] != 0)
    StrLen++;
  for (i = 0; i < 30; i++)
    Key[i] = String[StrLen - 30 + i];
  String[StrLen - 30] = 0;
  EncryptN(String, StrLen - 30, Key, 30, OutPut);
}

/* Decryption funciton with random key.
* ARGUMENTS:
*   - String to be decrypted (End with \0):
*       char *String;
*   - Dest buffer:
*       char *OutPut;
* RETURN: None.
*/
void DecryptRN( char *String, size_t StrLen, char *OutPut )
{
  char Key[30];
  int i;

  StrLen += 30;
  EncryptN(String, StrLen, STATIC_KEY, 239, String);
  for (i = 0; i < 30; i++)
    Key[i] = String[StrLen - 30 + i];
  String[StrLen - 30] = 0;
  EncryptN(String, StrLen - 30, Key, 30, OutPut);
}