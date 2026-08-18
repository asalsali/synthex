/* Function: FUN_140001940 @ 140001940 */

void FUN_140001940(undefined8 *param_1)

{
  void *pvVar1;
  void *_Memory;
  undefined8 *puVar2;
  
  if (param_1 != (undefined8 *)0x0) {
    puVar2 = param_1;
    do {
      _Memory = (void *)*puVar2;
      while (_Memory != (void *)0x0) {
        pvVar1 = *(void **)((longlong)_Memory + 0x280);
        free(_Memory);
        _Memory = pvVar1;
      }
      puVar2 = puVar2 + 1;
    } while (param_1 + 0x40 != puVar2);
    free(param_1);
    return;
  }
  return;
}