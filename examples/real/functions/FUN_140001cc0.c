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