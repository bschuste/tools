#pragma TCS_align=4
static const uint8_t qtab_lum[32] = {
8, 8, 8, 8, 8, 10, 12, 14, 
16, 17, 18, 19, 20, 21, 22, 23, 
24, 25, 26, 27, 28, 29, 30, 31, 
32, 34, 36, 38, 40, 42, 44, 46, 
};

#pragma TCS_align=4
static const uint8_t qtab_chrom[32] = {
8, 8, 8, 8, 8, 9, 9, 10, 
10, 11, 11, 12, 12, 13, 13, 14, 
14, 15, 15, 16, 16, 17, 17, 18, 
18, 19, 20, 21, 22, 23, 24, 25, 
};

static const uint32_t div_lum[32] = {
536870913, 536870913, 536870913, 536870913, 536870913, 429496730, 357913942, 306783379, 
268435457, 252645136, 238609295, 226050911, 214748365, 204522253, 195225787, 186737709, 
178956971, 171798692, 165191050, 159072863, 153391690, 148102321, 143165577, 138547333, 
134217729, 126322568, 119304648, 113025456, 107374183, 102261127, 97612894, 93368855, 
};

static const uint32_t div_chrom[32] = {
536870913, 536870913, 536870913, 536870913, 536870913, 477218589, 477218589, 429496730, 
429496730, 390451573, 390451573, 357913942, 357913942, 330382100, 330382100, 306783379, 
306783379, 286331154, 286331154, 268435457, 268435457, 252645136, 252645136, 238609295, 
238609295, 226050911, 214748365, 204522253, 195225787, 186737709, 178956971, 171798692, 
};

static const uint32_t inv_q[32] = {
1, 1, 2147483649, 1431655766, 1073741825, 858993460, 715827883, 613566757, 
536870913, 477218589, 429496730, 390451573, 357913942, 330382100, 306783379, 286331154, 
268435457, 252645136, 238609295, 226050911, 214748365, 204522253, 195225787, 186737709, 
178956971, 171798692, 165191050, 159072863, 153391690, 148102321, 143165577, 138547333, 
};

