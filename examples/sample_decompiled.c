/* Ghidra decompiled output -- stripped binary, x86_64 ELF */
/* Function: FUN_00401000 */
/* WARNING: Could not recover jumptable at 0x00401120 */

undefined8 FUN_00401000(long param_1, int param_2)
{
    int iVar1;
    long lVar2;
    undefined8 uVar3;
    void *pvVar4;
    long local_48;
    int local_3c;
    long local_38;
    long local_30;
    int local_28;
    long local_20;
    long local_10;

    local_10 = *(long *)(in_FS_OFFSET + 0x28);
    local_48 = param_1;
    local_3c = param_2;
    if (param_2 < 1) {
        uVar3 = 0xffffffffffffffff;
        goto LAB_004010f0;
    }
    pvVar4 = (void *)malloc(0x18);
    if (pvVar4 == (void *)0x0) {
        uVar3 = 0xffffffffffffffff;
        goto LAB_004010f0;
    }
    *(undefined8 *)pvVar4 = 0;
    *(undefined4 *)((long)pvVar4 + 8) = 0;
    *(undefined8 *)((long)pvVar4 + 0x10) = 0;
    local_38 = (long)pvVar4;
    local_30 = local_38;
    local_28 = 0;
    local_20 = 0;
    while (local_28 < local_3c) {
        lVar2 = *(long *)(local_48 + (long)local_28 * 8);
        if (lVar2 == 0) {
            goto LAB_00401080;
        }
        iVar1 = *(int *)(lVar2 + 4);
        if (iVar1 < 0) {
            goto LAB_00401080;
        }
        pvVar4 = (void *)malloc(0x18);
        if (pvVar4 == (void *)0x0) {
            goto LAB_004010c0;
        }
        *(long *)pvVar4 = lVar2;
        *(int *)((long)pvVar4 + 8) = iVar1;
        *(undefined8 *)((long)pvVar4 + 0x10) = 0;
        *(long *)(local_30 + 0x10) = (long)pvVar4;
        local_30 = (long)pvVar4;
        local_20 = local_20 + (long)iVar1;
LAB_00401080:
        local_28 = local_28 + 1;
    }
    if (local_20 == 0) {
        FUN_00401200(local_38);
        uVar3 = 0xffffffffffffffff;
        goto LAB_004010f0;
    }
    *(long *)(local_38 + 8) = local_20;
    uVar3 = local_38;
    goto LAB_004010f0;
LAB_004010c0:
    FUN_00401200(local_38);
    uVar3 = 0xffffffffffffffff;
LAB_004010f0:
    if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
        __stack_chk_fail();
    }
    return uVar3;
}


/* Function: FUN_00401200 */

void FUN_00401200(long param_1)
{
    long lVar1;
    long local_18;

    local_18 = param_1;
    while (local_18 != 0) {
        lVar1 = *(long *)(local_18 + 0x10);
        free((void *)local_18);
        local_18 = lVar1;
    }
    return;
}


/* Function: FUN_00401300 */

undefined8 FUN_00401300(long param_1, long param_2)
{
    int iVar1;
    long lVar2;
    undefined8 uVar3;
    long local_28;
    long local_20;
    int local_18;
    long local_10;

    local_10 = *(long *)(in_FS_OFFSET + 0x28);
    if (param_1 == 0) {
        uVar3 = 0;
        goto LAB_004013a0;
    }
    local_28 = *(long *)(param_1 + 0x10);
    local_20 = 0;
    local_18 = 0;
    while (local_28 != 0) {
        lVar2 = *(long *)local_28;
        if (lVar2 != 0) {
            iVar1 = FUN_00401400(lVar2, param_2);
            if (iVar1 != 0) {
                local_18 = local_18 + 1;
                local_20 = local_28;
            }
        }
        local_28 = *(long *)(local_28 + 0x10);
    }
    if (local_18 == 0) {
        uVar3 = 0;
        goto LAB_004013a0;
    }
    uVar3 = local_20;
LAB_004013a0:
    if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
        __stack_chk_fail();
    }
    return uVar3;
}


/* Function: FUN_00401400 */

int FUN_00401400(long param_1, long param_2)
{
    int iVar1;
    int local_c;

    local_c = 0;
    while (*(char *)(param_1 + (long)local_c) != '\0') {
        if (*(char *)(param_1 + (long)local_c) != *(char *)(param_2 + (long)local_c)) {
            return 0;
        }
        local_c = local_c + 1;
    }
    if (*(char *)(param_2 + (long)local_c) != '\0') {
        return 0;
    }
    return 1;
}
