/* Function: FUN_140001b60 @ 140001b60 */

undefined8 FUN_140001b60(undefined4 *param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  char *pcVar3;
  
  switch(*param_1) {
  default:
    pcVar3 = "Unknown error";
    break;
  case 1:
    pcVar3 = "Argument domain error (DOMAIN)";
    break;
  case 2:
    pcVar3 = "Argument singularity (SIGN)";
    break;
  case 3:
    pcVar3 = "Overflow range error (OVERFLOW)";
    break;
  case 4:
    pcVar3 = "The result is too small to be represented (UNDERFLOW)";
    break;
  case 5:
    pcVar3 = "Total loss of significance (TLOSS)";
    break;
  case 6:
    pcVar3 = "Partial loss of significance (PLOSS)";
  }
  uVar1 = *(undefined8 *)(param_1 + 2);
  uVar2 = __acrt_iob_func(2);
  FUN_140002d90(uVar2,"_matherr(): %s in %s(%g, %g)  (retval=%g)\n",pcVar3,uVar1);
  return 0;
}