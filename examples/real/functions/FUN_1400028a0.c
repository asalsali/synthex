/* Function: FUN_1400028a0 @ 1400028a0 */

longlong FUN_1400028a0(longlong param_1,ulonglong param_2)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  
  lVar3 = param_1 + *(int *)(param_1 + 0x3c);
  lVar2 = lVar3 + 0x18 + (ulonglong)*(ushort *)(lVar3 + 0x14);
  if (*(ushort *)(lVar3 + 6) != 0) {
    lVar1 = lVar2 + 0x28;
    do {
      if ((*(uint *)(lVar2 + 0xc) <= param_2) &&
         (param_2 < *(uint *)(lVar2 + 0xc) + *(int *)(lVar2 + 8))) {
        return lVar2;
      }
      lVar2 = lVar2 + 0x28;
    } while (lVar2 != lVar1 + (ulonglong)(*(ushort *)(lVar3 + 6) - 1) * 0x28);
  }
  return 0;
}