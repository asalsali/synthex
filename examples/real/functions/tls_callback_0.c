/* Function: tls_callback_0 @ 140001ad0 */

/* WARNING: Removing unreachable block (ram,0x000140001b13) */
/* WARNING: Removing unreachable block (ram,0x000140001b18) */
/* WARNING: Removing unreachable block (ram,0x000140001b20) */
/* WARNING: Removing unreachable block (ram,0x000140001b22) */
/* WARNING: Removing unreachable block (ram,0x000140001b2b) */

void tls_callback_0(undefined8 param_1,int param_2)

{
  if (*(int *)PTR_DAT_140005430 != 2) {
    *(undefined4 *)PTR_DAT_140005430 = 2;
  }
  if ((param_2 != 2) && (param_2 == 1)) {
    FUN_140002730(param_1,1);
    return;
  }
  return;
}