/* Function: FUN_140002e50 @ 140002e50 */

undefined8
FUN_140002e50(undefined4 *param_1,undefined8 *param_2,undefined8 *param_3,int param_4,
             undefined4 *param_5)

{
  undefined4 *puVar1;
  undefined8 *puVar2;
  
  _initialize_narrow_environment();
  _configure_narrow_argv(2 - (uint)(param_4 == 0));
  puVar1 = (undefined4 *)__p___argc();
  *param_1 = *puVar1;
  puVar2 = (undefined8 *)__p___argv();
  *param_2 = *puVar2;
  puVar2 = (undefined8 *)__p__environ();
  *param_3 = *puVar2;
  _set_new_mode(*param_5);
  return 0;
}