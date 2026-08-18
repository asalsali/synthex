/* Function: FUN_140001700 @ 140001700 */

undefined4 FUN_140001700(longlong param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  char *_Str1;
  uint uVar3;
  int iVar4;
  char *pcVar5;
  
  if ((param_1 != 0) && (param_2 != (char *)0x0)) {
    cVar1 = *param_2;
    if (cVar1 == '\0') {
      uVar3 = 5;
    }
    else {
      uVar3 = 0x1505;
      pcVar5 = param_2;
      do {
        pcVar5 = pcVar5 + 1;
        uVar3 = uVar3 * 0x21 + (int)cVar1;
        cVar1 = *pcVar5;
      } while (cVar1 != '\0');
      uVar3 = uVar3 & 0x3f;
    }
    pcVar5 = *(char **)(param_1 + (ulonglong)uVar3 * 8);
    _Str1 = (char *)0x0;
    while (pcVar2 = _Str1, _Str1 = pcVar5, _Str1 != (char *)0x0) {
      iVar4 = strcmp(_Str1,param_2);
      pcVar5 = *(char **)(_Str1 + 0x280);
      if (iVar4 == 0) {
        if (pcVar2 == (char *)0x0) {
          *(char **)(param_1 + (ulonglong)uVar3 * 8) = pcVar5;
        }
        else {
          *(char **)(pcVar2 + 0x280) = pcVar5;
        }
        free(_Str1);
        *(int *)(param_1 + 0x200) = *(int *)(param_1 + 0x200) + -1;
        return 0;
      }
    }
  }
  return 0xffffffff;
}