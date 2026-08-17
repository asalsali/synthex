/* Ghidra decompiled output -- libc_custom.so */

undefined8 FUN_00401c40(long param_1,long param_2,undefined4 param_3)

{
  byte bVar1;
  int iVar2;
  undefined8 uVar3;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 == 0) {
    uVar3 = 0xffffffff;
    goto LAB_00401d20;
  }
  if (param_2 == 0) {
    uVar3 = 0xfffffffe;
    goto LAB_00401d20;
  }
  iVar2 = 0;
  while( true ) {
    if ((int)param_3 + -1 <= iVar2) {
      *(undefined *)((long)iVar2 + param_1) = 0;
      uVar3 = 0;
      goto LAB_00401d20;
    }
    bVar1 = *(byte *)(param_2 + (long)iVar2);
    if (bVar1 == 0) break;
    *(byte *)(param_1 + (long)iVar2) = bVar1;
    iVar2 = iVar2 + 1;
  }
  *(undefined *)(param_1 + (long)iVar2) = 0;
  if (iVar2 == 0) {
    goto LAB_00401d10;
  }
  uVar3 = (undefined8)(long)iVar2;
  goto LAB_00401d20;
LAB_00401d10:
  uVar3 = 0;
LAB_00401d20:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar3;
}
