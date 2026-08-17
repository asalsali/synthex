/* Ghidra decompiled output -- libsort.so */

void FUN_00401f00(long param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_2 < 2) {
    goto LAB_00401fe0;
  }
  iVar5 = param_2 + -1;
  while (0 < iVar5) {
    iVar1 = 0;
    iVar2 = 0;
    while (iVar2 < iVar5) {
      iVar3 = *(int *)(param_1 + (long)iVar2 * 4);
      if (*(int *)(param_1 + (long)(iVar2 + 1) * 4) < iVar3) {
        uVar4 = *(undefined4 *)(param_1 + (long)(iVar2 + 1) * 4);
        *(int *)(param_1 + (long)(iVar2 + 1) * 4) = iVar3;
        *(undefined4 *)(param_1 + (long)iVar2 * 4) = uVar4;
        iVar1 = 1;
      }
      iVar2 = iVar2 + 1;
    }
    if (iVar1 == 0) {
      goto LAB_00401fe0;
    }
    iVar5 = iVar5 + -1;
  }
LAB_00401fe0:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return;
}
