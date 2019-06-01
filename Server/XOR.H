/* Konstantin Mitish, 11-6, 12.07.2016 */
#ifndef __XOR_H_
#define __XOR_H_
#include <windows.h>

#define STATIC_KEY "kzaR0OSGr05DeFksE790vQhSI5lYdNpvn13xCIvCEhFnZcyLRV93lT7ZUbe635YImGwcPhSIZX3XkhCsq0vtLhKoRTpDbPYhzY7fIKT1V4bjE8SMIe40OItskkqm3EXtrwxngo3dGD5IoG7hjMZmFKeXABQaxgTZApLHcC1YgrG9qOJIIb3oTR1lYOMFDB18D9BG9d8p8QpjG2JZMuviU3XucJXOI4M3LiMYh5AagB4ZVW7"

/* Random key generation function.
 * ARGUMENTS:
 *   - Key lenght:
 *       int Lenght;
 *   - Dest buffer:
 *       char *OutPut;
 * RETURN: None.
 */
void RandKey( size_t Lenght, char *OutPut );

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
void EncryptN( char *String, size_t StrLen, char *Key, size_t KeyLen, char *OutPut );

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
int Encrypt( char *String, char *Key, char *OutPut );

/* Encryption funciton with random key.
 * ARGUMENTS:
 *   - String to be encrypted (End with \0):
 *       char *String;
 *   - Dest buffer:
 *       char *OutPut;
 * RETURN: None.
 */
void EncryptR(char *String, char *OutPut);

/* Decryption funciton with random key.
 * ARGUMENTS:
 *   - String to be decrypted (End with \0):
 *       char *String;
 *   - Dest buffer:
 *       char *OutPut;
 * RETURN: None.
 */
void DecryptR(char *String, char *OutPut);

/* Encryption funciton with random key.
* ARGUMENTS:
*   - String to be encrypted (End with \0):
*       char *String;
*   - Dest buffer:
*       char *OutPut;
* RETURN: None.
*/
void EncryptRN(char *String, size_t StrLen, char *OutPut);

/* Decryption funciton with random key.
* ARGUMENTS:
*   - String to be decrypted (End with \0):
*       char *String;
*   - Dest buffer:
*       char *OutPut;
* RETURN: None.
*/
void DecryptRN(char *String, size_t StrLen, char *OutPut);
#endif /* __XOR_H_ */