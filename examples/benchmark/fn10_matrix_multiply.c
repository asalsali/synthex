/* Ghidra decompiled output -- libmath.so */

void FUN_00402580(long param_1,long param_2,long param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  iVar1 = 0;
  while (iVar1 < 3) {
    iVar2 = 0;
    while (iVar2 < 3) {
      iVar4 = 0;
      *(undefined4 *)(param_3 + (long)(iVar1 * 3 + iVar2) * 4) = 0;
      iVar3 = 0;
      while (iVar3 < 3) {
        iVar4 = iVar4 + *(int *)(param_1 + (long)(iVar1 * 3 + iVar3) * 4) *
                         *(int *)(param_2 + (long)(iVar3 * 3 + iVar2) * 4);
        iVar3 = iVar3 + 1;
      }
      *(int *)(param_3 + (long)(iVar1 * 3 + iVar2) * 4) = iVar4;
      if (iVar4 < -0x7fffffff) {
        goto LAB_00402660;
      }
      iVar2 = iVar2 + 1;
    }
    iVar1 = iVar1 + 1;
  }
  goto LAB_00402680;
LAB_00402660:
  *(undefined4 *)(param_3 + 0x24) = 0x7fffffff;
LAB_00402680:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return;
}
