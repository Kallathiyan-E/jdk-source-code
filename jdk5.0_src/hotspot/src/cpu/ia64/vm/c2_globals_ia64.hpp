#ifdef USE_PRAGMA_IDENT_HDR
#pragma ident "@(#)c2_globals_ia64.hpp	1.13 04/03/02 02:08:55 JVM"
#endif
/*
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL.  Use is subject to license terms.
 */


//
// Sets the default values for platform dependent flags used by the server compiler.
// (see c2_globals.hpp).  Alpha-sorted.

define_pd_global(bool, BackgroundCompilation,        true);
define_pd_global(bool, CICompileOSR,                 true);
define_pd_global(bool, InlineIntrinsics,             false);
define_pd_global(bool, PreferInterpreterNativeStubs, true);
define_pd_global(bool, ProfileInterpreter,           false); // Not yet
define_pd_global(bool, ProfileTraps,                 true);
define_pd_global(bool, UseOnStackReplacement,        false); // Not yet
define_pd_global(bool, TieredCompilation,            false);
define_pd_global(intx, CompileThreshold,             10000);
define_pd_global(intx, CompileThresholdMax,          100000);
define_pd_global(intx, CompileThresholdMin,          2000);
define_pd_global(intx, Tier2CompileThreshold,        15000);
define_pd_global(intx, OnStackReplacePercentage,     140);
define_pd_global(intx, ConditionalMoveLimit,         4);
define_pd_global(intx, FLOATPRESSURE,                24);
define_pd_global(intx, FreqInlineSize,		     100);   // Borrowed from sparc
define_pd_global(intx, INTPRESSURE,                  32);
define_pd_global(intx, InteriorEntryAlignment,       16);    // Bundle alignment
define_pd_global(intx, NewRatio,		     2);
define_pd_global(intx, NewSizeThreadIncrease, ScaleForWordSize(4*K));
define_pd_global(intx, OptoLoopAlignment,            16);    // Bundle alignment
define_pd_global(intx, RegisterCostAreaRatio,	     12000);
define_pd_global(bool, UseTLAB,                      true);
define_pd_global(bool, ResizeTLAB,                   true);
define_pd_global(intx, AllocatePrefetchStyle,        2);
define_pd_global(intx, AllocatePrefetchDistance,     512);
define_pd_global(intx, LoopUnrollLimit,	     	     60);    // Borrowed from sparc

// Peephole and CISC spilling both break the graph, and so makes the
// scheduler sick.
define_pd_global(bool, OptoPeephole,                 false);
define_pd_global(bool, UseCISCSpill,                 false);
define_pd_global(bool, OptoBundling,                 false);
define_pd_global(bool, OptoScheduling,               true);
define_pd_global(intx, UseSSE,                       2);

#ifdef _LP64
// We need to make sure that all generated code is within 
// 2 gigs of the libjvm.so runtime routines so we can use
// the faster "call" instruction rather than the expensive
// sequence of instructions to load a 64 bit pointer. 
define_pd_global(intx, InitialCodeCacheSize,     1024*K);
define_pd_global(intx, ReservedCodeCacheSize,    1024*M);
define_pd_global(intx, CodeCacheExpansionSize,   64*K);
#else
define_pd_global(intx, InitialCodeCacheSize,     160*K);
define_pd_global(intx, ReservedCodeCacheSize,    32*M);
define_pd_global(intx, CodeCacheExpansionSize,   32*K);
#endif
define_pd_global(uintx,CodeCacheMinBlockLength,      4);
define_pd_global(uintx,PermSize,    ScaleForWordSize(16*M));
define_pd_global(uintx,MaxPermSize, ScaleForWordSize(64*M));

