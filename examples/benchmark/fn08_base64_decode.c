/* Ghidra decompiled output -- libcrypto_util.so */

undefined8 FUN_004022a0(long param_1,long param_2,long param_3)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined8 uVar6;
  long lVar7;
  long in_FS_OFFSET;
  undefined local_118 [256];
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  memset(local_118,0xff,0x100);
  lVar7 = 0;
  while (lVar7 < 0x1a) {
    local_118[lVar7 + 0x41] = (undefined)(byte)lVar7;
    local_118[lVar7 + 0x61] = (undefined)(byte)(lVar7 + 0x1a);
    lVar7 = lVar7 + 1;
  }
  lVar7 = 0;
  while (lVar7 < 10) {
    local_118[lVar7 + 0x30] = (undefined)(byte)(lVar7 + 0x34);
    lVar7 = lVar7 + 1;
  }
  local_118[0x2b] = 0x3e;
  local_118[0x2f] = 0x3f;
  lVar7 = 0;
  iVar2 = 0;
  while (*(char *)(param_1 + lVar7) != '\0') {
    bVar1 = *(byte *)(param_1 + lVar7);
    if (bVar1 == 0x3d) {
      goto LAB_004023c0;
    }
    iVar3 = (int)local_118[(int)bVar1];
    if (iVar3 == 0xff) {
      uVar6 = 0xffffffff;
      goto LAB_00402400;
    }
    iVar4 = (int)lVar7 & 3;
    if (iVar4 == 0) {
      iVar5 = iVar3 << 2;
    }
    else if (iVar4 == 1) {
      *(byte *)(param_2 + (long)iVar2) = (byte)(iVar5 | iVar3 >> 4);
      iVar5 = (iVar3 & 0xf) << 4;
      iVar2 = iVar2 + 1;
    }
    else if (iVar4 == 2) {
      *(byte *)(param_2 + (long)iVar2) = (byte)(iVar5 | iVar3 >> 2);
      iVar5 = (iVar3 & 3) << 6;
      iVar2 = iVar2 + 1;
    }
    else {
      *(byte *)(param_2 + (long)iVar2) = (byte)(iVar5 | iVar3);
      iVar2 = iVar2 + 1;
    }
    lVar7 = lVar7 + 1;
  }
LAB_004023c0:
  *(long *)param_3 = (long)iVar2;
  uVar6 = 0;
LAB_00402400:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar6;
}
