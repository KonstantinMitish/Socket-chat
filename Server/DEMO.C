/* Konstantin Mitish, 11-6, 12.07.2016 */
#include <stdio.h>
#include <conio.h>
#include <commondf.h>
#include "xor.h"

VOID main( VOID )
{
  CHAR *Str = "SLAVA_Ukraine!";
  CHAR bufE[239];
  CHAR bufD[239];
  EncryptR(Str, bufE);
  DecryptR(bufE, bufD);
  printf("%s\n%s\n%s\n", Str, bufE, bufD);
  _getch();
}