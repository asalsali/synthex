/* Function: FUN_140002610 @ 140002610 */

undefined8 FUN_140002610(undefined4 param_1,undefined8 param_2)

{
  undefined4 *puVar1;
  
  if (DAT_1400080e8 != 0) {
    puVar1 = calloc(1,0x18);
    if (puVar1 == (undefined4 *)0x0) {
      return 0xffffffff;
    }
    *(undefined8 *)(puVar1 + 2) = param_2;
    *puVar1 = param_1;
    EnterCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
    *(undefined4 **)(puVar1 + 4) = DAT_1400080e0;
    DAT_1400080e0 = puVar1;
    LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
  }
  return 0;
}