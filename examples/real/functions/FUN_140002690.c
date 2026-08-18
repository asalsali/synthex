/* Function: FUN_140002690 @ 140002690 */

undefined8 FUN_140002690(int param_1)

{
  int *piVar1;
  int *_Memory;
  int *piVar2;
  
  if (DAT_1400080e8 == 0) {
    return 0;
  }
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
  if (DAT_1400080e0 != (int *)0x0) {
    piVar1 = DAT_1400080e0;
    piVar2 = (int *)0x0;
    do {
      _Memory = piVar1;
      piVar1 = *(int **)(_Memory + 4);
      if (*_Memory == param_1) {
        if (piVar2 != (int *)0x0) {
          *(int **)(piVar2 + 4) = piVar1;
          piVar1 = DAT_1400080e0;
        }
        DAT_1400080e0 = piVar1;
        free(_Memory);
        break;
      }
      piVar2 = _Memory;
    } while (piVar1 != (int *)0x0);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
  return 0;
}