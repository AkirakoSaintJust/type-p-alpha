/* SPDX-License-Identifier: GPL-3.0-only */
/* The generated firmware C uses GCC attributes for section placement and
 * always-inline control, which have no meaning in an MSVC Windows build, so the
 * plugin build makes them empty there. The firmware build keeps the real
 * attributes. */
#pragma once

#if defined(_MSC_VER)
#ifndef __attribute__
#define __attribute__(x)
#endif
#endif
