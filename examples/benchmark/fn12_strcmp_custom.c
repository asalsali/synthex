/* Ghidra decompiled output -- libc_custom.so */

int FUN_004028c0(long param_1,long param_2)

{
  byte bVar1;
  byte bVar2;
  int iVar3;
  long lVar4;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 == param_2) {
    iVar3 = 0;
    goto LAB_00402990;
  }
  if (param_1 == 0) {
    iVar3 = -1;
    goto LAB_00402990;
  }
  if (param_2 == 0) {
    iVar3 = 1;
    goto LAB_00402990;
  }
  lVar4 = 0;
  while( true ) {
    bVar1 = *(byte *)(param_1 + lVar4);
    bVar2 = *(byte *)(param_2 + lVar4);
    if (bVar1 != bVar2) break;
    if (bVar1 == 0) {
      iVar3 = 0;
      goto LAB_00402990;
    }
    lVar4 = lVar4 + 1;
    if (lVar4 == 0x1000) {
      iVar3 = 0;
      goto LAB_00402990;
    }
  }
  if (bVar1 < bVar2) {
    iVar3 = -1;
  }
  else {
    iVar3 = 1;
  }
LAB_00402990:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return iVar3;
}
