/* Function: FUN_140001c60 @ 140001c60 */

void FUN_140001c60(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  undefined8 local_res10;
  undefined8 local_res18;
  undefined8 local_res20;
  
  local_res10 = param_2;
  local_res18 = param_3;
  local_res20 = param_4;
  uVar1 = __acrt_iob_func(2);
  FUN_140002d90(uVar1,"Mingw-w64 runtime failure:\n",param_3,param_4);
  uVar1 = __acrt_iob_func(2);
  FUN_140002ca0(uVar1,param_1,&local_res10);
                    /* WARNING: Subroutine does not return */
  abort();
}