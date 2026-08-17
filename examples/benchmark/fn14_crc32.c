/* Ghidra decompiled output -- libchecksum.so */

undefined4 FUN_00402b60(long param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  long in_FS_OFFSET;
  uint local_428 [256];
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  iVar2 = 0;
  while (iVar2 < 0x100) {
    uVar4 = (uint)iVar2;
    iVar3 = 0;
    while (iVar3 < 8) {
      if ((uVar4 & 1) == 0) {
        uVar4 = uVar4 >> 1;
      }
      else {
        uVar4 = uVar4 >> 1 ^ 0xedb88320;
      }
      iVar3 = iVar3 + 1;
    }
    local_428[iVar2] = uVar4;
    iVar2 = iVar2 + 1;
  }
  uVar1 = 0xffffffff;
  iVar2 = 0;
  while (iVar2 < param_2) {
    uVar1 = local_428[(uVar1 ^ (uint)*(byte *)(param_1 + (long)iVar2)) & 0xff] ^ uVar1 >> 8;
    iVar2 = iVar2 + 1;
  }
  uVar1 = uVar1 ^ 0xffffffff;
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    goto LAB_00402c80;
  }
  return uVar1;
LAB_00402c80:
  __stack_chk_fail();
}
