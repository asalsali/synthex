/* Function: FUN_1400021d0 @ 1400021d0 */

void FUN_1400021d0(undefined4 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 param_5)

{
  undefined4 local_38 [2];
  undefined8 local_30;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined8 local_18;
  
  if (DAT_1400080b0 != (code *)0x0) {
    local_18 = param_5;
    local_38[0] = param_1;
    local_30 = param_2;
    local_28 = param_3;
    uStack_20 = param_4;
    (*DAT_1400080b0)(local_38);
  }
  return;
}