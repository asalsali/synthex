/* Function: FUN_140002b90 @ 140002b90 */

char * FUN_140002b90(int param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  undefined *puVar3;
  longlong lVar4;
  int *piVar5;
  
  if ((((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
       (piVar5 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                        *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar5 == 0x4550)) &&
      ((short)piVar5[6] == 0x20b)) && (uVar2 = (ulonglong)(uint)piVar5[0x24], piVar5[0x24] != 0)) {
    lVar4 = (longlong)piVar5 + (ulonglong)*(ushort *)(piVar5 + 5) + 0x18;
    if (*(ushort *)((longlong)piVar5 + 6) != 0) {
      lVar1 = lVar4 + 0x28;
      while ((uVar2 < *(uint *)(lVar4 + 0xc) ||
             (*(uint *)(lVar4 + 0xc) + *(int *)(lVar4 + 8) <= uVar2))) {
        lVar4 = lVar4 + 0x28;
        if (lVar1 + (ulonglong)(*(ushort *)((longlong)piVar5 + 6) - 1) * 0x28 == lVar4) {
          return (char *)0x0;
        }
      }
      for (puVar3 = PTR_IMAGE_DOS_HEADER_140005460 + uVar2;
          (*(int *)(puVar3 + 4) != 0 || (*(int *)(puVar3 + 0xc) != 0)); puVar3 = puVar3 + 0x14) {
        if (param_1 < 1) {
          return PTR_IMAGE_DOS_HEADER_140005460 + *(uint *)(puVar3 + 0xc);
        }
        param_1 = param_1 + -1;
      }
      return (char *)0x0;
    }
  }
  return (char *)0x0;
}