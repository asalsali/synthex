/* Ghidra decompiled output -- libthreadpool.so */

undefined4 FUN_00403480(long param_1,long param_2,undefined8 param_3)

{
  void *pvVar1;
  int iVar2;
  long lVar3;
  undefined4 uVar4;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 == 0) {
    uVar4 = 0xffffffff;
    goto LAB_004035e0;
  }
  if (*(int *)(param_1 + 0x40) != 0) {
    uVar4 = 0xfffffffe;
    goto LAB_004035e0;
  }
  pvVar1 = malloc(0x20);
  if (pvVar1 == (void *)0x0) {
    uVar4 = 0xfffffffd;
    goto LAB_004035e0;
  }
  *(long *)pvVar1 = param_2;
  *(undefined8 *)((long)pvVar1 + 8) = param_3;
  *(undefined8 *)((long)pvVar1 + 0x10) = 0;
  *(undefined4 *)((long)pvVar1 + 0x18) = 0;
  iVar2 = pthread_mutex_lock((pthread_mutex_t *)param_1);
  if (iVar2 != 0) {
    free(pvVar1);
    uVar4 = 0xfffffffc;
    goto LAB_004035e0;
  }
  if (*(int *)(param_1 + 0x38) >= *(int *)(param_1 + 0x3c)) {
    pthread_mutex_unlock((pthread_mutex_t *)param_1);
    free(pvVar1);
    uVar4 = 0xfffffffb;
    goto LAB_004035e0;
  }
  lVar3 = *(long *)(param_1 + 0x30);
  if (lVar3 == 0) {
    *(void **)(param_1 + 0x28) = pvVar1;
    *(void **)(param_1 + 0x30) = pvVar1;
  }
  else {
    *(void **)(lVar3 + 0x10) = pvVar1;
    *(void **)(param_1 + 0x30) = pvVar1;
  }
  *(int *)(param_1 + 0x38) = *(int *)(param_1 + 0x38) + 1;
  pthread_cond_signal((pthread_cond_t *)(param_1 + 0x48));
  pthread_mutex_unlock((pthread_mutex_t *)param_1);
  uVar4 = 0;
LAB_004035e0:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar4;
}
