/* Function: FUN_1400028f0 @ 1400028f0 */

/* WARNING: Enum "SectionFlags": Some values do not have unique names */

IMAGE_SECTION_HEADER * FUN_1400028f0(char *param_1)

{
  int iVar1;
  size_t sVar2;
  IMAGE_SECTION_HEADER *_Str1;
  int *piVar3;
  uint uVar4;
  
  sVar2 = strlen(param_1);
  if ((((sVar2 < 9) && (*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d)) &&
      (piVar3 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                       *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar3 == 0x4550)) &&
     (((short)piVar3[6] == 0x20b &&
      (_Str1 = (IMAGE_SECTION_HEADER *)
               ((longlong)piVar3 + (ulonglong)*(ushort *)(piVar3 + 5) + 0x18),
      *(short *)((longlong)piVar3 + 6) != 0)))) {
    uVar4 = 0;
    do {
      iVar1 = strncmp(_Str1->Name,param_1,8);
      if (iVar1 == 0) {
        return _Str1;
      }
      uVar4 = uVar4 + 1;
      _Str1 = _Str1 + 1;
    } while (uVar4 < *(ushort *)((longlong)piVar3 + 6));
  }
  return (IMAGE_SECTION_HEADER *)0x0;
}