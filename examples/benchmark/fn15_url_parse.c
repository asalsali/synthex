/* Ghidra decompiled output -- libhttp.so */

undefined4 FUN_00402cc0(long param_1,long param_2)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  undefined4 uVar5;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if ((param_1 == 0) || (param_2 == 0)) {
    uVar5 = 0xffffffff;
    goto LAB_00402e40;
  }
  memset((void *)param_2,0,0x120);
  lVar3 = param_1;
  iVar1 = *(int *)(param_1);
  if ((iVar1 == 0x70747468) &&
      (*(char *)(param_1 + 4) == ':' && *(char *)(param_1 + 5) == '/' &&
       *(char *)(param_1 + 6) == '/')) {
    *(undefined4 *)param_2 = 1;
    lVar3 = param_1 + 7;
    if (*(char *)(param_1 + 4) == 's') {
      *(undefined4 *)param_2 = 2;
      lVar3 = param_1 + 8;
    }
  }
  else {
    *(undefined4 *)param_2 = 0;
  }
  iVar1 = 0;
  lVar4 = lVar3;
  while ((*(char *)lVar4 != '\0') && (*(char *)lVar4 != ':') && (*(char *)lVar4 != '/')) {
    if (iVar1 < 0xff) {
      *(char *)(param_2 + 4 + (long)iVar1) = *(char *)lVar4;
      iVar1 = iVar1 + 1;
    }
    lVar4 = lVar4 + 1;
  }
  *(undefined *)(param_2 + 4 + (long)iVar1) = 0;
  if (*(char *)lVar4 == ':') {
    lVar4 = lVar4 + 1;
    iVar2 = 0;
    while ((*(char *)lVar4 >= '0') && (*(char *)lVar4 <= '9')) {
      iVar2 = iVar2 * 10 + (int)*(char *)lVar4 + -0x30;
      lVar4 = lVar4 + 1;
    }
    *(int *)(param_2 + 0x104) = iVar2;
  }
  else {
    if (*(int *)param_2 == 2) {
      *(int *)(param_2 + 0x104) = 0x1bb;
    }
    else {
      *(int *)(param_2 + 0x104) = 0x50;
    }
  }
  iVar1 = 0;
  if (*(char *)lVar4 == '/') {
    while ((*(char *)lVar4 != '\0') && (iVar1 < 0xff)) {
      *(char *)(param_2 + 0x108 + (long)iVar1) = *(char *)lVar4;
      iVar1 = iVar1 + 1;
      lVar4 = lVar4 + 1;
    }
  }
  if (iVar1 == 0) {
    *(char *)(param_2 + 0x108) = '/';
    *(undefined *)(param_2 + 0x109) = 0;
    goto LAB_00402e30;
  }
  *(undefined *)(param_2 + 0x108 + (long)iVar1) = 0;
LAB_00402e30:
  uVar5 = 0;
LAB_00402e40:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar5;
}
