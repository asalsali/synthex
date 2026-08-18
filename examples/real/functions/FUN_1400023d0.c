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