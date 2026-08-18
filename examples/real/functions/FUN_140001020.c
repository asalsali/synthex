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