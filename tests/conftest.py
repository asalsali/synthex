"""Shared fixtures for Synthex tests."""

from pathlib import Path

import pytest


EXAMPLES_DIR = Path(__file__).resolve().parent.parent / "examples"
SAMPLE_FILE = EXAMPLES_DIR / "sample_decompiled.c"


@pytest.fixture
def sample_code():
    return SAMPLE_FILE.read_text(encoding="utf-8")


@pytest.fixture
def clean_c_code():
    """Well-structured C code for scoring baseline."""
    return '''\
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Process a list of items and return the count.
 * @param items Pointer to item array.
 * @param count Number of items.
 * @return Total processed items, or -1 on error.
 */
int process_items(int *items, int count) {
    if (items == NULL || count <= 0) {
        return -1;
    }

    int total = 0;
    for (int i = 0; i < count; i++) {
        total += items[i];
    }

    return total;
}
'''


@pytest.fixture
def ghidra_c_code():
    """Typical Ghidra decompiler output — generated names, gotos, weak types."""
    return '''\
undefined8 FUN_00401000(long param_1, int param_2) {
    int iVar1;
    undefined8 uVar3;
    void *pvVar4;
    long local_48;

    local_48 = param_1;
    if (param_2 < 1) {
        uVar3 = 0xffffffffffffffff;
        goto LAB_004010f0;
    }
    pvVar4 = (void *)malloc(0x18);
    if (pvVar4 == (void *)0x0) {
        uVar3 = 0xffffffffffffffff;
        goto LAB_004010f0;
    }
    iVar1 = strcmp((char *)param_1, "test");
    uVar3 = 0;
LAB_004010f0:
    return uVar3;
}
'''
