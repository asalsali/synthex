/* Function: FUN_140001a00 @ 140001a00 */

void FUN_140001a00(void)

{
  undefined8 *puVar1;
  uint uVar2;
  ulonglong uVar3;
  undefined8 *puVar4;
  
  uVar2 = (uint)*(undefined8 *)PTR_DAT_140005450;
  if (uVar2 == 0xffffffff) {
    uVar3 = 0;
    do {
      uVar2 = (uint)uVar3;
      uVar3 = (ulonglong)(uVar2 + 1);
    } while (*(longlong *)(PTR_DAT_140005450 + uVar3 * 8) != 0);
  }
  if (uVar2 != 0) {
    puVar4 = (undefined8 *)(PTR_DAT_140005450 + (ulonglong)uVar2 * 8);
    puVar1 = (undefined8 *)
             (PTR_DAT_140005450 + ((ulonglong)uVar2 - (ulonglong)(uVar2 - 1)) * 8 + -8);
    do {
      (*(code *)*puVar4)();
      puVar4 = puVar4 + -1;
    } while (puVar4 != puVar1);
  }
  _crt_atexit(FUN_1400019c0);
  return;
}