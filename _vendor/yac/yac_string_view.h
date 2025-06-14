// The MIT License (MIT)
//
// Original work Copyright (C) 2025 Bilyi Vadym
//   :: https://github.com/belyivadim/cghost
// Modified work Copyright (C) 2025 Doccaico
//   :: https://github.com/doccaico/yac
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.


#ifndef YAC_STRING_VIEW_H_
#define YAC_STRING_VIEW_H_


#include <ctype.h> // isspace
#include <stdbool.h> // bool
#include <string.h> // strlen, strchr, strstr, strncmp, strnicmp(_strnicmp)

#ifndef YAC_STRING_VIEW_ASSERT
#include <assert.h> // assert
#define YAC_STRING_VIEW_ASSERT assert
#endif // YAC_STRING_VIEW_ASSERT

#ifndef YAC_STRING_VIEW_API
#ifdef YAC_STRING_VIEW_STATIC
#define YAC_STRING_VIEW_API static
#else
#define YAC_STRING_VIEW_API extern
#endif // YAC_STRING_VIEW_STATIC
#endif // YAC_STRING_VIEW_API


// DECLARATIONS

typedef struct {
    const char* begin;
    size_t len;
} YacStringView;

#define YacStringViewEmpty ((YacStringView){0})

#define YacStringViewFromCstr(str) ((YacStringView){.begin = str, .len = strlen(str)})

#define YacStringViewFromCstrSlice(str, offset, len_)            \
    ((YacStringView){.begin = (str) + (offset), .len = (len_)})

#define YacStringViewFromConstant(c)                         \
    ((YacStringView){.begin = (c), .len = sizeof((c)) - 1})

// Need #include "yac_dynamic_array.h" and "yac_string_builder.h"
#define YacStringViewFromStringView(sb) ((YacStringView){.begin = (sb).items, .len = (sb).len})

#define YacStringViewFarg "%.*s"

#define YacStringViewExpand(sv) (int)(sv).len, (sv).begin

#define YacStringViewSlice(sv, offset, len_)                    \
    (YacStringViewFromCstrSlice((sv).begin, (offset), (len_)))

#define YacStringViewAdvance(sv) (++(sv).begin, --(sv).len)

YAC_STRING_VIEW_API bool YacStringViewEquals(const YacStringView* lhs, const YacStringView* rhs);
YAC_STRING_VIEW_API bool YacStringViewEqualsIcase(const YacStringView* lhs, const YacStringView* rhs);

YAC_STRING_VIEW_API bool YacStringViewStartsWithCstr(const YacStringView* sv, const char* start);
YAC_STRING_VIEW_API bool YacStringViewStartsWithCstrIcase(const YacStringView* sv, const char* start);
YAC_STRING_VIEW_API bool YacStringViewEndsWithCstr(const YacStringView* sv, const char* end);
YAC_STRING_VIEW_API bool YacStringViewEndsWithCstrIcase(const YacStringView* sv, const char* end);

YAC_STRING_VIEW_API bool YacStringViewStartsWith(const YacStringView* sv, const YacStringView* start);
YAC_STRING_VIEW_API bool YacStringViewStartsWithIcase(const YacStringView* sv, const YacStringView* start);
YAC_STRING_VIEW_API bool YacStringViewEndsWith(const YacStringView* sv, const YacStringView* end);
YAC_STRING_VIEW_API bool YacStringViewEndsWithIcase(const YacStringView* sv, const YacStringView* end);
YAC_STRING_VIEW_API void YacStringViewStripL(YacStringView* sv);
YAC_STRING_VIEW_API void YacStringViewStripR(YacStringView* sv);
YAC_STRING_VIEW_API void YacStringViewStrip(YacStringView* sv);

YAC_STRING_VIEW_API int YacStringViewIndexOf(const YacStringView* sv, int rune);
YAC_STRING_VIEW_API int YacStringViewLastIndexOf(const YacStringView* sv, int rune);
YAC_STRING_VIEW_API int YacStringViewIndexOfCstr(const YacStringView* sv, const char* str);
YAC_STRING_VIEW_API YacStringView YacStringViewSplit(YacStringView* sv, const char* delim);
YAC_STRING_VIEW_API YacStringView YacStringViewSplitExcludeDelim(YacStringView* sv, const char* delim);

#endif // YAC_STRING_VIEW_H_


// IMPLEMENTATION

#ifdef YAC_STRING_VIEW_IMPLEMENTATION

#ifdef _MSC_VER
#define strncasecmp _strnicmp
#endif

YAC_STRING_VIEW_API bool YacStringViewEquals(const YacStringView* lhs, const YacStringView* rhs)
{
    YAC_STRING_VIEW_ASSERT(NULL != lhs);
    YAC_STRING_VIEW_ASSERT(NULL != rhs);

    return lhs->len == rhs->len && 0 == strncmp(lhs->begin, rhs->begin, lhs->len);
}

YAC_STRING_VIEW_API bool YacStringViewEqualsIcase(const YacStringView* lhs, const YacStringView* rhs)
{
    YAC_STRING_VIEW_ASSERT(NULL != lhs);
    YAC_STRING_VIEW_ASSERT(NULL != rhs);

    return lhs->len == rhs->len && 0 == strncasecmp(lhs->begin, rhs->begin, lhs->len);
}

YAC_STRING_VIEW_API bool YacStringViewStartsWithCstr(const YacStringView* sv, const char* start)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    size_t start_len = strlen(start);
    return sv->len >= start_len && 0 == strncmp(sv->begin, start, start_len);
}

YAC_STRING_VIEW_API bool YacStringViewStartsWithCstrIcase(const YacStringView* sv, const char* start)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    size_t start_len = strlen(start);
    return sv->len >= start_len && 0 == strncasecmp(sv->begin, start, start_len);
}

YAC_STRING_VIEW_API bool YacStringViewEndsWithCstr(const YacStringView* sv, const char* end)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    size_t end_len = strlen(end);
    return sv->len >= end_len && 0 == strncmp(sv->begin + sv->len - end_len, end, end_len);
}

YAC_STRING_VIEW_API bool YacStringViewEndsWithCstrIcase(const YacStringView* sv, const char* end)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    size_t end_len = strlen(end);
    return sv->len >= end_len && 0 == strncasecmp(sv->begin + sv->len - end_len, end, end_len);
}

YAC_STRING_VIEW_API bool YacStringViewStartsWith(const YacStringView* sv, const YacStringView* start)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);
    YAC_STRING_VIEW_ASSERT(NULL != start);

    return sv->len >= start->len && 0 == strncmp(sv->begin, start->begin, start->len);
}

YAC_STRING_VIEW_API bool YacStringViewStartsWithIcase(const YacStringView* sv, const YacStringView* start)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);
    YAC_STRING_VIEW_ASSERT(NULL != start);

    return sv->len >= start->len && 0 == strncasecmp(sv->begin, start->begin, start->len);
}

YAC_STRING_VIEW_API bool YacStringViewEndsWith(const YacStringView* sv, const YacStringView* end)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);
    YAC_STRING_VIEW_ASSERT(NULL != end);

    return sv->len >= end->len && 0 == strncmp(sv->begin + sv->len - end->len, end->begin, end->len);
}

YAC_STRING_VIEW_API bool YacStringViewEndsWithIcase(const YacStringView* sv, const YacStringView* end)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);
    YAC_STRING_VIEW_ASSERT(NULL != end);

    return sv->len >= end->len && 0 == strncasecmp(sv->begin + sv->len - end->len, end->begin, end->len);
}

YAC_STRING_VIEW_API void YacStringViewStripL(YacStringView* sv)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    while (sv->len > 0 && isspace(sv->begin[0])) {
        YacStringViewAdvance(*sv);
    }
}

YAC_STRING_VIEW_API void YacStringViewStripR(YacStringView* sv)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    while (sv->len > 0 && isspace(sv->begin[sv->len - 1])) {
        sv->len -= 1;
    }
}

YAC_STRING_VIEW_API void YacStringViewStrip(YacStringView* sv)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    YacStringViewStripL(sv);
    YacStringViewStripR(sv);
}

YAC_STRING_VIEW_API int YacStringViewIndexOf(const YacStringView* sv, int rune)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    const char* found = strchr(sv->begin, rune);
    return found == NULL ? -1 : (int)(found - sv->begin);
}

YAC_STRING_VIEW_API int YacStringViewLastIndexOf(const YacStringView* sv, int rune)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    const char* found = strrchr(sv->begin, rune);
    return found == NULL ? -1 : (int)(found - sv->begin);
}

YAC_STRING_VIEW_API int YacStringViewIndexOfCstr(const YacStringView* sv, const char* str)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    const char* found = strstr(sv->begin, str);
    return found == NULL ? -1 : (int)(found - sv->begin);
}

YAC_STRING_VIEW_API YacStringView YacStringViewSplit(YacStringView* sv, const char* delim)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    // returns YacStringView before the delim (not including)
    // sv becomes YacStringView from the delim (including) to the end of the given sv
    int index = YacStringViewIndexOfCstr(sv, delim);
    if (index < 0) {
        YacStringView result =* sv;
        sv->len = 0;
        return result;
    }

    YacStringView result = YacStringViewSlice(*sv, 0, index);
    *sv = YacStringViewSlice(*sv, index, sv->len - index);
    return result;
}

YAC_STRING_VIEW_API YacStringView YacStringViewSplitExcludeDelim(YacStringView* sv, const char* delim)
{
    YAC_STRING_VIEW_ASSERT(NULL != sv);

    int index = YacStringViewIndexOfCstr(sv, delim);
    if (index < 0) {
        YacStringView result =* sv;
        sv->len = 0;
        return result;
    }

    YacStringView result = YacStringViewSlice(*sv, 0, index);
    size_t delimlen = strlen(delim);
    *sv = YacStringViewSlice(*sv, index + delimlen, sv->len - (index + delimlen));
    return result;
}


#endif // YAC_STRING_VIEW_IMPLEMENTATION
