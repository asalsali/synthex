/* Ghidra decompiled output -- netclient.bin */

undefined4 FUN_00402140(char *param_1,undefined4 param_2,long param_3,int param_4)

{
  int iVar1;
  int iVar2;
  ssize_t sVar3;
  undefined4 uVar4;
  long in_FS_OFFSET;
  undefined local_38 [2];
  undefined4 local_36;
  undefined4 local_32;
  undefined local_2e [16];
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  iVar1 = socket(2,1,0);
  if (iVar1 == -1) {
    uVar4 = 0xffffffff;
    goto LAB_00402260;
  }
  memset(local_38,0,0x10);
  local_38[0] = 0;
  local_38[1] = 2;
  local_36 = htons((ushort)param_2);
  iVar2 = inet_pton(2,param_1,local_2e);
  if (iVar2 < 1) {
    close(iVar1);
    uVar4 = 0xfffffffe;
    goto LAB_00402260;
  }
  local_32 = *(undefined4 *)local_2e;
  iVar2 = connect(iVar1,(struct sockaddr *)local_38,0x10);
  if (iVar2 == -1) {
    close(iVar1);
    uVar4 = 0xfffffffd;
    goto LAB_00402260;
  }
  sVar3 = send(iVar1,(void *)param_3,(long)param_4,0);
  if ((int)sVar3 < param_4) {
    close(iVar1);
    uVar4 = 0xfffffffc;
    goto LAB_00402260;
  }
  uVar4 = iVar1;
LAB_00402260:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar4;
}
