#ifdef USE_PRAGMA_IDENT_SRC
#pragma ident "@(#)debug_amd64.cpp	1.2 03/12/23 16:35:44 JVM"
#endif
/*
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL.  Use is subject to license terms.
 */

#include "incls/_precompiled.incl"
#include "incls/_debug_amd64.cpp.incl"

void pd_ps(frame f) {}

// This function is used to add platform specific info
// to the error reporting code.

void pd_obfuscate_location(char* buf, int buflen) {}
