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