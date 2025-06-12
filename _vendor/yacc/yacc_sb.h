#ifndef YACC_SB_H_
#define YACC_SB_H_

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#ifndef YACC_SB_API
#ifdef YACC_SB_STATIC
#define YACC_SB_API static
#else
#define YACC_SB_API extern
#endif // YACC_SB_STATIC
#endif // YACC_SB_API


// DECLARATIONS

typedef struct {
    char *items;
    size_t len;
    size_t capacity;
} StringBuilder;


#define sb_clear_and_free(sb) da_clear_and_free((sb))

#define sb_expand_buffer(sb, new_cap)      \
    do {                                   \
        if ((sb)->capacity < (new_cap)) {  \
            da_reserve((sb), (new_cap));   \
        }                                  \
    } while (0)

#define sb_farg "%.*s"

#define sb_expand(sb) (int)(sb).len, (sb).items

YACC_SB_API StringBuilder sb_create(size_t capacity);
YACC_SB_API StringBuilder sb_clone(const StringBuilder *sb);
YACC_SB_API void sb_append_rune(StringBuilder *sb, int rune);
YACC_SB_API void sb_append_sv(StringBuilder *sb, const StringView *sv);
YACC_SB_API void sb_append_cstr(StringBuilder *sb, const char *cstr);
YACC_SB_API void sb_append_sb(StringBuilder *dest, const StringBuilder *src);
YACC_SB_API void sb_appendf(StringBuilder *sb, const char *fmt, ...);

// for COW String
#define sb_append_str(sb_ptr, str_ptr) sb_append_sb((sb_ptr), &(str_ptr)->h->b)

YACC_SB_API char *sb_get_cstr(StringBuilder *sb);


#endif // YACC_SB_H_


// IMPLEMENTATION

#ifdef YACC_SB_IMPLEMENTATION

YACC_SB_API StringBuilder sb_create(size_t capacity)
{
  StringBuilder sb = {0};
  da_reserve(&sb, capacity);
  return sb;
}

YACC_SB_API StringBuilder sb_clone(const StringBuilder *sb)
{
  StringBuilder clone = sb_create(sb->len);
  memcpy(clone.items, sb->items, sb->len);
  return clone;
}

YACC_SB_API void sb_append_rune(StringBuilder *sb, int rune)
{
  assert(rune >= -128 && rune <= 127);

  da_append(sb, (char)rune);
}

YACC_SB_API void sb_append_sv(StringBuilder *sb, const StringView *sv)
{
  for (size_t i = 0; i < sv->len; ++i) {
    sb_append_rune(sb, sv->begin[i]);
  }
}

YACC_SB_API void sb_append_cstr(StringBuilder *sb, const char *cstr)
{
  StringView sv = sv_from_cstr(cstr);
  sb_append_sv(sb, &sv);
}

YACC_SB_API void sb_append_sb(StringBuilder *dest, const StringBuilder *src)
{
  size_t new_capacity = dest->len + src->len;
  if (dest->capacity < new_capacity) {
    sb_expand_buffer(dest, new_capacity);
    dest->capacity = new_capacity;
  }
  memcpy(dest->items + dest->len, src->items, src->len);
  dest->len = new_capacity;
}

YACC_SB_API char *sb_get_cstr(StringBuilder *sb)
{
  if ('\0' != sb->items[sb->len])
    da_append(sb, '\0');

  return sb->items;
}

YACC_SB_API void sb_appendf(StringBuilder *sb, const char *fmt, ...)
{
  assert(NULL != sb);

  va_list args;

  va_start(args, fmt);
  int n = vsnprintf(NULL, 0, fmt, args);
  sb_expand_buffer(sb, sb->len + n + 1);
  vsnprintf(sb->items + sb->len, n + 1, fmt, args);

  va_end(args);

  sb->len += n;
}


#endif // YACC_SB_IMPLEMENTATION
