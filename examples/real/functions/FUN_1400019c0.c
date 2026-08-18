/* Function: FUN_1400019c0 @ 1400019c0 */

void FUN_1400019c0(void)

{
  code *pcVar1;
  
  pcVar1 = *(code **)PTR_DAT_140004000;
  while (pcVar1 != (code *)0x0) {
    (*pcVar1)();
    pcVar1 = *(code **)(PTR_DAT_140004000 + 8);
    PTR_DAT_140004000 = PTR_DAT_140004000 + 8;
  }
  return;
}