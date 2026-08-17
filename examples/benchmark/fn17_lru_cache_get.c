/* Ghidra decompiled output -- libcache.so */

undefined8 FUN_00403060(long param_1,undefined8 param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  int iVar5;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 == 0) {
    uVar4 = 0;
    goto LAB_004031a0;
  }
  iVar5 = FUN_00403200(param_2);
  iVar5 = iVar5 & *(int *)(param_1 + 0x10);
  lVar1 = *(long *)(*(long *)(param_1 + 8) + (long)iVar5 * 8);
  if (lVar1 == 0) {
    uVar4 = 0;
    goto LAB_004031a0;
  }
  while (lVar1 != 0) {
    if (*(undefined8 *)lVar1 == param_2) {
      goto LAB_00403100;
    }
    lVar1 = *(long *)(lVar1 + 0x20);
  }
  uVar4 = 0;
  goto LAB_004031a0;
LAB_00403100:
  *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + 1;
  lVar2 = *(long *)(param_1 + 0x28);
  if (lVar1 == lVar2) {
    uVar4 = *(undefined8 *)(lVar1 + 8);
    goto LAB_004031a0;
  }
  lVar3 = *(long *)(lVar1 + 0x18);
  lVar2 = *(long *)(lVar1 + 0x10);
  if (lVar3 != 0) {
    *(long *)(lVar3 + 0x10) = lVar2;
  }
  if (lVar2 != 0) {
    *(long *)(lVar2 + 0x18) = lVar3;
  }
  if (*(long *)(param_1 + 0x30) == lVar1) {
    *(long *)(param_1 + 0x30) = lVar3;
  }
  lVar2 = *(long *)(param_1 + 0x28);
  *(long *)(lVar1 + 0x10) = 0;
  *(long *)(lVar1 + 0x18) = lVar2;
  if (lVar2 != 0) {
    *(long *)(lVar2 + 0x10) = lVar1;
  }
  *(long *)(param_1 + 0x28) = lVar1;
  uVar4 = *(undefined8 *)(lVar1 + 8);
LAB_004031a0:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar4;
}
