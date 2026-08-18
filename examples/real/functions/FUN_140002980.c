/* Function: FUN_140002980 @ 140002980 */

/* WARNING: Enum "SectionFlags": Some values do not have unique names */

IMAGE_SECTION_HEADER * FUN_140002980(longlong param_1)

{
  IMAGE_SECTION_HEADER *pIVar1;
  IMAGE_SECTION_HEADER *pIVar2;
  int *piVar3;
  
  if (((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
      (piVar3 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                       *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar3 == 0x4550)) &&
     ((short)piVar3[6] == 0x20b)) {
    pIVar2 = (IMAGE_SECTION_HEADER *)((longlong)piVar3 + (ulonglong)*(ushort *)(piVar3 + 5) + 0x18);
    if (*(ushort *)((longlong)piVar3 + 6) != 0) {
      pIVar1 = pIVar2 + (ulonglong)(*(ushort *)((longlong)piVar3 + 6) - 1) + 1;
      do {
        if (((ulonglong)(uint)pIVar2->VirtualAddress <=
             (ulonglong)(param_1 - (longlong)PTR_IMAGE_DOS_HEADER_140005460)) &&
           ((ulonglong)(param_1 - (longlong)PTR_IMAGE_DOS_HEADER_140005460) <
            (ulonglong)(pIVar2->VirtualAddress + (pIVar2->Misc).PhysicalAddress))) {
          return pIVar2;
        }
        pIVar2 = pIVar2 + 1;
      } while (pIVar2 != pIVar1);
    }
    return (IMAGE_SECTION_HEADER *)0x0;
  }
  return (IMAGE_SECTION_HEADER *)0x0;
}