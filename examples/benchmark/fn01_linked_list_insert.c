/* Ghidra decompiled output -- libcollections.so */

undefined8 FUN_00401a30(long param_1,undefined8 param_2)

{
  long lVar1;
  void *pvVar2;
  long lVar3;
  undefined8 uVar4;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  pvVar2 = malloc(0x18);
  if (pvVar2 == (void *)0x0) {
    uVar4 = 0xffffffff;
    goto LAB_00401af0;
  }
  *(undefined8 *)pvVar2 = param_2;
  *(undefined8 *)((long)pvVar2 + 8) = 0;
  *(undefined8 *)((long)pvVar2 + 0x10) = 0;
  lVar1 = *(long *)param_1;
  if (lVar1 == 0) {
    *(void **)param_1 = pvVar2;
    *(long *)(param_1 + 8) = *(long *)(param_1 + 8) + 1;
    uVar4 = 0;
    goto LAB_00401af0;
  }
  lVar3 = lVar1;
  while (*(long *)(lVar3 + 8) != 0) {
    lVar3 = *(long *)(lVar3 + 8);
  }
  *(void **)(lVar3 + 8) = pvVar2;
  *(long *)((long)pvVar2 + 0x10) = lVar3;
  *(long *)(param_1 + 8) = *(long *)(param_1 + 8) + 1;
  if (*(long *)(param_1 + 0x10) != 0) {
    if (*(long *)(param_1 + 8) > *(long *)(param_1 + 0x10)) {
      goto LAB_00401ac0;
    }
  }
  uVar4 = 0;
  goto LAB_00401af0;
LAB_00401ac0:
  *(undefined4 *)(param_1 + 0x20) = 1;
  uVar4 = 0;
LAB_00401af0:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar4;
}
