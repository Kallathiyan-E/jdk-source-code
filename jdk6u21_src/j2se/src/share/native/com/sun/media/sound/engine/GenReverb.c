/*
 * @(#)GenReverb.c	1.20 10/03/23
 *
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
/*****************************************************************************/
/*
** "GenReverb.c"
**
**	Generalized Music Synthesis package. Part of SoundMusicSys.
**
**
**
** Modification History:
**
**	1/18/96		Spruced up for C++ extra error checking
**				Changed the macro 'THE_CHECK' to accept a type for typecasting the source pointer
**	3/1/96		Removed extra PV_DoCallBack, and PV_GetWavePitch
**	4/21/96		Put test around ReverbBuffer being NULL
**	12/30/96	Changed copyright
**	1/23/97		Added WebTV small footprint reverb unit, and support for PV_PostFilterStereo
**				code
**				Fixed REVERB_TYPE_6 to match be the same amount of spring for all output
**				rates
**	6/4/97		Added USE_SMALL_MEMORY_REVERB tests around code to disable when this
**				flag is used
**	2/3/98		Renamed songBufferLeftMono to songBufferDry
**	2/11/98		Added support for Q_48K & Q_24K
**	2/20/98		kcr		interrcept most fixed reverbs to the new variable reverb
**	2/22/98		Disabled the use of the PV_PostFilterStereo filter. It sounds bad
**	2/23/98		Removed last of old variable reverb code and filter (PV_PostFilterStereo)
**				Removed GM_InitReverbTaps & GM_GetReverbTaps & GM_SetReverbTaps
**	2/24/98		Removed redundant parameter setting in new reverb -- also now call
**				CheckReverbType() to clear out reverb buffers when switching type
**	3/2/98		Added some wrappers with USE_NEW_EFFECTS around some code
**	3/16/98		Rearranged how the verbs are setup/cleaned up and run. Added the missing
**				REVERB_TYPE_2 is the fixed case. Moved in from GenSetup.c GM_SetupReverb &
**				GM_CleanupReverb.
**				Added GM_ProcessReverb & GM_GetReverbEnableThreshold.
**				Moved GM_SetReverbType & GM_GetReverbType from GenSetup.c
**	3/24/98		Added a version of PV_RunMonoFixedReverb that does nothing for
**				no mono code support
**	6/26/98		Added GM_IsReverbFixed
**	7/7/98		Removed reverbIsVariable
**	7/30/98		Modified the verb (Lab and cavern) to be smaller when running at 48k to
**				avoid clicks
**	8/12/98		Modified GM_SetReverbType to walk through active voices and reset
**				various verb settings
**	11/9/98		Renamed NoteDur to voiceMode
**	2002-03-14	$$fb removed compiler warnings
*/
/*****************************************************************************/
#include "GenSnd.h"
#include "GenPriv.h"

#if USE_MONO_OUTPUT == TRUE
static void PV_RunMonoFixedReverb(ReverbMode which)
{
    register INT32		b, c, bz, cz;
    register INT32		*sourceLR, *sourceReverb;
    register INT32		*reverbBuf;
    register LOOPCOUNT	a = 0;
    register INT32		reverbPtr1, reverbPtr2, reverbPtr3, reverbPtr4;

    reverbBuf = &MusicGlobals->reverbBuffer[0];
    if (reverbBuf)
	{
	    sourceLR = &MusicGlobals->songBufferDry[0];
	    sourceReverb = &MusicGlobals->songBufferReverb[0];
	
	    b = MusicGlobals->LPfilterL;
	    c = MusicGlobals->LPfilterR;
	    bz = MusicGlobals->LPfilterLz;
	    cz = MusicGlobals->LPfilterRz;
	    reverbPtr1 = MusicGlobals->reverbPtr;

	    // NOTE: do not exceed a tap distance of 2730.  2730*6 is almost the full buffer size.
	    switch (which)
		{
		    // ----------
		case REVERB_TYPE_2:		// Igor's Closet
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 1;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 2;
			    break;
			case Q_44K:
			    a = 4;
			    break;
			case Q_48K:
			    a = 5;
			    break;
			}
		    reverbPtr2 = (MusicGlobals->reverbPtr - 632*a) & REVERB_BUFFER_MASK; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 450*a) & REVERB_BUFFER_MASK;
		    reverbPtr4 = (MusicGlobals->reverbPtr - 798*a) & REVERB_BUFFER_MASK;

		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b -= b >> 2;
			    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
			    reverbBuf[reverbPtr1] = *sourceLR + (b >> 1);
			    *sourceLR += b >> 2;
			    sourceLR++;
								
			    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK;
			}
		    break;
		    // ----------
		case REVERB_TYPE_3:		// Igor's Garage
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 1;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 2;
			    break;
			case Q_44K:
			    a = 4;
			    break;
			case Q_48K:
			    a = 5;
			    break;
			}
		    reverbPtr2 = (MusicGlobals->reverbPtr - 632*a) & REVERB_BUFFER_MASK; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 430*a) & REVERB_BUFFER_MASK;
		    reverbPtr4 = (MusicGlobals->reverbPtr - 798*a) & REVERB_BUFFER_MASK;
		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b -= b >> 2;
			    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
			    reverbBuf[reverbPtr1] = *sourceLR + (b >> 1);
			    *sourceLR += b >> 1;
			    sourceLR++;
					
			    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK;
			}
		    break;
		    // ----------
		case REVERB_TYPE_4:		// Igor's Acoustic Lab
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 1;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 2;
			    break;
			case Q_44K:
			    a = 4;
			    break;
			case Q_48K:
			    a = 5;
			    break;
			}
		    reverbPtr2 = (MusicGlobals->reverbPtr - 1100*a) & REVERB_BUFFER_MASK; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 1473*a) & REVERB_BUFFER_MASK;
		    reverbPtr4 = (MusicGlobals->reverbPtr - 1711*a) & REVERB_BUFFER_MASK;
		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b -= (bz + b) >> 2;
			    bz = b;
			    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
			    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 2);
			    *sourceLR = *sourceLR +  (b >> 1);
			    sourceLR++;
					
					
			    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK;
			}
		    break;
		    // ----------
		case REVERB_TYPE_5:		// Igor's Dungeon
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 1;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 2;
			    break;
			case Q_44K:
			    a = 4;
			    break;
			case Q_48K:
			    a = 5;
			    break;
			}
		    reverbPtr2 = (MusicGlobals->reverbPtr - 500*a) & REVERB_BUFFER_MASK; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 674*a) & REVERB_BUFFER_MASK;
		    reverbPtr4 = (MusicGlobals->reverbPtr - 1174*a) & REVERB_BUFFER_MASK;
		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b -= b >> 1;
			    b = (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 2;
			    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 2);
			    *sourceLR = (*sourceLR + (b << 2)) >> 1;
			    sourceLR++;
	
			    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK;
			}
		    break;
		    // ----------
		case REVERB_TYPE_6:		// Igor's Cavern
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 2;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 4;
			    break;
			case Q_44K:
			    a = 8;
			    break;
			case Q_48K:
			    a = 9;
			    break;
			}
		    reverbPtr2 = (MusicGlobals->reverbPtr - 2700/2*a) & REVERB_BUFFER_MASK; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 3250/2*a) & REVERB_BUFFER_MASK;
		    reverbPtr4 = (MusicGlobals->reverbPtr - 4095/2*a) & REVERB_BUFFER_MASK;
		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b += ((reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 4) - (b >> 2);
			    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 8);
			    *sourceLR = *sourceLR + b;
			    sourceLR++;
	
			    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK;
			    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK;
			}
		    break;
		    // ----------
		case REVERB_TYPE_7:		// webtv
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 1;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 2;
			    break;
			case Q_44K:
			    a = 4;
			    break;
			case Q_48K:
			    a = 5;
			    break;
			}
		    reverbPtr1 = MusicGlobals->reverbPtr & REVERB_BUFFER_MASK_SMALL;
		    reverbPtr2 = (MusicGlobals->reverbPtr - 1100*a) & REVERB_BUFFER_MASK_SMALL; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 1473*a) & REVERB_BUFFER_MASK_SMALL; 
		    reverbPtr4 = (MusicGlobals->reverbPtr - 1711*a) & REVERB_BUFFER_MASK_SMALL;
		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b -= (c + b) >> 2;
			    c = b;
			    b += reverbBuf[reverbPtr2] >> 3;
			    b += reverbBuf[reverbPtr3] >> 3;
			    b += reverbBuf[reverbPtr4] >> 3;
			    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 2);
			    *sourceLR = *sourceLR +  (b >> 1) - (b >> 3);
			    sourceLR++;
							
			    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK_SMALL;
			    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK_SMALL;
			    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK_SMALL;
			    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK_SMALL;
			}
		    break;
		}
	    MusicGlobals->LPfilterL = b;
	    MusicGlobals->LPfilterLz = bz;
	    MusicGlobals->LPfilterR = c;
	    MusicGlobals->LPfilterRz = cz;
	    MusicGlobals->reverbPtr = reverbPtr1;
	}
}
#else
static void PV_RunMonoFixedReverb(ReverbMode which)
{
    which = which;
}
#endif	// USE_MONO_OUTPUT

static void PV_RunStereoFixedReverb(ReverbMode which)
{
    register INT32		b, c, bz, cz;
    register INT32		*sourceLR;
    register INT32		*reverbBuf;
    register LOOPCOUNT	a = 0;
    register INT32		reverbPtr1, reverbPtr2, reverbPtr3, reverbPtr4;

    reverbBuf = &MusicGlobals->reverbBuffer[0];
    if (reverbBuf)
	{
	    sourceLR = &MusicGlobals->songBufferDry[0];
	
	    b = MusicGlobals->LPfilterL;
	    c = MusicGlobals->LPfilterR;
	    bz = MusicGlobals->LPfilterLz;
	    cz = MusicGlobals->LPfilterRz;
	    reverbPtr1 = MusicGlobals->reverbPtr;


	    // NOTE: do not exceed a tap distance of 2047.  2047*8 is almost the full buffer size.
	    switch (which)
		{
		    // ----------
		case REVERB_TYPE_2:	// was called closet -- really just early reflections
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 2;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 4;
			    break;
			case Q_44K:
			    a = 8;
			    break;
			case Q_48K:
			    a = 9;
			    break;
			}
		    reverbPtr2 = (MusicGlobals->reverbPtr - 632*a) & REVERB_BUFFER_MASK; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 450*a) & REVERB_BUFFER_MASK;
		    reverbPtr4 = (MusicGlobals->reverbPtr - 798*a) & REVERB_BUFFER_MASK;

		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b -= b >> 2;
			    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
			    reverbBuf[reverbPtr1] = *sourceLR + (b >> 1);
			    *sourceLR += (b + c) >> 2;
			    sourceLR++;
					
			    c -= c >> 2;
			    c += (reverbBuf[reverbPtr2+1] + reverbBuf[reverbPtr3+1] + reverbBuf[reverbPtr4+1]) >> 3;
			    reverbBuf[reverbPtr1+1] = *sourceLR + (c >> 1);
			    *sourceLR += (c + b) >> 2;
			    sourceLR++;
					
			    reverbPtr1 = (reverbPtr1 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr2 = (reverbPtr2 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr3 = (reverbPtr3 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr4 = (reverbPtr4 + 2) & REVERB_BUFFER_MASK;
			}
		    break;
								
		    // ----------
		case REVERB_TYPE_3:		// Igor's Garage
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 2;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 4;
			    break;
			case Q_44K:
			    a = 8;
			    break;
			case Q_48K:
			    a = 9;
			    break;
			}
		    reverbPtr2 = (MusicGlobals->reverbPtr - 632*a) & REVERB_BUFFER_MASK; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 430*a) & REVERB_BUFFER_MASK;
		    reverbPtr4 = (MusicGlobals->reverbPtr - 798*a) & REVERB_BUFFER_MASK;

		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b -= b >> 2;
			    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
			    reverbBuf[reverbPtr1] = *sourceLR + (b >> 1);
			    *sourceLR += (b + c) >> 1;
			    sourceLR++;
					
			    c -= c >> 2;
			    c += (reverbBuf[reverbPtr2+1] + reverbBuf[reverbPtr3+1] + reverbBuf[reverbPtr4+1]) >> 3;
			    reverbBuf[reverbPtr1+1] = *sourceLR + (c >> 1);
			    *sourceLR += (c + b) >> 1;
			    sourceLR++;
					
			    reverbPtr1 = (reverbPtr1 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr2 = (reverbPtr2 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr3 = (reverbPtr3 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr4 = (reverbPtr4 + 2) & REVERB_BUFFER_MASK;
			}
		    break;
		    // ----------
		case REVERB_TYPE_4:		// Igor's Acoustic Lab
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 2;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 4;
			    break;
			case Q_44K:
			case Q_48K:
			    a = 8;
			    break;
			}
		    reverbPtr2 = (MusicGlobals->reverbPtr - 1100*a) & REVERB_BUFFER_MASK; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 1473*a) & REVERB_BUFFER_MASK;
		    reverbPtr4 = (MusicGlobals->reverbPtr - 1711*a) & REVERB_BUFFER_MASK;
		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b -= (bz + b) >> 2;
			    bz = b;
			    b += (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 3;
			    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 2);
			    *sourceLR = *sourceLR +  ((c + b) >> 2);
			    sourceLR++;
					
			    c -= (cz + c) >> 2;
			    cz = c;
			    c += (reverbBuf[reverbPtr2+1] + reverbBuf[reverbPtr3+1] + reverbBuf[reverbPtr4+1]) >> 3;
			    reverbBuf[reverbPtr1+1] = *sourceLR + c - (c >> 2);
			    *sourceLR = *sourceLR + ((c + b) >> 2);
			    sourceLR++;
					
			    reverbPtr1 = (reverbPtr1 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr2 = (reverbPtr2 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr3 = (reverbPtr3 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr4 = (reverbPtr4 + 2) & REVERB_BUFFER_MASK;
			}
		    break;
		    // ----------
		case REVERB_TYPE_5:		// Igor's Dungeon
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 2;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 4;
			    break;
			case Q_44K:
			    a = 8;
			    break;
			case Q_48K:
			    a = 9;
			    break;
			}
		    reverbPtr2 = (MusicGlobals->reverbPtr - 500*a) & REVERB_BUFFER_MASK; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 674*a) & REVERB_BUFFER_MASK;
		    reverbPtr4 = (MusicGlobals->reverbPtr - 1174*a) & REVERB_BUFFER_MASK;
		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b -= b >> 1;
			    b = (reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 2;
			    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 2);
			    *sourceLR = *sourceLR + b;
			    sourceLR++;
					
			    c -= c >> 1;
			    c = (reverbBuf[reverbPtr2+1] + reverbBuf[reverbPtr3+1] + reverbBuf[reverbPtr4+1]) >> 2;
			    reverbBuf[reverbPtr1+1] = *sourceLR + c - (c >> 2);
			    *sourceLR = *sourceLR + c;
			    sourceLR++;
					
			    reverbPtr1 = (reverbPtr1 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr2 = (reverbPtr2 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr3 = (reverbPtr3 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr4 = (reverbPtr4 + 2) & REVERB_BUFFER_MASK;
			}
		    break;
		    // ---------- 
		case REVERB_TYPE_6:		// Igor's Cavern
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 2;
			    break;
			case Q_24K:
			case Q_22K_TERP_44K:
			case Q_22K:
			    a = 4;
			    break;
			case Q_44K:
			case Q_48K:
			    a = 8;
			    break;
			}
		    reverbPtr2 = (MusicGlobals->reverbPtr - 2700*a) & REVERB_BUFFER_MASK; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 3250*a) & REVERB_BUFFER_MASK;
		    reverbPtr4 = (MusicGlobals->reverbPtr - 4095*a) & REVERB_BUFFER_MASK;
		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b += ((reverbBuf[reverbPtr2] + reverbBuf[reverbPtr3] + reverbBuf[reverbPtr4]) >> 4) - (b >> 2);
			    reverbBuf[reverbPtr1] = *sourceLR + b - (b >> 8);
			    *sourceLR = *sourceLR + b + c;
			    sourceLR++;
					
			    c += ((reverbBuf[reverbPtr2+1] + reverbBuf[reverbPtr3+1] + reverbBuf[reverbPtr4+1]) >> 4) - (c >> 2);
			    reverbBuf[reverbPtr1+1] = *sourceLR + c - (c >> 8);
			    *sourceLR = *sourceLR + c + b;
			    sourceLR++;
					
			    reverbPtr1 = (reverbPtr1 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr2 = (reverbPtr2 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr3 = (reverbPtr3 + 2) & REVERB_BUFFER_MASK;
			    reverbPtr4 = (reverbPtr4 + 2) & REVERB_BUFFER_MASK;
			}
		    break;
		    // ----------
		case REVERB_TYPE_7:		// webtv
		    switch (MusicGlobals->outputQuality)
			{
			case Q_8K:
			case Q_11K_TERP_22K:
			case Q_11K:
			    a = 1;
			    break;
			case Q_22K_TERP_44K:
			case Q_22K:
			case Q_24K:
			    a = 2;
			    break;
			case Q_44K:
			    a = 4;
			    break;
			case Q_48K:
			    a = 5;
			    break;					
			}
		    reverbPtr1 = MusicGlobals->reverbPtr & REVERB_BUFFER_MASK_SMALL;
		    reverbPtr2 = (MusicGlobals->reverbPtr - 1100*a) & REVERB_BUFFER_MASK_SMALL; 
		    reverbPtr3 = (MusicGlobals->reverbPtr - 1473*a) & REVERB_BUFFER_MASK_SMALL; 
		    reverbPtr4 = (MusicGlobals->reverbPtr - 1711*a) & REVERB_BUFFER_MASK_SMALL;

		    for (a = MusicGlobals->One_Loop; a > 0; --a)
			{
			    b -= (c + b) >> 2;
			    c = b;
			    b += reverbBuf[reverbPtr2] >> 3;
			    b += reverbBuf[reverbPtr3] >> 3;
			    b += reverbBuf[reverbPtr4] >> 3;
			    reverbBuf[reverbPtr1] = *sourceLR + sourceLR[1] + b - (b >> 2);
			    *sourceLR = *sourceLR +  (b >> 1) - (b >> 3);
			    sourceLR++;
			    *sourceLR = *sourceLR +  (b >> 1) - (b >> 3);
			    sourceLR++;
							
			    reverbPtr1 = (reverbPtr1 + 1) & REVERB_BUFFER_MASK_SMALL;
			    reverbPtr2 = (reverbPtr2 + 1) & REVERB_BUFFER_MASK_SMALL;
			    reverbPtr3 = (reverbPtr3 + 1) & REVERB_BUFFER_MASK_SMALL;
			    reverbPtr4 = (reverbPtr4 + 1) & REVERB_BUFFER_MASK_SMALL;
			}
		    break;
		}
	    MusicGlobals->LPfilterL = b;
	    MusicGlobals->LPfilterLz = bz;
	    MusicGlobals->LPfilterR = c;
	    MusicGlobals->LPfilterRz = cz;
	    MusicGlobals->reverbPtr = reverbPtr1;
	}	
}


#if USE_NEW_EFFECTS == TRUE
static void PV_RunStereoNewReverb(ReverbMode which)
{
    which = which;
    CheckReverbType();
    RunNewReverb(MusicGlobals->songBufferReverb, MusicGlobals->songBufferDry, MusicGlobals->One_Loop);
}
#endif



// This table must not be in ROM, because the function pointers are set at runtime.
// This table matches the ReverbMode index but the first two ReverbMode's
// are empty. If you add more verb types, add an entry into this table
static GM_ReverbConfigure verbTypes[MAX_REVERB_TYPES] = 
{
    {	// dead
	REVERB_NO_CHANGE,
	127,
	TRUE,							// fixed
	0,
	NULL,
	NULL
    },
    {	// none - dead
	REVERB_TYPE_1,
	127,
	TRUE,							// fixed
	0,
	NULL,
	NULL
    },
    {	// Igor's Closet
	REVERB_TYPE_2,
	REVERB_CONTROLER_THRESHOLD,
	TRUE,							// fixed
	REVERB_BUFFER_SIZE * 2L * (INT32)sizeof(INT32),
	PV_RunMonoFixedReverb,
	PV_RunStereoFixedReverb
    },
    {	// Igor's Garage
	REVERB_TYPE_3,
	REVERB_CONTROLER_THRESHOLD,
	TRUE,							// fixed
	REVERB_BUFFER_SIZE * 2L * (INT32)sizeof(INT32),
	PV_RunMonoFixedReverb,
	PV_RunStereoFixedReverb
    },
    {	// Igor's Acoustic Lab
	REVERB_TYPE_4,
	REVERB_CONTROLER_THRESHOLD,
	TRUE,							// fixed
	REVERB_BUFFER_SIZE * 2L * (INT32)sizeof(INT32),
	PV_RunMonoFixedReverb,
	PV_RunStereoFixedReverb
    },
    {	// Igor's Dungeon
	REVERB_TYPE_5,
	REVERB_CONTROLER_THRESHOLD,
	TRUE,							// fixed
	REVERB_BUFFER_SIZE * 2L * (INT32)sizeof(INT32),
	PV_RunMonoFixedReverb,
	PV_RunStereoFixedReverb
    },
    {	// Igor's Cavern
	REVERB_TYPE_6,
	REVERB_CONTROLER_THRESHOLD,
	TRUE,							// fixed
	REVERB_BUFFER_SIZE * 2L * (INT32)sizeof(INT32),
	PV_RunMonoFixedReverb,
	PV_RunStereoFixedReverb
    },
    {	// Small reflections Reverb used for WebTV
	REVERB_TYPE_7,
	REVERB_CONTROLER_THRESHOLD,
	TRUE,							// fixed
	REVERB_BUFFER_SIZE_SMALL * 2L * (INT32)sizeof(INT32),
	PV_RunMonoFixedReverb,
	PV_RunStereoFixedReverb
    },
#if USE_NEW_EFFECTS == TRUE
    // if we're using the cool new verbs, set them up
    {	// Early reflections (variable verb)
	REVERB_TYPE_8,
	0,
	FALSE,							// fixed
	(kNumberOfDiffusionStages * kDiffusionBufferFrameSize * (INT32)sizeof(INT32)) +
	((INT32)sizeof(INT32) * kStereoizerBufferFrameSize * 2) + 
	((kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize) * (INT32)sizeof(INT32)),
	NULL,
	PV_RunStereoNewReverb
    },
    {	// Basement (variable verb)
	REVERB_TYPE_9,
	0,
	FALSE,							// fixed
	(kNumberOfDiffusionStages * kDiffusionBufferFrameSize * (INT32)sizeof(INT32)) +
	((INT32)sizeof(INT32) * kStereoizerBufferFrameSize * 2) + 
	((kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize) * (INT32)sizeof(INT32)),
	NULL,
	PV_RunStereoNewReverb
    },
    {	// Banquet hall (variable verb)
	REVERB_TYPE_10,
	0,
	FALSE,							// fixed
	(kNumberOfDiffusionStages * kDiffusionBufferFrameSize * (INT32)sizeof(INT32)) +
	((INT32)sizeof(INT32) * kStereoizerBufferFrameSize * 2) + 
	((kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize) * (INT32)sizeof(INT32)),
	NULL,
	PV_RunStereoNewReverb
    },
    {	// Catacombs (variable verb)
	REVERB_TYPE_11,
	0,
	FALSE,							// fixed
	(kNumberOfDiffusionStages * kDiffusionBufferFrameSize * (INT32)sizeof(INT32)) +
	((INT32)sizeof(INT32) * kStereoizerBufferFrameSize * 2) + 
	((kCombBufferFrameSize*kNumberOfCombFilters + kEarlyReflectionBufferFrameSize) * (INT32)sizeof(INT32)),
	NULL,
	PV_RunStereoNewReverb
    }
#else
    // else we match them to our exisiting verbs
    {	// Igor's Closet
	REVERB_TYPE_2,
	REVERB_CONTROLER_THRESHOLD,
	TRUE,							// fixed
	REVERB_BUFFER_SIZE * 2L * (INT32)sizeof(INT32),
	PV_RunMonoFixedReverb,
	PV_RunStereoFixedReverb
    },
    {	// Igor's Garage
	REVERB_TYPE_3,
	REVERB_CONTROLER_THRESHOLD,
	TRUE,							// fixed
	REVERB_BUFFER_SIZE * 2L * (INT32)sizeof(INT32),
	PV_RunMonoFixedReverb,
	PV_RunStereoFixedReverb
    },
    {	// Igor's Acoustic Lab
	REVERB_TYPE_4,
	REVERB_CONTROLER_THRESHOLD,
	TRUE,							// fixed
	REVERB_BUFFER_SIZE * 2L * (INT32)sizeof(INT32),
	PV_RunMonoFixedReverb,
	PV_RunStereoFixedReverb
    },
    {	// Igor's Dungeon
	REVERB_TYPE_5,
	REVERB_CONTROLER_THRESHOLD,
	TRUE,							// fixed
	REVERB_BUFFER_SIZE * 2L * (INT32)sizeof(INT32),
	PV_RunMonoFixedReverb,
	PV_RunStereoFixedReverb
    },
#endif
};

#define MAX_VERB_CONFIG_ENTRIES		(INT32)(sizeof (verbTypes) / sizeof(GM_ReverbConfigure))

// private function to allocate and setup the fixed verb types
static XBOOL PV_SetupFixedReverb(void)
{
    GM_Mixer	*pMixer;
    INT32		size;

    pMixer = MusicGlobals;

    // clean fixed verb filter status
    pMixer->LPfilterL = 0;
    pMixer->LPfilterR = 0;
    pMixer->LPfilterLz = 0;
    pMixer->LPfilterRz = 0;

    size = REVERB_BUFFER_SIZE * 2L * (INT32)sizeof(INT32);
    pMixer->reverbBuffer = (INT32 *)XNewPtr(size);
    if (pMixer->reverbBuffer == NULL)
	{
	    // if this failed, try to allocate the smaller verb entry
	    size = REVERB_BUFFER_SIZE_SMALL * 2L * (INT32)sizeof(INT32);
	    pMixer->reverbBuffer = (INT32 *)XNewPtr(size);
	    if (pMixer->reverbBuffer == NULL)
		{
		    size = 0;	// no verb
		}
	}
    pMixer->reverbBufferSize = size;
    return (size) ? TRUE : FALSE;
}

// private function to deallocate and cleanup the fixed verb types
static void PV_CleanupFixedReverb(void)
{
    XPTR	verb1;

    if (MusicGlobals)
	{
	    // capture all pointers and set them to zero so that the interrupt services
	    // will stop then deallocate the memory
	    verb1 = MusicGlobals->reverbBuffer;
	    if (verb1)
		{
		    MusicGlobals->reverbPtr = 0;
		    GM_SetReverbType(REVERB_TYPE_1);		// no reverb
		    MusicGlobals->reverbBuffer = NULL;
		    XDisposePtr(verb1);
		}
	}
}

void GM_ProcessReverb(void)
{
    GM_ReverbProc	pVerbProc;
    ReverbMode		type;

    if (MusicGlobals->reverbBuffer)
	{
	    pVerbProc = NULL;
	    type = MusicGlobals->reverbUnitType;
	    switch (type)
		{
		default:
		    type = REVERB_TYPE_1;	// none;
		    break;
			
		    // valid table entries
		case REVERB_TYPE_2:			// Igor's Closet
		case REVERB_TYPE_3:			// Igor's Garage
		case REVERB_TYPE_4:			// Igor's Acoustic Lab
		case REVERB_TYPE_5:			// Igor's Cavern
		case REVERB_TYPE_6:			// Igor's Dungeon
		case REVERB_TYPE_7:			// Small reflections Reverb used for WebTV
		case REVERB_TYPE_8:			// Early reflections (variable verb)
		case REVERB_TYPE_9:			// Basement (variable verb)
		case REVERB_TYPE_10:		// Banquet hall (variable verb)
		case REVERB_TYPE_11:		// Catacombs (variable verb)
		    break;
		}
	    if (type != REVERB_TYPE_1)
		{
		    if (verbTypes[(int) type].globalReverbUsageSize <= MusicGlobals->reverbBufferSize)
			{
			    if (MusicGlobals->generateStereoOutput)
				{
				    pVerbProc = verbTypes[(int) type].pStereoRuntimeProc;
				}
			    else
				{
				    pVerbProc = verbTypes[(int) type].pMonoRuntimeProc;
				}
			    if (pVerbProc)
				{
				    (*pVerbProc)(verbTypes[(int) type].type);
				}
			}
		}
	}
}

// Reverb setup and configure. This will cleanup the exisiting verb if enabled
void GM_SetupReverb(void)
{
    GM_Mixer	*pMixer;

    pMixer = MusicGlobals;
    if (pMixer)
	{
	    GM_CleanupReverb();

	    pMixer->reverbUnitType = REVERB_NO_CHANGE;
	    pMixer->reverbTypeAllocated = REVERB_NO_CHANGE;
	    pMixer->reverbBufferSize = 0;

	    // since we can only allocate memory in this function because its not being called during 
	    // an interrupt, we need to walk through all the verb types 

	    if (PV_SetupFixedReverb())	// try to allocate the fixed verb first)
		{
#if USE_NEW_EFFECTS == TRUE
		    if (InitNewReverb())
			{
			    InitChorus();		
			}
#endif
		}
	}
}

void GM_CleanupReverb(void)
{
    if (MusicGlobals)
	{
	    PV_CleanupFixedReverb();

#if USE_NEW_EFFECTS
	    ShutdownNewReverb();
	    ShutdownChorus();

	    // these effects will be fully integrated later...
	    //ShutdownDelay();
	    //ShutdownGraphicEq();
	    //ShutdownParametricEq();
	    //ShutdownResonantFilter();
#endif
	}
}

// get highest MIDI verb amount required to activate verb
UBYTE GM_GetReverbEnableThreshold(void)
{
    UBYTE	thres;

    thres = MAX_NOTE_VOLUME;
    if (MusicGlobals)
	{
	    if (MusicGlobals->reverbBuffer)
		{
		    thres = verbTypes[(int) MusicGlobals->reverbUnitType].thresholdEnableValue;
		}
	}
    return thres;
}

// Is current reverb fixed (old style)?
XBOOL GM_IsReverbFixed(void)
{
    UBYTE	fixed;

    fixed = TRUE;
    if (MusicGlobals)
	{
	    if (MusicGlobals->reverbBuffer)
		{
		    fixed = verbTypes[(int) MusicGlobals->reverbUnitType].isFixed;
		}
	}
    return fixed;
}

// Set the global reverb type. This can happen at interrupt time, so don't allocate any memory
void GM_SetReverbType(ReverbMode reverbMode)
{
    XBOOL	changed;

    changed = FALSE;
    if (MusicGlobals)
	{
	    if (MusicGlobals->reverbBuffer)
		{
		    switch (reverbMode)
			{
			case REVERB_NO_CHANGE:	// no change
			default:
			    break;
				// valid types
			case REVERB_TYPE_1:
			case REVERB_TYPE_2:
			case REVERB_TYPE_3:
			case REVERB_TYPE_4:
			case REVERB_TYPE_5:
			case REVERB_TYPE_6:
			case REVERB_TYPE_7:
			case REVERB_TYPE_8:
			case REVERB_TYPE_9:
			case REVERB_TYPE_10:
			case REVERB_TYPE_11:
			    MusicGlobals->reverbUnitType = reverbMode;
#if USE_NEW_EFFECTS == TRUE
			    CheckReverbType();
#endif
			    changed = TRUE;
			    break;
			}
		}

	    // now walk through all active voices and reset various reverb controls
	    if (changed)
		{
		    register LOOPCOUNT count;
		    register GM_Voice *pVoice;

		    for (count = 0; count < MusicGlobals->MaxNotes + MusicGlobals->MaxEffects; count++)
			{
			    pVoice = &MusicGlobals->NoteEntry[count];
			    if (pVoice->voiceMode != VOICE_UNUSED)	// active?
				{
				    if (pVoice->pSong)		// active song voice
					{
					    if (pVoice->pInstrument)
						{
						    pVoice->avoidReverb = pVoice->pInstrument->avoidReverb;	// use instrument default. in case instrument designer
						}
					    else
						{
						    pVoice->avoidReverb = FALSE;
						}
					    pVoice->reverbLevel = pVoice->pSong->channelReverb[(int) pVoice->NoteChannel];	// set current verb level
					    pVoice->chorusLevel = (INT16)PV_ModifyVelocityFromCurve(pVoice->pSong, 
												    pVoice->pSong->channelChorus[(int) pVoice->NoteChannel]);
					    // wants no verb enabled
					    if (GM_IsReverbFixed())
						{
						    // if the instrument defines reverb on or the channel has reverb on, then enable it.
						    // if the channel is off, but the instrument defines reverb then enable it
						    if (pVoice->pSong->channelReverb[(int) pVoice->NoteChannel] < GM_GetReverbEnableThreshold())
							{
							    pVoice->avoidReverb = TRUE;		// force off
							}
						    if (pVoice->avoidReverb)
							{
							    pVoice->reverbLevel = 0;
							    pVoice->chorusLevel = 0;
							}
						}
					}
				}
			}			
		}
	}
}

// Return the current verb type
ReverbMode GM_GetReverbType(void)
{
    ReverbMode reverbMode;

    reverbMode = REVERB_TYPE_1;
    if (MusicGlobals)
	{
	    reverbMode = (ReverbMode)MusicGlobals->reverbUnitType;
	}
    return reverbMode;
}

// EOF of GenReverb.c
