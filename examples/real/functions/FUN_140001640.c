/* Function: FUN_140001640 @ 140001640 */

char * FUN_140001640(longlong param_1,char *param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  ulonglong uVar4;
  char *pcVar5;
  
  if ((param_1 != 0) && (param_2 != (char *)0x0)) {
    cVar1 = *param_2;
    if (cVar1 == '\0') {
      uVar4 = 5;
    }
    else {
      uVar2 = 0x1505;
      pcVar5 = param_2;
      do {
        pcVar5 = pcVar5 + 1;
        uVar2 = uVar2 * 0x21 + (int)cVar1;
        cVar1 = *pcVar5;
      } while (cVar1 != '\0');
      uVar4 = (ulonglong)(uVar2 & 0x3f);
    }
    pcVar5 = *(char **)(param_1 + uVar4 * 8);
    while( true ) {
      if (pcVar5 == (char *)0x0) {
        return (char *)0x0;
      }
      iVar3 = strcmp(pcVar5,param_2);
      if (iVar3 == 0) break;
      pcVar5 = *(char **)(pcVar5 + 0x280);
    }
    return pcVar5 + 0x80;
  }
  return (char *)0x0;
}