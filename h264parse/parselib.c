typedef struct {
UInt32 word;
Int avail_bits;
UInt32 mask;
UInt32 exp_mask;
}  bitstream_t, *bitstream_p;


bitstream_t bts;
bitstream_p bts_p = &bts;

init_bits() {
  word = *p32_buf;
  avail_bits = 32;
  mask = 0xffffffff;
}

int read_bits(a) {
  ra = avail_bits - a;
  rb = a - avail_bits;
  if (avail_bits == a) {
    val32 = word;
    word = *p32_bits;
    avail_bits = 32;
    mask = 0xffffffff;
  }
  else if (ra > 0) {
    val32 = word >> ra;
    mask = mask >> ra;
    word = word & mask;
  }
  else {
    val32 = word << rb;
    tmp = *p32_buf;
    mask = 0xffffffff >> rb;
    val32 |= tmp >> (32-rb);
    word = tmp & mask;
  }
  return val32;
}

int read_exp() {
  test = 1 << (avail_bits - 1);
  len = 0;
  if (word&test) {
    val32 = 0;
    avail_bits--;
    if (!avail_bits) {
        word = *p32_bits;
        mask = 0xffffffff;
        avail_bits = 32;
    }
    else {
        mask = mask>>1;
        word = word&(~test);
    }
  }
  else {
    while !(word&test) {
      word = word&(~test);
      test = test >> 1;
      mask = mask >> 1;
      len++;
      if (!test) {
        word = *p32_bits;
        test = 0x80000000;
        mask = 0x7fffffff;
      }
    }
    word  = word&(~test);
    val32 = (word>>(32-len)) - 1;
  }
  return val32;
}
