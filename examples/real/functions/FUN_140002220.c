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