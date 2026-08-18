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