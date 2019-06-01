/* Konstantin Mitish, 11-6, 12.07.2016 */
#ifndef __XOR_H_
#define __XOR_H_
#include <windows.h>

#define DYNAMIC_KEY_LEN 30

#define STATIC_KEY "kzaR0OSGr05DeFksE790vQhSI5lYdNpvn13xCIvCEhFnZcyLRV93lT7ZUbe635YImGwcPhSIZX3XkhCsq0vtLhKoRTpDbPYhzY7fIKT1V4bjE8SMIe40OItskkqm3EXtrwxngo3dGD5IoG7hjMZmFKeXABQaxgTZApLHcC1YgrG9qOJIIb3oTR1lYOMFDB18D9BG9d8p8QpjG2JZMuviU3XucJXOI4M3LiMYh5AagB4ZVW7"

/* Random key generation function.
 * ARGUMENTS:
 *   - Key lenght:
 *       size_t Lenght;
 *   - Dest buffer:
 *       CHAR *OutPut;
 * RETURN: None.
 */
VOID RandKey( BYTE *OutPut, size_t Lenght);

/* Encryption/Decryption funciton.
 * ARGUMENTS:
 *   - String to be encrypted/decrypted:
 *       CHAR *String;
 *   - String lenght:
 *       size_t StrLen;
 *   - Encryption key:
 *       CHAR *Key;
 *   - Key lenght:
 *       size_t KeyLen;
 *   - Dest buffer:
 *       CHAR *OutPut;
 * RETURN: None.
 */
VOID EncryptN( const BYTE *String, size_t StrLen, const BYTE *Key, size_t KeyLen, BYTE *OutPut, size_t *OutPutLen );

/* Encryption funciton with random key.
* ARGUMENTS:
*   - String to be encrypted (End with \0):
*       CHAR *String;
*   - Dest buffer:
*       CHAR *OutPut;
* RETURN: None.
*/
VOID EncryptRN( const BYTE *String, size_t StrLen, BYTE *OutPut, size_t *OutPutLen );

/* Decryption funciton with random key.
* ARGUMENTS:
*   - String to be decrypted (End with \0):
*       CHAR *String;
*   - Dest buffer:
*       CHAR *OutPut;
* RETURN: None.
*/
VOID DecryptRN( const BYTE *String, size_t StrLen, BYTE *OutPut, size_t *OutPutLen );

/* Decryption funciton with random key.
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
VOID DecryptRNX( const BYTE *String, size_t StrLen, const BYTE *StaticKey, size_t KeyLen, BYTE *OutPut, size_t *OutPutLen );

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
VOID EncryptRNX( const BYTE *String, size_t StrLen, const BYTE *StaticKey, size_t KeyLen, BYTE *OutPut, size_t *OutPutLen );

#endif /* __XOR_H_ */