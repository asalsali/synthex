/* Ghidra decompiled output -- libprotocol.so */

undefined4 FUN_004031e0(long param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  undefined4 uVar6;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 == 0) {
    uVar6 = 0;
    goto LAB_004032e0;
  }
  uVar1 = *(uint *)param_1;
  uVar2 = *(uint *)(param_1 + 4);
  uVar3 = *(uint *)(param_1 + 8);
  uVar4 = *(uint *)(param_1 + 0xc);
  uVar5 = *(uint *)(param_1 + 0x10);
  if (0xf < uVar1) {
    uVar1 = uVar1 & 0xf;
  }
  if (0x3f < uVar2) {
    uVar2 = uVar2 & 0x3f;
  }
  if (0x1f < uVar3) {
    uVar3 = uVar3 & 0x1f;
  }
  if (0xff < uVar4) {
    uVar4 = uVar4 & 0xff;
  }
  if (0x1ff < uVar5) {
    uVar5 = uVar5 & 0x1ff;
    goto LAB_004032c0;
  }
  uVar6 = uVar1 << 0x1c | uVar2 << 0x16 | uVar3 << 0x11 | uVar4 << 9 | uVar5;
  goto LAB_004032e0;
LAB_004032c0:
  uVar6 = uVar1 << 0x1c | uVar2 << 0x16 | uVar3 << 0x11 | uVar4 << 9 | uVar5;
  *(undefined4 *)(param_1 + 0x14) = 1;
LAB_004032e0:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar6;
}
