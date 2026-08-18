/* Function: FUN_140002a00 @ 140002a00 */

undefined2 FUN_140002a00(void)

{
  int *piVar1;
  
  if (((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
      (piVar1 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                       *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar1 == 0x4550)) &&
     ((short)piVar1[6] == 0x20b)) {
    return *(undefined2 *)((longlong)piVar1 + 6);
  }
  return 0;
}