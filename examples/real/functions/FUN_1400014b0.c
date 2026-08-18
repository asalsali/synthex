/* Function: FUN_1400014b0 @ 1400014b0 */

undefined4 FUN_1400014b0(longlong param_1,char *param_2,char *param_3)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  size_t sVar4;
  char *pcVar5;
  char *pcVar6;
  
  if ((((param_2 != (char *)0x0 && param_3 != (char *)0x0) && (param_1 != 0)) &&
      (sVar4 = strlen(param_2), sVar4 < 0x80)) && (sVar4 = strlen(param_3), sVar4 < 0x200)) {
    cVar1 = *param_2;
    if (cVar1 == '\0') {
      uVar2 = 5;
    }
    else {
      uVar2 = 0x1505;
      pcVar6 = param_2;
      do {
        pcVar6 = pcVar6 + 1;
        uVar2 = uVar2 * 0x21 + (int)cVar1;
        cVar1 = *pcVar6;
      } while (cVar1 != '\0');
      uVar2 = uVar2 & 0x3f;
    }
    pcVar6 = *(char **)(param_1 + (ulonglong)uVar2 * 8);
    for (pcVar5 = pcVar6; pcVar5 != (char *)0x0; pcVar5 = *(char **)(pcVar5 + 0x280)) {
      iVar3 = strcmp(pcVar5,param_2);
      if (iVar3 == 0) {
        strncpy(pcVar5 + 0x80,param_3,0x1ff);
        pcVar5[0x27f] = '\0';
        return 0;
      }
    }
    pcVar5 = malloc(0x288);
    if (pcVar5 != (char *)0x0) {
      strncpy(pcVar5,param_2,0x7f);
      pcVar5[0x7f] = '\0';
      strncpy(pcVar5 + 0x80,param_3,0x1ff);
      *(char **)(param_1 + (ulonglong)uVar2 * 8) = pcVar5;
      *(int *)(param_1 + 0x200) = *(int *)(param_1 + 0x200) + 1;
      pcVar5[0x27f] = '\0';
      *(char **)(pcVar5 + 0x280) = pcVar6;
      return 0;
    }
  }
  return 0xffffffff;
}