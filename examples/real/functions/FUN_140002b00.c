/* Function: FUN_140002b00 @ 140002b00 */

uint FUN_140002b00(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  int *piVar3;
  
  if (((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
      (piVar3 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                       *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar3 == 0x4550)) &&
     ((short)piVar3[6] == 0x20b)) {
    lVar2 = (longlong)piVar3 + (ulonglong)*(ushort *)(piVar3 + 5) + 0x18;
    if (*(ushort *)((longlong)piVar3 + 6) != 0) {
      lVar1 = lVar2 + 0x28;
      while (((ulonglong)(param_1 - (longlong)PTR_IMAGE_DOS_HEADER_140005460) <
              (ulonglong)*(uint *)(lVar2 + 0xc) ||
             ((ulonglong)(*(uint *)(lVar2 + 0xc) + *(int *)(lVar2 + 8)) <=
              (ulonglong)(param_1 - (longlong)PTR_IMAGE_DOS_HEADER_140005460)))) {
        lVar2 = lVar2 + 0x28;
        if (lVar1 + (ulonglong)(*(ushort *)((longlong)piVar3 + 6) - 1) * 0x28 == lVar2) {
          return 0;
        }
      }
      return ~*(uint *)(lVar2 + 0x24) >> 0x1f;
    }
  }
  return 0;
}