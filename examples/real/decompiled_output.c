/* Ghidra decompiled output -- kvstore.exe */

/* Function: FUN_140001000 @ 140001000 */

void FUN_140001000(void)

{
  return;
}



/* Function: FUN_140001010 @ 140001010 */

void FUN_140001010(void)

{
  fflush((FILE *)0x0);
  return;
}



/* Function: FUN_140001020 @ 140001020 */

ulonglong FUN_140001020(undefined8 param_1,undefined8 *param_2,char *param_3,ulonglong param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined8 *puVar4;
  int iVar5;
  longlong lVar6;
  undefined8 *puVar7;
  ulonglong uVar8;
  FILE *_File;
  LPTOP_LEVEL_EXCEPTION_FILTER pPVar9;
  int *piVar10;
  undefined4 *puVar11;
  uint *puVar12;
  undefined8 *puVar13;
  size_t sVar14;
  void *_Dst;
  undefined8 *puVar15;
  code *pcVar16;
  undefined8 uVar17;
  longlong *plVar18;
  longlong lVar19;
  char *_Size;
  longlong lVar20;
  longlong unaff_GS_OFFSET;
  bool bVar21;
  undefined4 local_4c [3];
  
  puVar1 = PTR_DAT_140005530;
  lVar20 = *(longlong *)(*(longlong *)(unaff_GS_OFFSET + 0x30) + 8);
  while( true ) {
    LOCK();
    lVar6 = *(longlong *)puVar1;
    bVar21 = lVar6 == 0;
    if (bVar21) {
      *(longlong *)puVar1 = lVar20;
      lVar6 = 0;
    }
    puVar2 = PTR_DAT_140005540;
    UNLOCK();
    if (bVar21) {
      bVar21 = false;
      goto LAB_14000106c;
    }
    if (lVar20 == lVar6) break;
    Sleep(1000);
  }
  bVar21 = true;
LAB_14000106c:
  if (*(int *)PTR_DAT_140005540 == 1) goto LAB_140001410;
  if (*(int *)PTR_DAT_140005540 == 0) {
    *(undefined4 *)PTR_DAT_140005540 = 1;
    _File = (FILE *)__acrt_iob_func(2);
    param_4 = 0;
    plVar18 = (longlong *)0x0;
    uVar8 = 4;
    setvbuf(_File,(char *)0x0,4,0);
    pcVar16 = FUN_140001010;
    iVar5 = _crt_atexit();
    if (iVar5 != 0) {
                    /* WARNING: Subroutine does not return */
      abort();
    }
    FUN_140001e30(pcVar16,plVar18,uVar8,param_4);
    pPVar9 = SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)PTR_FUN_1400055c0);
    *(LPTOP_LEVEL_EXCEPTION_FILTER *)PTR_DAT_140005520 = pPVar9;
    _set_invalid_parameter_handler(FUN_140001000);
    FUN_140002860();
    *(undefined4 *)PTR_DAT_1400054f0 = 1;
    *(undefined4 *)PTR_DAT_140005500 = 1;
    *(undefined4 *)PTR_DAT_140005510 = 1;
    DAT_140008008 = 0;
    if ((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
       (piVar10 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                         *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar10 == 0x4550)) {
      if ((short)piVar10[6] == 0x10b) {
        if (0xe < (uint)piVar10[0x1d]) {
          DAT_140008008 = (uint)(piVar10[0x3a] != 0);
        }
      }
      else if (((short)piVar10[6] == 0x20b) && (0xe < (uint)piVar10[0x21])) {
        param_4 = (ulonglong)(uint)piVar10[0x3e];
        DAT_140008008 = (uint)(piVar10[0x3e] != 0);
      }
    }
    param_3 = (char *)(ulonglong)*(uint *)PTR_DAT_1400054e0;
    if (*(uint *)PTR_DAT_1400054e0 != 0) goto LAB_140001370;
    _set_app_type(1);
    do {
      puVar11 = (undefined4 *)__p__fmode();
      *puVar11 = *(undefined4 *)PTR_DAT_1400055b0;
      puVar12 = (uint *)__p__commode();
      puVar7 = (undefined8 *)(ulonglong)*(uint *)PTR_DAT_140005590;
      *puVar12 = *(uint *)PTR_DAT_140005590;
      uVar17 = FUN_140001aa0();
      if (-1 < (int)uVar17) {
        if (*(int *)PTR_DAT_140005440 == 1) {
          FUN_140002210(PTR_FUN_1400055d0);
        }
        if (*(int *)PTR_DAT_1400054a0 == -1) {
          _configthreadlocale(-1);
        }
        param_2 = (undefined8 *)PTR_DAT_140005580;
        iVar5 = _initterm_e(PTR_DAT_140005570);
        if (iVar5 != 0) goto LAB_1400013ff;
        param_3 = (char *)&DAT_140008010;
        puVar7 = &DAT_140008018;
        local_4c[0] = *(undefined4 *)PTR_DAT_1400055e0;
        param_4 = (ulonglong)*(uint *)PTR_DAT_1400055a0;
        uVar17 = FUN_140002e50(&DAT_140008020,&DAT_140008018,&DAT_140008010,
                               *(uint *)PTR_DAT_1400055a0,local_4c);
        iVar5 = DAT_140008020;
        if (-1 < (int)uVar17) {
          lVar20 = (longlong)DAT_140008020;
          puVar13 = malloc((longlong)(DAT_140008020 + 1) << 3);
          puVar4 = DAT_140008018;
          if (puVar13 != (undefined8 *)0x0) {
            puVar15 = puVar13;
            if (iVar5 < 1) {
LAB_140001388:
              puVar3 = PTR_DAT_140005560;
              *puVar15 = 0;
              DAT_140008018 = puVar13;
              _initterm(PTR_DAT_140005550,puVar3);
              FUN_140001a80();
              *(undefined4 *)puVar2 = 2;
              goto LAB_140001096;
            }
            lVar6 = 1;
            while( true ) {
              lVar19 = lVar6;
              sVar14 = strlen((char *)puVar4[lVar19 + -1]);
              _Size = (char *)(sVar14 + 1);
              _Dst = malloc((size_t)_Size);
              puVar13[lVar19 + -1] = _Dst;
              if (_Dst == (void *)0x0) break;
              puVar7 = (undefined8 *)puVar4[lVar19 + -1];
              memcpy(_Dst,puVar7,(size_t)_Size);
              lVar6 = lVar19 + 1;
              param_3 = _Size;
              if (lVar19 == lVar20) {
                puVar15 = puVar13 + (int)lVar19;
                goto LAB_140001388;
              }
            }
          }
        }
      }
      FUN_140002e10(8,puVar7,param_3,param_4);
LAB_140001370:
      _set_app_type(2);
    } while( true );
  }
  DAT_140008004 = 1;
LAB_140001096:
  if (!bVar21) {
    LOCK();
    *(undefined8 *)puVar1 = 0;
    UNLOCK();
  }
  if (*(code **)PTR_PTR_140005490 != (code *)0x0) {
    (**(code **)PTR_PTR_140005490)(0,2,0);
  }
  puVar7 = (undefined8 *)FUN_140002e00();
  iVar5 = DAT_140008020;
  param_3 = DAT_140008010;
  *puVar7 = DAT_140008010;
  param_2 = DAT_140008018;
  uVar8 = FUN_140003050(iVar5,(longlong)DAT_140008018,param_3,param_4);
  if (DAT_140008008 != 0) {
    if (DAT_140008004 == 0) {
      _cexit();
      uVar8 = uVar8 & 0xffffffff;
    }
    return uVar8;
  }
LAB_140001409:
  FUN_140002840((int)uVar8);
LAB_140001410:
  uVar17 = 0x1f;
  FUN_140002e10(0x1f,param_2,param_3,param_4);
  *(undefined4 *)PTR_DAT_1400054e0 = 1;
  uVar8 = FUN_140001020(uVar17,param_2,param_3,param_4);
  return uVar8;
LAB_1400013ff:
  uVar8 = FUN_140002e10(10,param_2,param_3,param_4);
  goto LAB_140001409;
}



/* Function: FUN_140001420 @ 140001420 */

void FUN_140001420(undefined8 param_1,undefined8 *param_2,char *param_3,ulonglong param_4)

{
  *(undefined4 *)PTR_DAT_1400054e0 = 1;
  FUN_140001020(param_1,param_2,param_3,param_4);
  return;
}



/* Function: entry @ 140001440 */

void entry(undefined8 param_1,undefined8 *param_2,char *param_3,ulonglong param_4)

{
  *(undefined4 *)PTR_DAT_1400054e0 = 0;
  FUN_140001020(param_1,param_2,param_3,param_4);
  return;
}



/* Function: FUN_140001470 @ 140001470 */

void FUN_140001470(void)

{
  _crt_atexit(FUN_140001480);
  return;
}



/* Function: FUN_140001480 @ 140001480 */

void FUN_140001480(void)

{
  return;
}



/* Function: FUN_1400014a0 @ 1400014a0 */

void FUN_1400014a0(void)

{
  calloc(1,0x208);
  return;
}



/* Function: FUN_1400014b0 @ 1400014b0 */

undefined4 FUN_1400014b0(longlong param_1,char *param_2,char *param_3)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  size_t sVar4;
  char *pcVar5;
  char *pcVar6;
  
  if ((((param_2 != (char *)0x0 && param_3 != (char *)0x0) && (param_1 != 0)) &&
      (sVar4 = strlen(param_2), sVar4 < 0x80)) && (sVar4 = strlen(param_3), sVar4 < 0x200)) {
    cVar1 = *param_2;
    if (cVar1 == '\0') {
      uVar2 = 5;
    }
    else {
      uVar2 = 0x1505;
      pcVar6 = param_2;
      do {
        pcVar6 = pcVar6 + 1;
        uVar2 = uVar2 * 0x21 + (int)cVar1;
        cVar1 = *pcVar6;
      } while (cVar1 != '\0');
      uVar2 = uVar2 & 0x3f;
    }
    pcVar6 = *(char **)(param_1 + (ulonglong)uVar2 * 8);
    for (pcVar5 = pcVar6; pcVar5 != (char *)0x0; pcVar5 = *(char **)(pcVar5 + 0x280)) {
      iVar3 = strcmp(pcVar5,param_2);
      if (iVar3 == 0) {
        strncpy(pcVar5 + 0x80,param_3,0x1ff);
        pcVar5[0x27f] = '\0';
        return 0;
      }
    }
    pcVar5 = malloc(0x288);
    if (pcVar5 != (char *)0x0) {
      strncpy(pcVar5,param_2,0x7f);
      pcVar5[0x7f] = '\0';
      strncpy(pcVar5 + 0x80,param_3,0x1ff);
      *(char **)(param_1 + (ulonglong)uVar2 * 8) = pcVar5;
      *(int *)(param_1 + 0x200) = *(int *)(param_1 + 0x200) + 1;
      pcVar5[0x27f] = '\0';
      *(char **)(pcVar5 + 0x280) = pcVar6;
      return 0;
    }
  }
  return 0xffffffff;
}



/* Function: FUN_140001640 @ 140001640 */

char * FUN_140001640(longlong param_1,char *param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  ulonglong uVar4;
  char *pcVar5;
  
  if ((param_1 != 0) && (param_2 != (char *)0x0)) {
    cVar1 = *param_2;
    if (cVar1 == '\0') {
      uVar4 = 5;
    }
    else {
      uVar2 = 0x1505;
      pcVar5 = param_2;
      do {
        pcVar5 = pcVar5 + 1;
        uVar2 = uVar2 * 0x21 + (int)cVar1;
        cVar1 = *pcVar5;
      } while (cVar1 != '\0');
      uVar4 = (ulonglong)(uVar2 & 0x3f);
    }
    pcVar5 = *(char **)(param_1 + uVar4 * 8);
    while( true ) {
      if (pcVar5 == (char *)0x0) {
        return (char *)0x0;
      }
      iVar3 = strcmp(pcVar5,param_2);
      if (iVar3 == 0) break;
      pcVar5 = *(char **)(pcVar5 + 0x280);
    }
    return pcVar5 + 0x80;
  }
  return (char *)0x0;
}



/* Function: FUN_140001700 @ 140001700 */

undefined4 FUN_140001700(longlong param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  char *_Str1;
  uint uVar3;
  int iVar4;
  char *pcVar5;
  
  if ((param_1 != 0) && (param_2 != (char *)0x0)) {
    cVar1 = *param_2;
    if (cVar1 == '\0') {
      uVar3 = 5;
    }
    else {
      uVar3 = 0x1505;
      pcVar5 = param_2;
      do {
        pcVar5 = pcVar5 + 1;
        uVar3 = uVar3 * 0x21 + (int)cVar1;
        cVar1 = *pcVar5;
      } while (cVar1 != '\0');
      uVar3 = uVar3 & 0x3f;
    }
    pcVar5 = *(char **)(param_1 + (ulonglong)uVar3 * 8);
    _Str1 = (char *)0x0;
    while (pcVar2 = _Str1, _Str1 = pcVar5, _Str1 != (char *)0x0) {
      iVar4 = strcmp(_Str1,param_2);
      pcVar5 = *(char **)(_Str1 + 0x280);
      if (iVar4 == 0) {
        if (pcVar2 == (char *)0x0) {
          *(char **)(param_1 + (ulonglong)uVar3 * 8) = pcVar5;
        }
        else {
          *(char **)(pcVar2 + 0x280) = pcVar5;
        }
        free(_Str1);
        *(int *)(param_1 + 0x200) = *(int *)(param_1 + 0x200) + -1;
        return 0;
      }
    }
  }
  return 0xffffffff;
}



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



/* Function: FUN_140001940 @ 140001940 */

void FUN_140001940(undefined8 *param_1)

{
  void *pvVar1;
  void *_Memory;
  undefined8 *puVar2;
  
  if (param_1 != (undefined8 *)0x0) {
    puVar2 = param_1;
    do {
      _Memory = (void *)*puVar2;
      while (_Memory != (void *)0x0) {
        pvVar1 = *(void **)((longlong)_Memory + 0x280);
        free(_Memory);
        _Memory = pvVar1;
      }
      puVar2 = puVar2 + 1;
    } while (param_1 + 0x40 != puVar2);
    free(param_1);
    return;
  }
  return;
}



/* Function: FUN_1400019c0 @ 1400019c0 */

void FUN_1400019c0(void)

{
  code *pcVar1;
  
  pcVar1 = *(code **)PTR_DAT_140004000;
  while (pcVar1 != (code *)0x0) {
    (*pcVar1)();
    pcVar1 = *(code **)(PTR_DAT_140004000 + 8);
    PTR_DAT_140004000 = PTR_DAT_140004000 + 8;
  }
  return;
}



/* Function: FUN_140001a00 @ 140001a00 */

void FUN_140001a00(void)

{
  undefined8 *puVar1;
  uint uVar2;
  ulonglong uVar3;
  undefined8 *puVar4;
  
  uVar2 = (uint)*(undefined8 *)PTR_DAT_140005450;
  if (uVar2 == 0xffffffff) {
    uVar3 = 0;
    do {
      uVar2 = (uint)uVar3;
      uVar3 = (ulonglong)(uVar2 + 1);
    } while (*(longlong *)(PTR_DAT_140005450 + uVar3 * 8) != 0);
  }
  if (uVar2 != 0) {
    puVar4 = (undefined8 *)(PTR_DAT_140005450 + (ulonglong)uVar2 * 8);
    puVar1 = (undefined8 *)
             (PTR_DAT_140005450 + ((ulonglong)uVar2 - (ulonglong)(uVar2 - 1)) * 8 + -8);
    do {
      (*(code *)*puVar4)();
      puVar4 = puVar4 + -1;
    } while (puVar4 != puVar1);
  }
  _crt_atexit(FUN_1400019c0);
  return;
}



/* Function: FUN_140001a80 @ 140001a80 */

void FUN_140001a80(void)

{
  if (DAT_140008030 != 0) {
    return;
  }
  DAT_140008030 = 1;
  FUN_140001a00();
  return;
}



/* Function: FUN_140001aa0 @ 140001aa0 */

undefined8 FUN_140001aa0(void)

{
  return 0;
}



/* Function: tls_callback_1 @ 140001ab0 */

void tls_callback_1(undefined8 param_1,uint param_2)

{
  if ((param_2 != 3) && (param_2 != 0)) {
    return;
  }
  FUN_140002730(param_1,param_2);
  return;
}



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



/* Function: FUN_140001b50 @ 140001b50 */

undefined8 FUN_140001b50(void)

{
  return 0;
}



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



/* Function: FUN_140001cc0 @ 140001cc0 */

/* WARNING: Enum "SectionFlags": Some values do not have unique names */

void FUN_140001cc0(ulonglong param_1,undefined8 param_2,ulonglong param_3,ulonglong param_4)

{
  BOOL BVar1;
  DWORD DVar2;
  ulonglong *puVar3;
  IMAGE_SECTION_HEADER *pIVar4;
  undefined4 *puVar5;
  IMAGE_DOS_HEADER *pIVar6;
  SIZE_T SVar7;
  PDWORD lpflOldProtect;
  longlong lVar8;
  undefined8 uVar9;
  uint uVar10;
  _MEMORY_BASIC_INFORMATION local_48;
  
  lVar8 = (longlong)(int)DAT_1400080a4;
  if ((int)DAT_1400080a4 < 1) {
    lVar8 = 0;
  }
  else {
    param_4 = 0;
    puVar3 = (ulonglong *)(DAT_1400080a8 + 0x18);
    do {
      param_3 = *puVar3;
      if ((param_3 <= param_1) && (param_3 = param_3 + *(uint *)(puVar3[1] + 8), param_1 < param_3))
      {
        return;
      }
      uVar10 = (int)param_4 + 1;
      param_4 = (ulonglong)uVar10;
      puVar3 = puVar3 + 5;
    } while (uVar10 != DAT_1400080a4);
  }
  pIVar4 = FUN_140002980(param_1);
  if (pIVar4 == (IMAGE_SECTION_HEADER *)0x0) {
                    /* WARNING: Subroutine does not return */
    FUN_140001c60("Address %p has no image-section",param_1,param_3,param_4);
  }
  lVar8 = lVar8 * 0x28;
  puVar5 = (undefined4 *)(DAT_1400080a8 + lVar8);
  *(IMAGE_SECTION_HEADER **)(puVar5 + 8) = pIVar4;
  *puVar5 = 0;
  pIVar6 = FUN_140002ac0();
  uVar10 = pIVar4->VirtualAddress;
  *(char **)(DAT_1400080a8 + 0x18 + lVar8) = pIVar6->e_magic + uVar10;
  SVar7 = VirtualQuery(pIVar6->e_magic + uVar10,&local_48,0x30);
  if (SVar7 != 0) {
    if (((local_48.Protect - 4 & 0xfffffffb) != 0) && ((local_48.Protect - 0x40 & 0xffffffbf) != 0))
    {
      uVar9 = 0x40;
      if (local_48.Protect == 2) {
        uVar9 = 4;
      }
      lpflOldProtect = (PDWORD)(lVar8 + DAT_1400080a8);
      *(PVOID *)(lpflOldProtect + 2) = local_48.BaseAddress;
      *(SIZE_T *)(lpflOldProtect + 4) = local_48.RegionSize;
      BVar1 = VirtualProtect(local_48.BaseAddress,local_48.RegionSize,(DWORD)uVar9,lpflOldProtect);
      if (BVar1 == 0) {
        DVar2 = GetLastError();
                    /* WARNING: Subroutine does not return */
        FUN_140001c60("  VirtualProtect failed with code 0x%x",(ulonglong)DVar2,uVar9,lpflOldProtect
                     );
      }
    }
    DAT_1400080a4 = DAT_1400080a4 + 1;
    return;
  }
                    /* WARNING: Subroutine does not return */
  FUN_140001c60("  VirtualQuery failed for %d bytes at address %p",
                (ulonglong)(pIVar4->Misc).PhysicalAddress,
                *(undefined8 *)(DAT_1400080a8 + 0x18 + lVar8),param_4);
}



/* Function: FUN_140001e30 @ 140001e30 */

void FUN_140001e30(undefined8 param_1,longlong *param_2,ulonglong param_3,ulonglong param_4)

{
  uint *_Dst;
  uint uVar1;
  DWORD flNewProtect;
  SIZE_T dwSize;
  LPVOID lpAddress;
  longlong lVar2;
  undefined *puVar3;
  undefined *puVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  DWORD *pDVar7;
  uint uVar8;
  ulonglong uVar9;
  uint *puVar10;
  longlong lVar11;
  int *piVar12;
  int *piVar13;
  int iVar14;
  int *_Dst_00;
  ulonglong *puVar15;
  undefined8 auStack_90 [5];
  longlong alStack_68 [2];
  undefined1 auStack_58 [8];
  longlong local_50 [2];
  
  if (DAT_1400080a0 != 0) {
    return;
  }
  DAT_1400080a0 = 1;
  auStack_90[0] = 0x140001e6f;
  FUN_140002a00();
  auStack_90[0] = 0x140001e86;
  uVar5 = FUN_140002c60();
  puVar4 = PTR_DAT_140005470;
  puVar3 = PTR_IMAGE_DOS_HEADER_140005460;
  lVar2 = -uVar5;
  DAT_1400080a4 = 0;
  DAT_1400080a8 = auStack_58 + lVar2;
  if ((longlong)PTR_DAT_140005470 - (longlong)PTR_DAT_140005480 < 8) {
    DAT_1400080a4 = 0;
    return;
  }
  piVar12 = (int *)PTR_DAT_140005480;
  if ((longlong)PTR_DAT_140005470 - (longlong)PTR_DAT_140005480 < 0xc) {
LAB_140001ec3:
    if (*piVar12 == 0) {
      iVar14 = piVar12[1];
LAB_140001ed0:
      if (iVar14 == 0) {
        uVar1 = piVar12[2];
        if (uVar1 != 1) {
                    /* WARNING: Subroutine does not return */
          *(undefined **)((longlong)auStack_90 + lVar2) = &UNK_1400021c1;
          FUN_140001c60("  Unknown pseudo relocation protocol version %d.\n",(ulonglong)uVar1,
                        param_3,param_4);
        }
        puVar10 = (uint *)(piVar12 + 3);
        if (PTR_DAT_140005470 <= puVar10) {
          DAT_1400080a4 = 0;
          return;
        }
        do {
          while( true ) {
            uVar1 = puVar10[2];
            uVar9 = (ulonglong)uVar1 & 0xff;
            puVar15 = (ulonglong *)(puVar3 + *puVar10);
            uVar8 = (uint)uVar9;
            uVar5 = *puVar15;
            _Dst = (uint *)(puVar3 + puVar10[1]);
            if (uVar8 != 0x20) break;
            uVar6 = (ulonglong)*_Dst;
            if ((int)*_Dst < 0) {
              uVar6 = uVar6 | 0xffffffff00000000;
            }
            local_50[0] = (uVar6 - (longlong)puVar15) + uVar5;
            if (((uVar1 & 0xc0) == 0) && ((0xffffffff < local_50[0] || (local_50[0] < -0x80000000)))
               ) goto LAB_1400021a1;
            *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x1400020c3;
            FUN_140001cc0((ulonglong)_Dst,uVar9,(ulonglong)(uVar1 & 0xc0),uVar5);
            *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x1400020d4;
            memcpy(_Dst,local_50,4);
LAB_140001f6d:
            puVar10 = puVar10 + 3;
            if (puVar4 <= puVar10) goto LAB_140001ff0;
          }
          if (uVar8 < 0x21) {
            if (uVar8 == 8) {
              uVar6 = (ulonglong)(byte)*_Dst;
              if ((char)(byte)*_Dst < '\0') {
                uVar6 = uVar6 | 0xffffffffffffff00;
              }
              local_50[0] = (uVar6 - (longlong)puVar15) + uVar5;
              if (((uVar1 & 0xc0) == 0) && ((0xff < local_50[0] || (local_50[0] < -0x80)))) {
LAB_1400021a1:
                *(longlong *)((longlong)alStack_68 + lVar2) = local_50[0];
                    /* WARNING: Subroutine does not return */
                *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x1400021b5;
                FUN_140001c60("%d bit pseudo relocation at %p out of range, targeting %p, yielding the value %p.\n"
                              ,uVar9,_Dst,uVar5);
              }
              *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x140002127;
              FUN_140001cc0((ulonglong)_Dst,uVar9,(ulonglong)(uVar1 & 0xc0),uVar5);
              *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x140002138;
              memcpy(_Dst,local_50,1);
            }
            else {
              if (uVar8 != 0x10) goto LAB_14000218d;
              uVar6 = (ulonglong)(ushort)*_Dst;
              if ((short)(ushort)*_Dst < 0) {
                uVar6 = uVar6 | 0xffffffffffff0000;
              }
              local_50[0] = (uVar6 - (longlong)puVar15) + uVar5;
              if (((uVar1 & 0xc0) == 0) && ((0xffff < local_50[0] || (local_50[0] < -0x8000))))
              goto LAB_1400021a1;
              *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x140001f5c;
              FUN_140001cc0((ulonglong)_Dst,uVar9,(ulonglong)(uVar1 & 0xc0),uVar5);
              *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x140001f6d;
              memcpy(_Dst,local_50,2);
            }
            goto LAB_140001f6d;
          }
          if (uVar8 != 0x40) {
LAB_14000218d:
            local_50[0] = 0;
                    /* WARNING: Subroutine does not return */
            *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x1400021a1;
            FUN_140001c60("  Unknown pseudo relocation bit size %d.\n",uVar9,(ulonglong)uVar1,uVar5)
            ;
          }
          local_50[0] = (*(longlong *)_Dst - (longlong)puVar15) + uVar5;
          if (((uVar1 & 0xc0) == 0) && (-1 < local_50[0])) goto LAB_1400021a1;
          puVar10 = puVar10 + 3;
          *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x140001fd6;
          FUN_140001cc0((ulonglong)_Dst,uVar9,(ulonglong)(uVar1 & 0xc0),uVar5);
          *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x140001fe7;
          memcpy(_Dst,local_50,8);
        } while (puVar10 < puVar4);
        goto LAB_140001ff0;
      }
    }
  }
  else {
    param_2 = (longlong *)(ulonglong)*(uint *)PTR_DAT_140005480;
    if (*(uint *)PTR_DAT_140005480 == 0) {
      iVar14 = *(int *)(PTR_DAT_140005480 + 4);
      if (iVar14 == 0 && *(int *)(PTR_DAT_140005480 + 8) == 0) {
        piVar12 = (int *)(PTR_DAT_140005480 + 0xc);
        goto LAB_140001ec3;
      }
      goto LAB_140001ed0;
    }
  }
  if (PTR_DAT_140005470 <= piVar12) {
    DAT_1400080a4 = 0;
    return;
  }
  do {
    piVar13 = piVar12 + 2;
    _Dst_00 = (int *)(puVar3 + (uint)piVar12[1]);
    local_50[0] = CONCAT44(local_50[0]._4_4_,*piVar12 + *_Dst_00);
    *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x140002172;
    FUN_140001cc0((ulonglong)_Dst_00,param_2,param_3,param_4);
    param_3 = 4;
    *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x140002183;
    param_2 = local_50;
    memcpy(_Dst_00,local_50,4);
    piVar12 = piVar13;
  } while (piVar13 < puVar4);
LAB_140001ff0:
  lVar11 = 0;
  if (0 < DAT_1400080a4) {
    iVar14 = 0;
    do {
      pDVar7 = (DWORD *)(DAT_1400080a8 + lVar11);
      flNewProtect = *pDVar7;
      if (flNewProtect != 0) {
        dwSize = *(SIZE_T *)(pDVar7 + 4);
        lpAddress = *(LPVOID *)(pDVar7 + 2);
        *(undefined8 *)((longlong)auStack_90 + lVar2) = 0x14000202f;
        VirtualProtect(lpAddress,dwSize,flNewProtect,(PDWORD)local_50);
      }
      iVar14 = iVar14 + 1;
      lVar11 = lVar11 + 0x28;
    } while (iVar14 < DAT_1400080a4);
  }
  return;
}



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



/* Function: FUN_140002210 @ 140002210 */

void FUN_140002210(undefined8 param_1)

{
  DAT_1400080b0 = param_1;
  __setusermatherr();
  return;
}



/* Function: FUN_140002220 @ 140002220 */

bool FUN_140002220(uint *param_1)

{
  uint uVar1;
  bool bVar2;
  code *extraout_RAX;
  code *extraout_RAX_00;
  code *pcVar3;
  code *extraout_RAX_01;
  code *extraout_RAX_02;
  
  if ((param_1[1] & 2) == 0) {
    uVar1 = *param_1;
    if (((uVar1 & 0x20ffffff) != 0x20474343) || ((param_1[1] & 1) != 0)) {
      if (0xc0000096 < uVar1) goto LAB_140002360;
      if (uVar1 < 0xc000008c) {
        if (uVar1 == 0xc0000005) {
          signal(0xb);
          if (extraout_RAX_01 == (code *)0x1) {
            signal(0xb);
          }
          else {
            if (extraout_RAX_01 == (code *)0x0) goto LAB_140002360;
            (*extraout_RAX_01)(0xb);
          }
        }
        else {
          if (uVar1 < 0xc0000006) {
            return uVar1 != 0x80000002;
          }
          if (uVar1 != 0xc0000008) {
            if (uVar1 == 0xc000001d) goto switchD_140002270_caseD_c0000096;
            goto LAB_140002360;
          }
        }
      }
      else {
        switch(uVar1) {
        case 0xc000008d:
        case 0xc000008e:
        case 0xc000008f:
        case 0xc0000090:
        case 0xc0000091:
        case 0xc0000093:
          signal(8);
          pcVar3 = extraout_RAX_00;
          if (extraout_RAX_00 == (code *)0x1) {
            signal(8);
            FUN_140002860();
          }
          else {
LAB_1400022f6:
            if (pcVar3 == (code *)0x0) goto LAB_140002360;
            (*pcVar3)(8);
          }
          break;
        case 0xc0000094:
        case 0xc0000095:
          signal(8);
          pcVar3 = extraout_RAX_02;
          if (extraout_RAX_02 != (code *)0x1) goto LAB_1400022f6;
          signal(8);
          break;
        case 0xc0000096:
switchD_140002270_caseD_c0000096:
          signal(4);
          if (extraout_RAX == (code *)0x1) {
            signal(4);
          }
          else {
            if (extraout_RAX == (code *)0x0) goto LAB_140002360;
            (*extraout_RAX)(4);
          }
        }
      }
    }
    bVar2 = false;
  }
  else {
LAB_140002360:
    bVar2 = true;
  }
  return bVar2;
}



/* Function: FUN_1400023d0 @ 1400023d0 */

undefined8 FUN_1400023d0(undefined8 *param_1)

{
  uint uVar1;
  code *extraout_RAX;
  code *pcVar2;
  undefined8 uVar3;
  code *extraout_RAX_00;
  code *extraout_RAX_01;
  code *extraout_RAX_02;
  
  uVar1 = *(uint *)*param_1;
  if (((uVar1 & 0x20ffffff) == 0x20474343) && ((((uint *)*param_1)[1] & 1) == 0)) {
    return 0xffffffff;
  }
  if (0xc0000096 < uVar1) goto LAB_14000243f;
  if (uVar1 < 0xc000008c) {
    if (uVar1 == 0xc0000005) {
      signal(0xb);
      if (extraout_RAX_02 == (code *)0x1) {
        signal(0xb);
        return 0xffffffff;
      }
      if (extraout_RAX_02 != (code *)0x0) {
        (*extraout_RAX_02)(0xb);
        return 0xffffffff;
      }
      goto LAB_14000243f;
    }
    if (uVar1 < 0xc0000006) {
      if (uVar1 == 0x80000002) {
        return 0xffffffff;
      }
      goto LAB_14000243f;
    }
    if (uVar1 == 0xc0000008) {
      return 0xffffffff;
    }
    if (uVar1 != 0xc000001d) goto LAB_14000243f;
switchD_14000241b_caseD_c0000096:
    signal(4);
    if (extraout_RAX_00 == (code *)0x1) {
      signal(4);
    }
    else {
      if (extraout_RAX_00 == (code *)0x0) goto LAB_14000243f;
      (*extraout_RAX_00)(4);
    }
  }
  else {
    switch(uVar1) {
    case 0xc000008d:
    case 0xc000008e:
    case 0xc000008f:
    case 0xc0000090:
    case 0xc0000091:
    case 0xc0000093:
      signal(8);
      pcVar2 = extraout_RAX;
      if (extraout_RAX == (code *)0x1) {
        signal(8);
        FUN_140002860();
        return 0xffffffff;
      }
      break;
    default:
      goto switchD_14000241b_caseD_c0000092;
    case 0xc0000094:
    case 0xc0000095:
      signal(8);
      pcVar2 = extraout_RAX_01;
      if (extraout_RAX_01 == (code *)0x1) {
        signal(8);
        return 0xffffffff;
      }
      break;
    case 0xc0000096:
      goto switchD_14000241b_caseD_c0000096;
    }
    if (pcVar2 == (code *)0x0) {
LAB_14000243f:
      if (DAT_1400080d0 == (code *)0x0) {
        return 0;
      }
                    /* WARNING: Could not recover jumptable at 0x000140002453. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar3 = (*DAT_1400080d0)(param_1);
      return uVar3;
    }
    (*pcVar2)(8);
  }
switchD_14000241b_caseD_c0000092:
  return 0xffffffff;
}



/* Function: FUN_1400025a0 @ 1400025a0 */

void FUN_1400025a0(void)

{
  DWORD *pDVar1;
  DWORD DVar2;
  LPVOID pvVar3;
  
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
  for (pDVar1 = DAT_1400080e0; pDVar1 != (DWORD *)0x0; pDVar1 = *(DWORD **)(pDVar1 + 4)) {
    pvVar3 = TlsGetValue(*pDVar1);
    DVar2 = GetLastError();
    if ((pvVar3 != (LPVOID)0x0) && (DVar2 == 0)) {
      (**(code **)(pDVar1 + 2))(pvVar3);
    }
  }
                    /* WARNING: Could not recover jumptable at 0x000140002609. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
  return;
}



/* Function: FUN_140002610 @ 140002610 */

undefined8 FUN_140002610(undefined4 param_1,undefined8 param_2)

{
  undefined4 *puVar1;
  
  if (DAT_1400080e8 != 0) {
    puVar1 = calloc(1,0x18);
    if (puVar1 == (undefined4 *)0x0) {
      return 0xffffffff;
    }
    *(undefined8 *)(puVar1 + 2) = param_2;
    *puVar1 = param_1;
    EnterCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
    *(undefined4 **)(puVar1 + 4) = DAT_1400080e0;
    DAT_1400080e0 = puVar1;
    LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
  }
  return 0;
}



/* Function: FUN_140002690 @ 140002690 */

undefined8 FUN_140002690(int param_1)

{
  int *piVar1;
  int *_Memory;
  int *piVar2;
  
  if (DAT_1400080e8 == 0) {
    return 0;
  }
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
  if (DAT_1400080e0 != (int *)0x0) {
    piVar1 = DAT_1400080e0;
    piVar2 = (int *)0x0;
    do {
      _Memory = piVar1;
      piVar1 = *(int **)(_Memory + 4);
      if (*_Memory == param_1) {
        if (piVar2 != (int *)0x0) {
          *(int **)(piVar2 + 4) = piVar1;
          piVar1 = DAT_1400080e0;
        }
        DAT_1400080e0 = piVar1;
        free(_Memory);
        break;
      }
      piVar2 = _Memory;
    } while (piVar1 != (int *)0x0);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
  return 0;
}



/* Function: FUN_140002730 @ 140002730 */

undefined8 FUN_140002730(undefined8 param_1,uint param_2)

{
  void *pvVar1;
  void *_Memory;
  
  if (param_2 == 2) {
    FUN_140002860();
    return 1;
  }
  if (param_2 < 3) {
    if (param_2 == 0) {
      if (DAT_1400080e8 != 0) {
        FUN_1400025a0();
      }
      if (DAT_1400080e8 == 1) {
        DAT_1400080e8 = 1;
        _Memory = DAT_1400080e0;
        while (_Memory != (void *)0x0) {
          pvVar1 = *(void **)((longlong)_Memory + 0x10);
          free(_Memory);
          _Memory = pvVar1;
        }
        DAT_1400080e0 = (void *)0x0;
        DAT_1400080e8 = 0;
        DeleteCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
      }
    }
    else {
      if (DAT_1400080e8 == 0) {
        InitializeCriticalSection((LPCRITICAL_SECTION)&DAT_140008100);
      }
      DAT_1400080e8 = 1;
    }
  }
  else if ((param_2 == 3) && (DAT_1400080e8 != 0)) {
    FUN_1400025a0();
  }
  return 1;
}



/* Function: FUN_140002840 @ 140002840 */

void FUN_140002840(int param_1)

{
  (**(code **)PTR_PTR_exit_1400054d0)();
  return;
}



/* Function: FUN_140002860 @ 140002860 */

void FUN_140002860(void)

{
  return;
}



/* Function: FUN_140002870 @ 140002870 */

bool FUN_140002870(short *param_1)

{
  if ((*param_1 == 0x5a4d) &&
     (*(int *)((longlong)param_1 + (longlong)*(int *)(param_1 + 0x1e)) == 0x4550)) {
    return (short)((int *)((longlong)param_1 + (longlong)*(int *)(param_1 + 0x1e)))[6] == 0x20b;
  }
  return false;
}



/* Function: FUN_1400028a0 @ 1400028a0 */

longlong FUN_1400028a0(longlong param_1,ulonglong param_2)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  
  lVar3 = param_1 + *(int *)(param_1 + 0x3c);
  lVar2 = lVar3 + 0x18 + (ulonglong)*(ushort *)(lVar3 + 0x14);
  if (*(ushort *)(lVar3 + 6) != 0) {
    lVar1 = lVar2 + 0x28;
    do {
      if ((*(uint *)(lVar2 + 0xc) <= param_2) &&
         (param_2 < *(uint *)(lVar2 + 0xc) + *(int *)(lVar2 + 8))) {
        return lVar2;
      }
      lVar2 = lVar2 + 0x28;
    } while (lVar2 != lVar1 + (ulonglong)(*(ushort *)(lVar3 + 6) - 1) * 0x28);
  }
  return 0;
}



/* Function: FUN_1400028f0 @ 1400028f0 */

/* WARNING: Enum "SectionFlags": Some values do not have unique names */

IMAGE_SECTION_HEADER * FUN_1400028f0(char *param_1)

{
  int iVar1;
  size_t sVar2;
  IMAGE_SECTION_HEADER *_Str1;
  int *piVar3;
  uint uVar4;
  
  sVar2 = strlen(param_1);
  if ((((sVar2 < 9) && (*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d)) &&
      (piVar3 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                       *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar3 == 0x4550)) &&
     (((short)piVar3[6] == 0x20b &&
      (_Str1 = (IMAGE_SECTION_HEADER *)
               ((longlong)piVar3 + (ulonglong)*(ushort *)(piVar3 + 5) + 0x18),
      *(short *)((longlong)piVar3 + 6) != 0)))) {
    uVar4 = 0;
    do {
      iVar1 = strncmp(_Str1->Name,param_1,8);
      if (iVar1 == 0) {
        return _Str1;
      }
      uVar4 = uVar4 + 1;
      _Str1 = _Str1 + 1;
    } while (uVar4 < *(ushort *)((longlong)piVar3 + 6));
  }
  return (IMAGE_SECTION_HEADER *)0x0;
}



/* Function: FUN_140002980 @ 140002980 */

/* WARNING: Enum "SectionFlags": Some values do not have unique names */

IMAGE_SECTION_HEADER * FUN_140002980(longlong param_1)

{
  IMAGE_SECTION_HEADER *pIVar1;
  IMAGE_SECTION_HEADER *pIVar2;
  int *piVar3;
  
  if (((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
      (piVar3 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                       *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar3 == 0x4550)) &&
     ((short)piVar3[6] == 0x20b)) {
    pIVar2 = (IMAGE_SECTION_HEADER *)((longlong)piVar3 + (ulonglong)*(ushort *)(piVar3 + 5) + 0x18);
    if (*(ushort *)((longlong)piVar3 + 6) != 0) {
      pIVar1 = pIVar2 + (ulonglong)(*(ushort *)((longlong)piVar3 + 6) - 1) + 1;
      do {
        if (((ulonglong)(uint)pIVar2->VirtualAddress <=
             (ulonglong)(param_1 - (longlong)PTR_IMAGE_DOS_HEADER_140005460)) &&
           ((ulonglong)(param_1 - (longlong)PTR_IMAGE_DOS_HEADER_140005460) <
            (ulonglong)(pIVar2->VirtualAddress + (pIVar2->Misc).PhysicalAddress))) {
          return pIVar2;
        }
        pIVar2 = pIVar2 + 1;
      } while (pIVar2 != pIVar1);
    }
    return (IMAGE_SECTION_HEADER *)0x0;
  }
  return (IMAGE_SECTION_HEADER *)0x0;
}



/* Function: FUN_140002a00 @ 140002a00 */

undefined2 FUN_140002a00(void)

{
  int *piVar1;
  
  if (((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
      (piVar1 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                       *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar1 == 0x4550)) &&
     ((short)piVar1[6] == 0x20b)) {
    return *(undefined2 *)((longlong)piVar1 + 6);
  }
  return 0;
}



/* Function: FUN_140002a40 @ 140002a40 */

/* WARNING: Enum "SectionFlags": Some values do not have unique names */

IMAGE_SECTION_HEADER * FUN_140002a40(longlong param_1)

{
  IMAGE_SECTION_HEADER *pIVar1;
  IMAGE_SECTION_HEADER *pIVar2;
  int *piVar3;
  
  if (((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
      (piVar3 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                       *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar3 == 0x4550)) &&
     ((short)piVar3[6] == 0x20b)) {
    pIVar2 = (IMAGE_SECTION_HEADER *)((longlong)piVar3 + (ulonglong)*(ushort *)(piVar3 + 5) + 0x18);
    if (*(ushort *)((longlong)piVar3 + 6) != 0) {
      pIVar1 = pIVar2 + (ulonglong)(*(ushort *)((longlong)piVar3 + 6) - 1) + 1;
      do {
        if ((pIVar2->Characteristics & IMAGE_SCN_MEM_EXECUTE) != 0) {
          if (param_1 == 0) {
            return pIVar2;
          }
          param_1 = param_1 + -1;
        }
        pIVar2 = pIVar2 + 1;
      } while (pIVar1 != pIVar2);
    }
    return (IMAGE_SECTION_HEADER *)0x0;
  }
  return (IMAGE_SECTION_HEADER *)0x0;
}



/* Function: FUN_140002ac0 @ 140002ac0 */

IMAGE_DOS_HEADER * FUN_140002ac0(void)

{
  IMAGE_DOS_HEADER *pIVar1;
  
  if ((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
     (*(int *)(PTR_IMAGE_DOS_HEADER_140005460 + *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)) ==
      0x4550)) {
    pIVar1 = (IMAGE_DOS_HEADER *)0x0;
    if ((short)*(int *)((longlong)
                        (PTR_IMAGE_DOS_HEADER_140005460 +
                        *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)) + 0x18) == 0x20b) {
      pIVar1 = (IMAGE_DOS_HEADER *)PTR_IMAGE_DOS_HEADER_140005460;
    }
    return pIVar1;
  }
  return (IMAGE_DOS_HEADER *)0x0;
}



/* Function: FUN_140002b00 @ 140002b00 */

uint FUN_140002b00(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  int *piVar3;
  
  if (((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
      (piVar3 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                       *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar3 == 0x4550)) &&
     ((short)piVar3[6] == 0x20b)) {
    lVar2 = (longlong)piVar3 + (ulonglong)*(ushort *)(piVar3 + 5) + 0x18;
    if (*(ushort *)((longlong)piVar3 + 6) != 0) {
      lVar1 = lVar2 + 0x28;
      while (((ulonglong)(param_1 - (longlong)PTR_IMAGE_DOS_HEADER_140005460) <
              (ulonglong)*(uint *)(lVar2 + 0xc) ||
             ((ulonglong)(*(uint *)(lVar2 + 0xc) + *(int *)(lVar2 + 8)) <=
              (ulonglong)(param_1 - (longlong)PTR_IMAGE_DOS_HEADER_140005460)))) {
        lVar2 = lVar2 + 0x28;
        if (lVar1 + (ulonglong)(*(ushort *)((longlong)piVar3 + 6) - 1) * 0x28 == lVar2) {
          return 0;
        }
      }
      return ~*(uint *)(lVar2 + 0x24) >> 0x1f;
    }
  }
  return 0;
}



/* Function: FUN_140002b90 @ 140002b90 */

char * FUN_140002b90(int param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  undefined *puVar3;
  longlong lVar4;
  int *piVar5;
  
  if ((((*(short *)PTR_IMAGE_DOS_HEADER_140005460 == 0x5a4d) &&
       (piVar5 = (int *)(PTR_IMAGE_DOS_HEADER_140005460 +
                        *(int *)(PTR_IMAGE_DOS_HEADER_140005460 + 0x3c)), *piVar5 == 0x4550)) &&
      ((short)piVar5[6] == 0x20b)) && (uVar2 = (ulonglong)(uint)piVar5[0x24], piVar5[0x24] != 0)) {
    lVar4 = (longlong)piVar5 + (ulonglong)*(ushort *)(piVar5 + 5) + 0x18;
    if (*(ushort *)((longlong)piVar5 + 6) != 0) {
      lVar1 = lVar4 + 0x28;
      while ((uVar2 < *(uint *)(lVar4 + 0xc) ||
             (*(uint *)(lVar4 + 0xc) + *(int *)(lVar4 + 8) <= uVar2))) {
        lVar4 = lVar4 + 0x28;
        if (lVar1 + (ulonglong)(*(ushort *)((longlong)piVar5 + 6) - 1) * 0x28 == lVar4) {
          return (char *)0x0;
        }
      }
      for (puVar3 = PTR_IMAGE_DOS_HEADER_140005460 + uVar2;
          (*(int *)(puVar3 + 4) != 0 || (*(int *)(puVar3 + 0xc) != 0)); puVar3 = puVar3 + 0x14) {
        if (param_1 < 1) {
          return PTR_IMAGE_DOS_HEADER_140005460 + *(uint *)(puVar3 + 0xc);
        }
        param_1 = param_1 + -1;
      }
      return (char *)0x0;
    }
  }
  return (char *)0x0;
}



/* Function: FUN_140002c60 @ 140002c60 */

ulonglong FUN_140002c60(void)

{
  ulonglong in_RAX;
  ulonglong uVar1;
  undefined8 *puVar2;
  undefined8 local_res8 [4];
  
  puVar2 = local_res8;
  uVar1 = in_RAX;
  if (0xfff < in_RAX) {
    do {
      puVar2 = puVar2 + -0x200;
      *puVar2 = *puVar2;
      uVar1 = uVar1 - 0x1000;
    } while (0x1000 < uVar1);
  }
  *(undefined8 *)((longlong)puVar2 - uVar1) = *(undefined8 *)((longlong)puVar2 - uVar1);
  return in_RAX;
}



/* Function: FUN_140002ca0 @ 140002ca0 */

void FUN_140002ca0(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)FUN_140002df0();
  __stdio_common_vfprintf(*puVar1,param_1,param_2,0,param_3);
  return;
}



/* Function: FUN_140002ce0 @ 140002ce0 */

void FUN_140002ce0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined8 local_res10;
  undefined8 local_res18;
  undefined8 local_res20;
  
  local_res10 = param_2;
  local_res18 = param_3;
  local_res20 = param_4;
  uVar1 = __acrt_iob_func(0);
  puVar2 = (undefined8 *)FUN_140002de0();
  __stdio_common_vfscanf(*puVar2,uVar1,param_1,0,&local_res10);
  return;
}



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



/* Function: FUN_140002de0 @ 140002de0 */

undefined * FUN_140002de0(void)

{
  return &DAT_140004050;
}



/* Function: FUN_140002df0 @ 140002df0 */

undefined * FUN_140002df0(void)

{
  return &DAT_140004060;
}



/* Function: FUN_140002e00 @ 140002e00 */

undefined * FUN_140002e00(void)

{
  return *(undefined **)PTR_PTR_1400054b0;
}



/* Function: FUN_140002e10 @ 140002e10 */

void FUN_140002e10(uint param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  
  uVar1 = __acrt_iob_func(2);
  FUN_140002d90(uVar1,"runtime error %d\n",(ulonglong)param_1,param_4);
  (**(code **)PTR_PTR__exit_1400054c0)(0xff);
  return;
}



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



