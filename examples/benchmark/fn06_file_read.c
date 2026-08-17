/* Ghidra decompiled output -- file_utils.bin */

undefined8 FUN_00402010(char *param_1,long param_2,long param_3)

{
  FILE *pFVar1;
  size_t sVar2;
  long lVar3;
  undefined8 uVar4;
  long in_FS_OFFSET;
  long local_10;

  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 == (char *)0x0) {
    uVar4 = 0xffffffff;
    goto LAB_004020f0;
  }
  pFVar1 = fopen(param_1,"rb");
  if (pFVar1 == (FILE *)0x0) {
    uVar4 = 0xfffffffe;
    goto LAB_004020f0;
  }
  fseek(pFVar1,0,2);
  lVar3 = ftell(pFVar1);
  fseek(pFVar1,0,0);
  if (param_3 < lVar3) {
    fclose(pFVar1);
    uVar4 = 0xfffffffd;
    goto LAB_004020f0;
  }
  sVar2 = fread((void *)param_2,1,(long)lVar3,pFVar1);
  if (sVar2 != (size_t)lVar3) {
    fclose(pFVar1);
    uVar4 = 0xfffffffc;
    goto LAB_004020f0;
  }
  *(undefined *)((long)param_2 + lVar3) = 0;
  fclose(pFVar1);
  uVar4 = lVar3;
LAB_004020f0:
  if (*(long *)(in_FS_OFFSET + 0x28) != local_10) {
    __stack_chk_fail();
  }
  return uVar4;
}
