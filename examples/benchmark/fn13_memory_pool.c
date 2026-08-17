/* Ghidra decompiled output -- libpool.so */

void * FUN_004029e0(long param_1)

{
  long lVar1;
  void *pvVar2;
  long lVar3;
  void *pvVar4;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 == 0) {
    pvVar4 = (void *)0x0;
    goto LAB_00402b20;
  }
  lVar1 = *(long *)(param_1 + 0x18);
  if (lVar1 != 0) {
    *(long *)(param_1 + 0x18) = *(long *)lVar1;
    *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + -1;
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 1;
    memset((void *)lVar1,0,*(long *)(param_1 + 8));
    pvVar4 = (void *)lVar1;
    goto LAB_00402b20;
  }
  if (*(int *)(param_1 + 0x28) != 0) {
    pvVar4 = (void *)0x0;
    goto LAB_00402b20;
  }
  lVar3 = *(long *)(param_1 + 8);
  pvVar2 = malloc((long)*(int *)(param_1 + 0x10) * lVar3);
  if (pvVar2 == (void *)0x0) {
    pvVar4 = (void *)0x0;
    goto LAB_00402b20;
  }
  iVar5 = 1;
  while (iVar5 < *(int *)(param_1 + 0x10)) {
    lVar1 = (long)pvVar2 + (long)iVar5 * lVar3;
    *(long *)lVar1 = *(long *)(param_1 + 0x18);
    *(long *)(param_1 + 0x18) = lVar1;
    *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + 1;
    iVar5 = iVar5 + 1;
  }
  *(int *)(param_1 + 0x2c) = *(int *)(param_1 + 0x2c) + 1;
  *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 1;
  memset(pvVar2,0,lVar3);
  pvVar4 = pvVar2;
LAB_00402b20:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return pvVar4;
}
