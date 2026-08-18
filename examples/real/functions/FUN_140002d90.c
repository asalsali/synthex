/* Function: FUN_140002d90 @ 140002d90 */

void FUN_140002d90(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 *puVar1;
  undefined8 local_res18;
  undefined8 local_res20;
  
  local_res18 = param_3;
  local_res20 = param_4;
  puVar1 = (undefined8 *)FUN_140002df0();
  __stdio_common_vfprintf(*puVar1,param_1,param_2,0,&local_res18);
  return;
}