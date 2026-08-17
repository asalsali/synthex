/* Ghidra decompiled output -- libtree.so */

void FUN_00402440(long param_1,long param_2,long *param_3)

{
  long lVar1;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 == 0) {
    goto LAB_00402530;
  }
  lVar1 = *(long *)(param_1 + 8);
  if (lVar1 != 0) {
    FUN_00402440(lVar1,param_2,param_3);
  }
  if (*param_3 < *(long *)(param_2 + 0x10)) {
    *(undefined8 *)(*(long *)param_2 + *param_3 * 8) = *(undefined8 *)param_1;
    *param_3 = *param_3 + 1;
  }
  else {
    *(undefined4 *)(param_2 + 0x18) = 1;
    goto LAB_00402530;
  }
  lVar1 = *(long *)(param_1 + 0x10);
  if (lVar1 == 0) {
    goto LAB_00402530;
  }
  if (*(int *)(param_2 + 0x18) != 0) {
    goto LAB_00402530;
  }
  FUN_00402440(lVar1,param_2,param_3);
LAB_00402530:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return;
}
