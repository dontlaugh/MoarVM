static uint32_t MVM_bithacks_count_bits(uint64_t value) {
    uint32_t count;

    for (count = 0; value; count++)
        value &= value - 1;

    return count;
}

static int MVM_bithacks_is_pow2z(uint64_t value)
{
    return (value & (value - 1)) == 0;
}

static uint64_t MVM_bithacks_next_greater_pow2(uint64_t value)
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    return value + 1;
}
