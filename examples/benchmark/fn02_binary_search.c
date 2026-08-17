/* Ghidra decompiled output -- libsearch.so */

undefined4 FUN_00401b10(long param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_2 < 1) {
    uVar4 = 0xffffffff;
    goto LAB_00401bc0;
  }
  iVar1 = 0;
  iVar2 = param_2 + -1;
  while (iVar1 <= iVar2) {
    iVar3 = (iVar1 + iVar2) / 2;
    if (*(int *)(param_1 + (long)iVar3 * 4) == param_3) {
      uVar4 = iVar3;
      goto LAB_00401bc0;
    }
    if (*(int *)(param_1 + (long)iVar3 * 4) < param_3) {
      iVar1 = iVar3 + 1;
    }
    else {
      iVar2 = iVar3 + -1;
    }
  }
  uVar4 = 0xffffffff;
  goto LAB_00401bc0;
LAB_00401bc0:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar4;
}
