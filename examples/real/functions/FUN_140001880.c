/* Function: FUN_140001880 @ 140001880 */

undefined8 FUN_140001880(longlong param_1,char *param_2)

{
  FILE *_File;
  char *pcVar1;
  size_t sVar2;
  char acStack_2a9 [665];
  
  if (((param_1 != 0) && (param_2 != (char *)0x0)) &&
     (_File = fopen(param_2,"r"), _File != (FILE *)0x0)) {
    while (pcVar1 = fgets(acStack_2a9 + 1,0x282,_File), pcVar1 != (char *)0x0) {
      sVar2 = strlen(acStack_2a9 + 1);
      if ((sVar2 != 0) && (acStack_2a9[sVar2] == '\n')) {
        acStack_2a9[sVar2] = '\0';
      }
      pcVar1 = strchr(acStack_2a9 + 1,0x3d);
      if (pcVar1 != (char *)0x0) {
        *pcVar1 = '\0';
        FUN_1400014b0(param_1,acStack_2a9 + 1,pcVar1 + 1);
      }
    }
    fclose(_File);
    return 0;
  }
  return 0xffffffff;
}