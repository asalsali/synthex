/* Ghidra decompiled output -- libringbuf.so */

undefined4 FUN_004026c0(long param_1,undefined8 param_2,int param_3)

{
  int iVar1;
  int iVar2;
  long lVar3;
  undefined4 uVar4;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 == 0) {
    uVar4 = 0xffffffff;
    goto LAB_004027c0;
  }
  iVar1 = *(int *)(param_1 + 0x14);
  iVar2 = *(int *)(param_1 + 0x10);
  if (iVar1 == iVar2) {
    if (param_3 == 0) {
      uVar4 = 0xfffffffe;
      goto LAB_004027c0;
    }
    *(int *)(param_1 + 0x10) = (iVar2 + 1) % *(int *)(param_1 + 0x18);
  }
  lVar3 = *(long *)(param_1 + 8);
  *(undefined8 *)(lVar3 + (long)*(int *)(param_1 + 0x14) * 8) = param_2;
  iVar1 = (*(int *)(param_1 + 0x14) + 1) % *(int *)(param_1 + 0x18);
  *(int *)(param_1 + 0x14) = iVar1;
  *(int *)param_1 = *(int *)param_1 + 1;
  if (*(int *)(param_1 + 0x18) < *(int *)param_1) {
    *(int *)param_1 = *(int *)(param_1 + 0x18);
  }
  uVar4 = 0;
  goto LAB_004027c0;
LAB_004027c0:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar4;
}

undefined8 FUN_004027e0(long param_1,long param_2)

{
  int iVar1;
  undefined8 uVar2;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (*(int *)param_1 == 0) {
    uVar2 = 0xffffffff;
    goto LAB_00402880;
  }
  iVar1 = *(int *)(param_1 + 0x10);
  *(undefined8 *)param_2 = *(undefined8 *)(*(long *)(param_1 + 8) + (long)iVar1 * 8);
  *(int *)(param_1 + 0x10) = (iVar1 + 1) % *(int *)(param_1 + 0x18);
  *(int *)param_1 = *(int *)param_1 + -1;
  uVar2 = 0;
LAB_00402880:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar2;
}
