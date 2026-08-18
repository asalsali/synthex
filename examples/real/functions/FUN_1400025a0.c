/* Function: FUN_1400025a0 @ 1400025a0 */

void FUN_1400025a0(void)

{
  DWORD *pDVar1;
  DWORD DVar2;
  LPVOID pvVar3;
  
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
  for (pDVar1 = DAT_1400080e0; pDVar1 != (DWORD *)0x0; pDVar1 = *(DWORD **)(pDVar1 + 4)) {
    pvVar3 = TlsGetValue(*pDVar1);
    DVar2 = GetLastError();
    if ((pvVar3 != (LPVOID)0x0) && (DVar2 == 0)) {
      (**(code **)(pDVar1 + 2))(pvVar3);
    }
  }
                    /* WARNING: Could not recover jumptable at 0x000140002609. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
  return;
}