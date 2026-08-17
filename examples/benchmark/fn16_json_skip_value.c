/* Ghidra decompiled output -- libjson.so */

long FUN_00402e80(long param_1,long param_2)

{
  char cVar1;
  long lVar2;
  int iVar3;
  long lVar4;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 >= param_2) {
    lVar4 = -1;
    goto LAB_00403010;
  }
  while (*(char *)param_1 == ' ' || *(char *)param_1 == '\n' ||
         *(char *)param_1 == '\t') {
    param_1 = param_1 + 1;
    if (param_1 >= param_2) {
      lVar4 = -1;
      goto LAB_00403010;
    }
  }
  cVar1 = *(char *)param_1;
  if (cVar1 == '"') {
    lVar2 = param_1 + 1;
    while (lVar2 < param_2) {
      if (*(char *)lVar2 == '\\') {
        lVar2 = lVar2 + 2;
      }
      else {
        if (*(char *)lVar2 == '"') {
          lVar4 = lVar2 + 1;
          goto LAB_00403010;
        }
        lVar2 = lVar2 + 1;
      }
    }
    lVar4 = -1;
    goto LAB_00403010;
  }
  if ((cVar1 == '{') || (cVar1 == '[')) {
    iVar3 = 1;
    lVar2 = param_1 + 1;
    while ((lVar2 < param_2) && (iVar3 > 0)) {
      cVar1 = *(char *)lVar2;
      if (cVar1 == '"') {
        lVar2 = FUN_00402e80(lVar2,param_2);
        if (lVar2 == -1) {
          lVar4 = -1;
          goto LAB_00403010;
        }
      }
      else {
        if ((cVar1 == '{') || (cVar1 == '[')) {
          iVar3 = iVar3 + 1;
        }
        else if ((cVar1 == '}') || (cVar1 == ']')) {
          iVar3 = iVar3 + -1;
        }
        lVar2 = lVar2 + 1;
      }
    }
    if (iVar3 != 0) {
      lVar4 = -1;
      goto LAB_00403010;
    }
    lVar4 = lVar2;
    goto LAB_00403010;
  }
  lVar2 = param_1;
  while ((lVar2 < param_2) && (*(char *)lVar2 != ',') &&
         (*(char *)lVar2 != '}') && (*(char *)lVar2 != ']') &&
         (*(char *)lVar2 != ' ') && (*(char *)lVar2 != '\n')) {
    lVar2 = lVar2 + 1;
  }
  lVar4 = lVar2;
LAB_00403010:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return lVar4;
}
