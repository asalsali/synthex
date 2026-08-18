/* Function: FUN_1400017f0 @ 1400017f0 */

undefined8 FUN_1400017f0(longlong *param_1,char *param_2)

{
  longlong *plVar1;
  longlong lVar2;
  FILE *_File;
  undefined8 uVar3;
  
  if (((param_1 == (longlong *)0x0) || (param_2 == (char *)0x0)) ||
     (_File = fopen(param_2,"w"), _File == (FILE *)0x0)) {
    uVar3 = 0xffffffff;
  }
  else {
    plVar1 = param_1 + 0x40;
    do {
      for (lVar2 = *param_1; lVar2 != 0; lVar2 = *(longlong *)(lVar2 + 0x280)) {
        FUN_140002d90(_File,"%s=%s\n",lVar2,lVar2 + 0x80);
      }
      param_1 = param_1 + 1;
    } while (param_1 != plVar1);
    fclose(_File);
    uVar3 = 0;
  }
  return uVar3;
}