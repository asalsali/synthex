/* Function: FUN_140002d30 @ 140002d30 */

void FUN_140002d30(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined8 local_res10;
  undefined8 local_res18;
  undefined8 local_res20;
  
  local_res10 = param_2;
  local_res18 = param_3;
  local_res20 = param_4;
  uVar1 = __acrt_iob_func(1);
  puVar2 = (undefined8 *)FUN_140002df0();
  __stdio_common_vfprintf(*puVar2,uVar1,param_1,0,&local_res10);
  return;
}