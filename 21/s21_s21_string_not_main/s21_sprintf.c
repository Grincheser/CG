#include "s21_string.h"

// ── spec struct ───────────────────────────────────────────────────────────────

typedef struct {
  int  minus;
  int  plus;
  int  space;
  int  width;
  int  precision;
  int  has_prec;
  char length;
  char spec;
} FormatSpec;

// ── primitives ────────────────────────────────────────────────────────────────

static void reverse_buf(char *buf, int lo, int hi) {
  hi--;
  while (lo < hi) {
    char tmp = buf[lo];
    buf[lo]  = buf[hi];
    buf[hi]  = tmp;
    lo++;
    hi--;
  }
}

static int write_chars(char *buf, int pos, char ch, int n) {
  while (n-- > 0)
    buf[pos++] = ch;
  return pos;
}

// ── integer helpers ───────────────────────────────────────────────────────────

static int fmt_uint(char *tmp, unsigned long long val, int prec, int has_prec) {
  int len = 0;
  if (val == 0 && has_prec && prec == 0) return 0;
  if (val == 0) { tmp[len++] = '0'; return len; }
  while (val > 0) {
    tmp[len++] = (char)('0' + val % 10);
    val /= 10;
  }
  while (has_prec && len < prec)
    tmp[len++] = '0';
  reverse_buf(tmp, 0, len);
  return len;
}

static int int_part_to_str(char *buf, long long n) {
  if (n == 0) { buf[0] = '0'; return 1; }
  int len = 0;
  long long abs_n = (n < 0) ? -n : n;
  while (abs_n > 0) { buf[len++] = (char)('0' + abs_n % 10); abs_n /= 10; }
  reverse_buf(buf, 0, len);
  return len;
}

// ── handlers (FormatSpec by value — no -> anywhere) ───────────────────────────

static int handle_c(char *buf, int pos, FormatSpec f, char ch) {
  int pad_n = (f.width > 1) ? f.width - 1 : 0;
  if (!f.minus) pos = write_chars(buf, pos, ' ', pad_n);
  buf[pos++] = ch;
  if (f.minus)  pos = write_chars(buf, pos, ' ', pad_n);
  return pos;
}

static int handle_s(char *buf, int pos, FormatSpec f, const char *str) {
  if (!str) str = "(null)";
  int len = 0;
  while (str[len]) len++;
  if (f.has_prec && f.precision < len) len = f.precision;

  int pad_n = (f.width > len) ? f.width - len : 0;
  if (!f.minus) pos = write_chars(buf, pos, ' ', pad_n);
  for (int i = 0; i < len; i++) buf[pos++] = str[i];
  if (f.minus)  pos = write_chars(buf, pos, ' ', pad_n);
  return pos;
}

static int handle_d(char *buf, int pos, FormatSpec f, long long val) {
  char tmp[64];
  char sign_ch = 0;
  int  sign    = 0;

  if (val < 0)       { sign_ch = '-'; sign = 1; }
  else if (f.plus)   { sign_ch = '+'; sign = 1; }
  else if (f.space)  { sign_ch = ' '; sign = 1; }

  unsigned long long uval = (val < 0) ? (unsigned long long)(-val)
                                       : (unsigned long long)val;
  int dig      = fmt_uint(tmp, uval, f.precision, f.has_prec);
  int prec_pad = (f.has_prec && dig < f.precision) ? f.precision - dig : 0;
  int total    = sign + prec_pad + dig;
  int pad_n    = (f.width > total) ? f.width - total : 0;

  if (!f.minus) pos = write_chars(buf, pos, ' ', pad_n);
  if (sign_ch)  buf[pos++] = sign_ch;
  pos = write_chars(buf, pos, '0', prec_pad);
  for (int i = 0; i < dig; i++) buf[pos++] = tmp[i];
  if (f.minus)  pos = write_chars(buf, pos, ' ', pad_n);
  return pos;
}

static int handle_u(char *buf, int pos, FormatSpec f, unsigned long long val) {
  char tmp[64];
  int  dig   = fmt_uint(tmp, val, f.precision, f.has_prec);
  int  pad_n = (f.width > dig) ? f.width - dig : 0;

  if (!f.minus) pos = write_chars(buf, pos, ' ', pad_n);
  for (int i = 0; i < dig; i++) buf[pos++] = tmp[i];
  if (f.minus)  pos = write_chars(buf, pos, ' ', pad_n);
  return pos;
}

static int handle_f(char *buf, int pos, FormatSpec f, long double val) {
  int prec = f.has_prec ? f.precision : 6;
  char tmp[128];
  int  tlen = 0;

  int neg = (val < 0.0L);
  if (neg) val = -val;

  long double factor = 1.0L;
  for (int i = 0; i < prec; i++) factor *= 10.0L;
  val = (long double)(long long)(val * factor + 0.5L) / factor;

  long long   ipart = (long long)val;
  long double fpart = val - (long double)ipart;
  char sign_ch = neg ? '-' : (f.plus ? '+' : (f.space ? ' ' : 0));

  char ibuf[64];
  int  ilen  = int_part_to_str(ibuf, ipart);
  int  total = (sign_ch ? 1 : 0) + ilen + (prec > 0 ? 1 + prec : 0);
  int  pad_n = (f.width > total) ? f.width - total : 0;

  if (!f.minus) pos = write_chars(buf, pos, ' ', pad_n);
  if (sign_ch)  tmp[tlen++] = sign_ch;
  for (int i = 0; i < ilen; i++) tmp[tlen++] = ibuf[i];

  if (prec > 0) {
    tmp[tlen++] = '.';
    for (int i = 0; i < prec; i++) {
      fpart *= 10.0L;
      int d = (int)fpart;
      tmp[tlen++] = (char)('0' + d);
      fpart -= d;
    }
  }

  for (int i = 0; i < tlen; i++) buf[pos++] = tmp[i];
  if (f.minus) pos = write_chars(buf, pos, ' ', pad_n);
  return pos;
}

// ── parser (returns updated FormatSpec by value — no -> anywhere) ─────────────

typedef struct { const char *fmt; FormatSpec f; } ParseResult;

static ParseResult parse_flags(const char *fmt, FormatSpec f) {
  int done = 0;
  while (!done) {
    if      (*fmt == '-') { f.minus = 1; fmt++; }
    else if (*fmt == '+') { f.plus  = 1; fmt++; }
    else if (*fmt == ' ') { f.space = 1; fmt++; }
    else done = 1;
  }
  return (ParseResult){fmt, f};
}

static ParseResult parse_width(const char *fmt, FormatSpec f) {
  while (*fmt >= '0' && *fmt <= '9') {
    f.width = f.width * 10 + (*fmt - '0');
    fmt++;
  }
  return (ParseResult){fmt, f};
}

static ParseResult parse_precision(const char *fmt, FormatSpec f) {
  if (*fmt == '.') {
    fmt++;
    f.has_prec = 1;
    while (*fmt >= '0' && *fmt <= '9') {
      f.precision = f.precision * 10 + (*fmt - '0');
      fmt++;
    }
  }
  return (ParseResult){fmt, f};
}

static ParseResult parse_length(const char *fmt, FormatSpec f) {
  if (*fmt == 'h' || *fmt == 'l') {
    f.length = *fmt;
    fmt++;
  }
  return (ParseResult){fmt, f};
}

static ParseResult parse_spec(const char *fmt) {
  FormatSpec f = {0};
  ParseResult r;

  r = parse_flags(fmt, f);     fmt = r.fmt; f = r.f;
  r = parse_width(fmt, f);     fmt = r.fmt; f = r.f;
  r = parse_precision(fmt, f); fmt = r.fmt; f = r.f;
  r = parse_length(fmt, f);    fmt = r.fmt; f = r.f;

  f.spec = *fmt++;
  return (ParseResult){fmt, f};
}

// ── dispatch ──────────────────────────────────────────────────────────────────

static int dispatch(char *buf, int pos, FormatSpec f, va_list *ap) {
  if (f.spec == '%') { buf[pos++] = '%'; return pos; }
  if (f.spec == 'c') return handle_c(buf, pos, f, (char)va_arg(*ap, int));
  if (f.spec == 's') return handle_s(buf, pos, f, va_arg(*ap, const char *));

  if (f.spec == 'd') {
    long long val;
    if (f.length == 'h')      val = (short)va_arg(*ap, int);
    else if (f.length == 'l') val = va_arg(*ap, long);
    else                       val = va_arg(*ap, int);
    return handle_d(buf, pos, f, val);
  }

  if (f.spec == 'u') {
    unsigned long long val;
    if (f.length == 'h')      val = (unsigned short)va_arg(*ap, unsigned int);
    else if (f.length == 'l') val = va_arg(*ap, unsigned long);
    else                       val = va_arg(*ap, unsigned int);
    return handle_u(buf, pos, f, val);
  }

  if (f.spec == 'f') {
    long double val = (f.length == 'L') ? va_arg(*ap, long double)
                                        : (long double)va_arg(*ap, double);
    return handle_f(buf, pos, f, val);
  }

  return pos;
}

// ── public entry point ────────────────────────────────────────────────────────

int s21_sprintf(char *str, const char *format, ...) {
  va_list ap;
  va_start(ap, format);

  int         pos = 0;
  const char *fmt = format;

  while (*fmt) {
    if (*fmt != '%') {
      str[pos++] = *fmt++;
    } else {
      fmt++;
      ParseResult r = parse_spec(fmt);
      fmt = r.fmt;
      pos = dispatch(str, pos, r.f, &ap);
    }
  }

  str[pos] = '\0';
  va_end(ap);
  return pos;
}