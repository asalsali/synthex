/* Ghidra decompiled output -- libhtable.so */

undefined8 FUN_00401d80(long param_1,long param_2)

{
  ulong uVar1;
  byte bVar2;
  long lVar3;
  int iVar4;
  undefined8 uVar5;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 == 0) {
    uVar5 = 0;
    goto LAB_00401ea0;
  }
  uVar1 = 0x1505;
  lVar3 = param_2;
  while (*(char *)lVar3 != '\0') {
    uVar1 = (ulong)(byte)*(char *)lVar3 + uVar1 * 0x21;
    lVar3 = lVar3 + 1;
  }
  uVar1 = uVar1 & *(ulong *)(param_1 + 0x10);
  lVar3 = *(long *)(*(long *)(param_1 + 8) + uVar1 * 8);
  if (lVar3 == 0) {
    uVar5 = 0;
    goto LAB_00401ea0;
  }
  while (lVar3 != 0) {
    iVar4 = 0;
    while( true ) {
      bVar2 = *(byte *)(*(long *)lVar3 + (long)iVar4);
      if (bVar2 != *(byte *)(param_2 + (long)iVar4)) {
        goto LAB_00401e50;
      }
      if (bVar2 == 0) {
        uVar5 = *(undefined8 *)(lVar3 + 8);
        goto LAB_00401ea0;
      }
      iVar4 = iVar4 + 1;
    }
LAB_00401e50:
    lVar3 = *(long *)(lVar3 + 0x10);
  }
  uVar5 = 0;
LAB_00401ea0:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar5;
}
