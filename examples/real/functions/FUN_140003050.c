/* Function: FUN_140003050 @ 140003050 */

undefined8 FUN_140003050(int param_1,longlong param_2,char *param_3,undefined8 param_4)

{
  void *pvVar1;
  void *_Memory;
  int iVar2;
  longlong *_Memory_00;
  undefined8 uVar3;
  char *_Str;
  FILE *_File;
  int *piVar4;
  longlong *plVar5;
  int local_2b8;
  char local_2b4;
  int local_2a8 [32];
  char local_228 [520];
  
  FUN_140001a80();
  piVar4 = (int *)0x208;
  _Memory_00 = calloc(1,0x208);
  if (_Memory_00 == (longlong *)0x0) {
    _File = (FILE *)__acrt_iob_func(2);
    fwrite("Failed to create store\n",1,0x17,_File);
    uVar3 = 1;
  }
  else {
    if (1 < param_1) {
      piVar4 = *(int **)(param_2 + 8);
      FUN_140001880((longlong)_Memory_00,(char *)piVar4);
    }
LAB_140003098:
    while( true ) {
      FUN_140002d30("kvstore> ",piVar4,param_3,param_4);
      piVar4 = &local_2b8;
      iVar2 = FUN_140002ce0(&DAT_1400050b1,piVar4,param_3,param_4);
      if (iVar2 != 1) break;
      if (local_2b8 == 0x746573) goto LAB_14000310a;
      if (local_2b8 == 0x746567) {
        piVar4 = local_2a8;
        iVar2 = FUN_140002ce0("%127s",piVar4,param_3,param_4);
        if (iVar2 == 1) {
          piVar4 = local_2a8;
          _Str = FUN_140001640((longlong)_Memory_00,(char *)piVar4);
          if (_Str == (char *)0x0) {
LAB_140003201:
            puts("(nil)");
          }
          else {
            puts(_Str);
          }
        }
      }
      else if (local_2b8 == 0x6c6564) {
        piVar4 = local_2a8;
        iVar2 = FUN_140002ce0("%127s",piVar4,param_3,param_4);
        if (iVar2 == 1) {
          piVar4 = local_2a8;
          iVar2 = FUN_140001700((longlong)_Memory_00,(char *)piVar4);
          if (iVar2 != 0) goto LAB_140003201;
          goto LAB_140003145;
        }
      }
      else if ((local_2b8 == 0x65766173) && (local_2b4 == '\0')) {
        piVar4 = local_2a8;
        iVar2 = FUN_140002ce0("%127s",piVar4,param_3,param_4);
        if (iVar2 == 1) {
          piVar4 = local_2a8;
          uVar3 = FUN_1400017f0(_Memory_00,(char *)piVar4);
          if ((int)uVar3 == 0) goto LAB_140003145;
          goto LAB_140003188;
        }
      }
      else {
        if ((local_2b8 == 0x74697571) && (local_2b4 == '\0')) break;
        puts("Commands: set <key> <value> | get <key> | del <key> | save <file> | quit");
      }
    }
    plVar5 = _Memory_00;
    do {
      _Memory = (void *)*plVar5;
      while (_Memory != (void *)0x0) {
        pvVar1 = *(void **)((longlong)_Memory + 0x280);
        free(_Memory);
        _Memory = pvVar1;
      }
      plVar5 = plVar5 + 1;
    } while (plVar5 != _Memory_00 + 0x40);
    free(_Memory_00);
    uVar3 = 0;
  }
  return uVar3;
LAB_14000310a:
  param_3 = local_228;
  piVar4 = local_2a8;
  iVar2 = FUN_140002ce0("%127s %511[^\n]",piVar4,param_3,param_4);
  if (iVar2 == 2) {
    param_3 = local_228;
    piVar4 = local_2a8;
    iVar2 = FUN_1400014b0((longlong)_Memory_00,(char *)piVar4,param_3);
    if (iVar2 == 0) {
LAB_140003145:
      puts("OK");
    }
    else {
LAB_140003188:
      puts("ERROR");
    }
  }
  goto LAB_140003098;
}