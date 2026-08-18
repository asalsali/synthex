/* Function: FUN_140002ac0 @ 140002ac0 */

IMAGE_DOS_HEADER * FUN_140002ac0(void)

{
  IMAGE_DOS_HEADER *pIVar1;
  
  if ((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
     (*(int *)(PTR_IMAGE_DOS_HEADER_140005460 + *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)) ==
      0x4550)) {
    pIVar1 = (IMAGE_DOS_HEADER *)0x0;
    if ((short)*(int *)((longlong)
                        (PTR_IMAGE_DOS_HEADER_140005460 +
                        *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)) + 0x18) == 0x20b) {
      pIVar1 = (IMAGE_DOS_HEADER *)PTR_IMAGE_DOS_HEADER_140005460;
    }
    return pIVar1;
  }
  return (IMAGE_DOS_HEADER *)0x0;
}