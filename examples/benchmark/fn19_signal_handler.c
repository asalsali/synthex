/* Ghidra decompiled output -- daemon.bin */

undefined4 FUN_00403320(long param_1,undefined8 param_2)

{
  int iVar1;
  undefined4 uVar2;
  long in_FS_OFFSET;
  undefined local_a8 [4];
  undefined4 local_a4;
  undefined local_a0 [128];
  undefined8 local_20;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  memset(local_a8,0,0x98);
  if (param_1 == 0) {
    uVar2 = 0xffffffff;
    goto LAB_00403440;
  }
  *(long *)(local_a8 + 8) = param_1;
  local_a4 = 0x14000000;
  sigemptyset((sigset_t *)local_a0);
  local_20 = 0;
  iVar1 = sigaction(2,(struct sigaction *)local_a8,(struct sigaction *)0x0);
  if (iVar1 != 0) {
    uVar2 = 0xfffffffe;
    goto LAB_00403440;
  }
  iVar1 = sigaction(0xf,(struct sigaction *)local_a8,(struct sigaction *)0x0);
  if (iVar1 != 0) {
    uVar2 = 0xfffffffd;
    goto LAB_00403440;
  }
  iVar1 = sigaction(1,(struct sigaction *)local_a8,(struct sigaction *)0x0);
  if (iVar1 != 0) {
    uVar2 = 0xfffffffc;
    goto LAB_00403440;
  }
  if (param_2 != 0) {
    iVar1 = sigaction(0xa,(struct sigaction *)local_a8,(struct sigaction *)0x0);
    if (iVar1 != 0) {
      uVar2 = 0xfffffffb;
      goto LAB_00403440;
    }
  }
  *(undefined4 *)(local_a8 + 8) = 0;
  *(long *)(local_a8 + 0x10) = 1;
  iVar1 = sigaction(0xd,(struct sigaction *)local_a8,(struct sigaction *)0x0);
  if (iVar1 != 0) {
    uVar2 = 0xfffffffa;
    goto LAB_00403440;
  }
  uVar2 = 0;
LAB_00403440:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar2;
}
