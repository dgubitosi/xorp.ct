/* -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*- */

/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software")
 * to deal in the Software without restriction, subject to the conditions
 * listed in the XORP LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the XORP LICENSE file; the license in that file is
 * legally binding.
 */

/*
 * $XORP: xorp/libxorp/utility.h,v 1.4 2003/11/06 01:21:38 pavlin Exp $
 */

#ifndef __LIBXORP_UTILITY_H__
#define __LIBXORP_UTILITY_H__

/*
 * Compile time assertion.
 */
#ifndef static_assert
#define static_assert(a) switch (a) case 0: case (a):
#endif /* static_assert */

/*
 * A macro to avoid compilation warnings about unused functions arguments.
 * XXX: this should be used only in C. In C++ just remove the argument name
 * in the function definition.
 */
#ifdef UNUSED
# undef UNUSED
#endif /* UNUSED */
#define UNUSED(var)	static_assert(sizeof(var) != 0)

#ifdef __cplusplus
#define cstring(s) (s).str().c_str()
#endif

#define ADD_POINTER(pointer, size, type)				\
	((type)(((uint8_t *)(pointer)) + (size)))

/*
 * Various ctype(3) wrappers that work properly even if the value of the int
 * argument is not representable as an unsigned char and doesn't have the
 * value of EOF.
 */
#ifdef __cplusplus
extern "C" {
#endif

extern int xorp_isalnum(int c);
extern int xorp_isalpha(int c);
extern int xorp_isblank(int c);
extern int xorp_iscntrl(int c);
extern int xorp_isdigit(int c);
extern int xorp_isgraph(int c);
extern int xorp_islower(int c);
extern int xorp_isprint(int c);
extern int xorp_ispunct(int c);
extern int xorp_isspace(int c);
extern int xorp_isupper(int c);
extern int xorp_isxdigit(int c);
extern int xorp_tolower(int c);
extern int xorp_toupper(int c);

#ifdef __cplusplus
}
#endif

#endif /* __LIBXORP_UTILITY_H__ */
