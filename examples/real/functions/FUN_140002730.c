/* Function: FUN_140002730 @ 140002730 */

undefined8 FUN_140002730(undefined8 param_1,uint param_2)

{
  void *pvVar1;
  void *_Memory;
  
  if (param_2 == 2) {
    FUN_140002860();
    return 1;
  }
  if (param_2 < 3) {
    if (param_2 == 0) {
      if (DAT_1400080e8 != 0) {
        FUN_1400025a0();
      }
      if (DAT_1400080e8 == 1) {
        DAT_1400080e8 = 1;
        _Memory = DAT_1400080e0;
        while (_Memory != (void *)0x0) {
          pvVar1 = *(void **)((longlong)_Memory + 0x10);
          free(_Memory);
          _Memory = pvVar1;
        }
        DAT_1400080e0 = (void *)0x0;
        DAT_1400080e8 = 0;
        DeleteCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
      }
    }
    else {
      if (DAT_1400080e8 == 0) {
        InitializeCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
      }
      DAT_1400080e8 = 1;
    }
  }
  else if ((param_2 == 3) && (DAT_1400080e8 != 0)) {
    FUN_1400025a0();
  }
  return 1;
}