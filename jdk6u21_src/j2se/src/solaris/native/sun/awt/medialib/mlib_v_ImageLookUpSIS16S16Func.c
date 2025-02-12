/*
 * @(#)mlib_v_ImageLookUpSIS16S16Func.c	1.14 10/03/23
 *
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
  

#pragma ident	"@(#)mlib_v_ImageLookUpSIS16S16Func.c	1.10	02/04/15 SMI"

#include "vis_proto.h"
#include "mlib_image.h"
#include "mlib_v_ImageLookUpFunc.h"

/***************************************************************/
static void mlib_v_ImageLookUpSI_S16_S16_2_DstA8D1(const mlib_s16 *src,
                                                   mlib_s16       *dst,
                                                   mlib_s32       xsize,
                                                   const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S16_S16_2_D1(const mlib_s16 *src,
                                              mlib_s16       *dst,
                                              mlib_s32       xsize,
                                              const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S16_S16_3_D1(const mlib_s16 *src,
                                              mlib_s16       *dst,
                                              mlib_s32       xsize,
                                              const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S16_S16_4_DstOff0_D1(const mlib_s16 *src,
                                                      mlib_s16       *dst,
                                                      mlib_s32       xsize,
                                                      const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S16_S16_4_DstOff1_D1(const mlib_s16 *src,
                                                      mlib_s16       *dst,
                                                      mlib_s32       xsize,
                                                      const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S16_S16_4_DstOff2_D1(const mlib_s16 *src,
                                                      mlib_s16       *dst,
                                                      mlib_s32       xsize,
                                                      const mlib_s16 **table);

static void mlib_v_ImageLookUpSI_S16_S16_4_DstOff3_D1(const mlib_s16 *src,
                                                      mlib_s16       *dst,
                                                      mlib_s32       xsize,
                                                      const mlib_s16 **table);

/***************************************************************/
#define VIS_LD_U16_I(X, Y)      vis_ld_u16_i((void *)(X), (Y))

/***************************************************************/
void mlib_v_ImageLookUpSI_S16_S16_2_DstA8D1(const mlib_s16 *src,
                                            mlib_s16       *dst,
                                            mlib_s32       xsize,
                                            const mlib_s16 **table)
{
  mlib_s16 *sp;              /* pointer to source data */
  mlib_s32 s0, s1;           /* source data */
  mlib_s16 *dl;              /* pointer to start of destination */
  mlib_d64 *dp;              /* aligned pointer to destination */
  mlib_d64 t0, t1, t2;       /* destination data */
  mlib_d64 t3, acc;          /* destination data */
  mlib_s32 i;                /* loop variable */
  const mlib_s16 *tab0 = &table[0][32768];
  const mlib_s16 *tab1 = &table[1][32768];

  sp   = (void *)src;
  dl   = dst;
  dp   = (mlib_d64 *) dl;

  vis_alignaddr((void *) 0, 6);

  if (xsize >= 2) {

    s0 = (sp[0] << 1);
    s1 = (sp[1] << 1);
    sp += 2;

#pragma pipeloop(0)
    for(i = 0; i <= xsize - 4; i+=2, sp+=2) {
      t3 = VIS_LD_U16_I(tab1, s1);
      t2 = VIS_LD_U16_I(tab0, s1);
      t1 = VIS_LD_U16_I(tab1, s0);
      t0 = VIS_LD_U16_I(tab0, s0);
      acc = vis_faligndata(t3, acc);
      acc = vis_faligndata(t2, acc);
      acc = vis_faligndata(t1, acc);
      acc = vis_faligndata(t0, acc);
      s0 = (sp[0] << 1);
      s1 = (sp[1] << 1);
      *dp++ = acc;
    }

    t3 = VIS_LD_U16_I(tab1, s1);
    t2 = VIS_LD_U16_I(tab0, s1);
    t1 = VIS_LD_U16_I(tab1, s0);
    t0 = VIS_LD_U16_I(tab0, s0);
    acc = vis_faligndata(t3, acc);
    acc = vis_faligndata(t2, acc);
    acc = vis_faligndata(t1, acc);
    acc = vis_faligndata(t0, acc);
    *dp++ = acc;
  }

  if ((xsize & 1) != 0) {
    s0 = (sp[0] << 1);
    t1 = VIS_LD_U16_I(tab1, s0);
    t0 = VIS_LD_U16_I(tab0, s0);
    acc = vis_faligndata(t1, acc);
    acc = vis_faligndata(t0, acc);
    *(mlib_f32*)dp = vis_read_hi(acc);
  }
}

/***************************************************************/
void mlib_v_ImageLookUpSI_S16_S16_2_D1(const mlib_s16 *src,
                                       mlib_s16       *dst,
                                       mlib_s32       xsize,
                                       const mlib_s16 **table)
{
  mlib_s16 *sp;                /* pointer to source data */
  mlib_s32 s0, s1, s2;         /* source data */
  mlib_s16 *dl;                /* pointer to start of destination */
  mlib_d64 *dp;                /* aligned pointer to destination */
  mlib_d64 t0, t1, t2;         /* destination data */
  mlib_d64 t3, acc;            /* destination data */
  mlib_s32 i;                  /* loop variable */
  const mlib_s16 *tab0 = &table[0][32768];
  const mlib_s16 *tab1 = &table[1][32768];

  sp   = (void *)src;
  dl   = dst;

  vis_alignaddr((void *) 0, 6);

  s0 = *sp++;
  *dl++ = tab0[s0];
  dp   = (mlib_d64 *) dl;
  xsize--; s0 <<= 1;

  if (xsize >= 2) {

    s1 = (sp[0] << 1);
    s2 = (sp[1] << 1);
    sp += 2;

#pragma pipeloop(0)
    for(i = 0; i <= xsize - 4; i+=2, sp+=2) {
      t3 = VIS_LD_U16_I(tab0, s2);
      t2 = VIS_LD_U16_I(tab1, s1);
      t1 = VIS_LD_U16_I(tab0, s1);
      t0 = VIS_LD_U16_I(tab1, s0);
      acc = vis_faligndata(t3, acc);
      acc = vis_faligndata(t2, acc);
      acc = vis_faligndata(t1, acc);
      acc = vis_faligndata(t0, acc);
      s0 = s2;
      s1 = (sp[0] << 1);
      s2 = (sp[1] << 1);
      *dp++ = acc;
    }

    t3 = VIS_LD_U16_I(tab0, s2);
    t2 = VIS_LD_U16_I(tab1, s1);
    t1 = VIS_LD_U16_I(tab0, s1);
    t0 = VIS_LD_U16_I(tab1, s0);
    acc = vis_faligndata(t3, acc);
    acc = vis_faligndata(t2, acc);
    acc = vis_faligndata(t1, acc);
    acc = vis_faligndata(t0, acc);
    s0 = s2;
    *dp++ = acc;
  }

  dl = (mlib_s16*)dp;

  if ((xsize & 1) != 0) {
    s1 = (sp[0] << 1);
    t1 = VIS_LD_U16_I(tab0, s1);
    t0 = VIS_LD_U16_I(tab1, s0);
    acc = vis_faligndata(t1, acc);
    acc = vis_faligndata(t0, acc);
    *(mlib_f32*)dp = vis_read_hi(acc);
    s0 = s1; dl += 2;
  }

  s0 >>= 1;
  *dl = tab1[s0];
}

/***************************************************************/
void mlib_v_ImageLookUpSI_S16_S16_2(const mlib_s16 *src,
                                    mlib_s32       slb,
                                    mlib_s16       *dst,
                                    mlib_s32       dlb,
                                    mlib_s32       xsize,
                                    mlib_s32       ysize,
                                    const mlib_s16 **table)
{
  mlib_s16 *sl;
  mlib_s16 *dl;
  mlib_s32 j;
  const mlib_s16 *tab0 = &table[0][32768];
  const mlib_s16 *tab1 = &table[1][32768];

  sl = (void *)src;
  dl = dst;

  /* row loop */
  for (j = 0; j < ysize; j ++) {
    mlib_s16 *sp = sl;
    mlib_s16 *dp = dl;
    mlib_s32 off, s0, size = xsize;

    off = ((8 - ((mlib_addr)dp & 7)) & 7);

    if ((off >= 4) && (size > 0)) {
      s0 = *sp++;
      *dp++ = tab0[s0];
      *dp++ = tab1[s0];
      size--;
    }

    if (size > 0) {

      if (((mlib_addr)dp & 7) == 0) {
        mlib_v_ImageLookUpSI_S16_S16_2_DstA8D1(sp, dp, size, table);
      } else {
        mlib_v_ImageLookUpSI_S16_S16_2_D1(sp, dp, size, table);
      }
    }

    sl = (mlib_s16 *) ((mlib_u8 *) sl + slb);
    dl = (mlib_s16 *) ((mlib_u8 *) dl + dlb);
  }
}

/***************************************************************/
void mlib_v_ImageLookUpSI_S16_S16_3_D1(const mlib_s16 *src,
                                       mlib_s16       *dst,
                                       mlib_s32       xsize,
                                       const mlib_s16 **table)
{
  mlib_s16 *sp;              /* pointer to source data */
  mlib_s16 *dl;              /* pointer to start of destination */
  mlib_d64 *dp;              /* aligned pointer to destination */
  mlib_d64 t0, t1, t2, t3;   /* destination data */
  mlib_d64 acc0, acc1, acc2; /* destination data */
  mlib_s32 i;                /* loop variable */
  const mlib_s16 *tab0 = &table[0][32768];
  const mlib_s16 *tab1 = &table[1][32768];
  const mlib_s16 *tab2 = &table[2][32768];
  mlib_s32 s00, s01, s02, s03;

  sp   = (void *)src;
  dl   = dst;
  dp   = (mlib_d64 *) dl;

  vis_alignaddr((void *) 0, 6);

  i = 0;

  if (xsize >= 4) {

    s00 = (sp[0] << 1);
    s01 = (sp[1] << 1);
    s02 = (sp[2] << 1);
    s03 = (sp[3] << 1);
    sp += 4;

#pragma pipeloop(0)
    for(i = 0; i <= xsize - 8; i+=4, sp+=4) {
      t3 = VIS_LD_U16_I(tab0, s01);
      t2 = VIS_LD_U16_I(tab2, s00);
      t1 = VIS_LD_U16_I(tab1, s00);
      t0 = VIS_LD_U16_I(tab0, s00);
      acc0 = vis_faligndata(t3, acc0);
      acc0 = vis_faligndata(t2, acc0);
      acc0 = vis_faligndata(t1, acc0);
      acc0 = vis_faligndata(t0, acc0);
      t3 = VIS_LD_U16_I(tab1, s02);
      t2 = VIS_LD_U16_I(tab0, s02);
      t1 = VIS_LD_U16_I(tab2, s01);
      t0 = VIS_LD_U16_I(tab1, s01);
      acc1 = vis_faligndata(t3, acc1);
      acc1 = vis_faligndata(t2, acc1);
      acc1 = vis_faligndata(t1, acc1);
      acc1 = vis_faligndata(t0, acc1);
      t3 = VIS_LD_U16_I(tab2, s03);
      t2 = VIS_LD_U16_I(tab1, s03);
      t1 = VIS_LD_U16_I(tab0, s03);
      t0 = VIS_LD_U16_I(tab2, s02);
      acc2 = vis_faligndata(t3, acc2);
      acc2 = vis_faligndata(t2, acc2);
      acc2 = vis_faligndata(t1, acc2);
      acc2 = vis_faligndata(t0, acc2);
      s00 = (sp[0] << 1);
      s01 = (sp[1] << 1);
      s02 = (sp[2] << 1);
      s03 = (sp[3] << 1);
      *dp++ = acc0;
      *dp++ = acc1;
      *dp++ = acc2;
    }

    t3 = VIS_LD_U16_I(tab0, s01);
    t2 = VIS_LD_U16_I(tab2, s00);
    t1 = VIS_LD_U16_I(tab1, s00);
    t0 = VIS_LD_U16_I(tab0, s00);
    acc0 = vis_faligndata(t3, acc0);
    acc0 = vis_faligndata(t2, acc0);
    acc0 = vis_faligndata(t1, acc0);
    acc0 = vis_faligndata(t0, acc0);
    t3 = VIS_LD_U16_I(tab1, s02);
    t2 = VIS_LD_U16_I(tab0, s02);
    t1 = VIS_LD_U16_I(tab2, s01);
    t0 = VIS_LD_U16_I(tab1, s01);
    acc1 = vis_faligndata(t3, acc1);
    acc1 = vis_faligndata(t2, acc1);
    acc1 = vis_faligndata(t1, acc1);
    acc1 = vis_faligndata(t0, acc1);
    t3 = VIS_LD_U16_I(tab2, s03);
    t2 = VIS_LD_U16_I(tab1, s03);
    t1 = VIS_LD_U16_I(tab0, s03);
    t0 = VIS_LD_U16_I(tab2, s02);
    acc2 = vis_faligndata(t3, acc2);
    acc2 = vis_faligndata(t2, acc2);
    acc2 = vis_faligndata(t1, acc2);
    acc2 = vis_faligndata(t0, acc2);
    *dp++ = acc0;
    *dp++ = acc1;
    *dp++ = acc2;
    i += 4;
  }

  dl = (mlib_s16*)dp;

#pragma pipeloop(0)
  for (; i < xsize; i++) {
    s00 = sp[0];
    dl[0] = tab0[s00];
    dl[1] = tab1[s00];
    dl[2] = tab2[s00];
    dl += 3; sp ++;
  }
}

/***************************************************************/
void mlib_v_ImageLookUpSI_S16_S16_3(const mlib_s16 *src,
                                    mlib_s32       slb,
                                    mlib_s16       *dst,
                                    mlib_s32       dlb,
                                    mlib_s32       xsize,
                                    mlib_s32       ysize,
                                    const mlib_s16 **table)
{
  mlib_s16  *sl;
  mlib_s16 *dl;
  mlib_s32 i, j;
  const mlib_s16 *tab0 = &table[0][32768];
  const mlib_s16 *tab1 = &table[1][32768];
  const mlib_s16 *tab2 = &table[2][32768];

  sl = (void *)src;
  dl = dst;

  /* row loop */
  for (j = 0; j < ysize; j ++) {
    mlib_s16 *sp = sl;
    mlib_s16*dp = dl;
    mlib_s32 off, s0, size = xsize;

    off = ((mlib_addr)dp & 7) >> 1;
    off = (off < size) ? off : size;

    for (i = 0; i < off; i++) {
      s0 = *sp++;
      *dp++ = tab0[s0];
      *dp++ = tab1[s0];
      *dp++ = tab2[s0];
      size--;
    }

    if (size > 0) {
      mlib_v_ImageLookUpSI_S16_S16_3_D1(sp, dp, size, table);
    }

    sl = (mlib_s16 *) ((mlib_u8 *) sl + slb);
    dl = (mlib_s16 *) ((mlib_u8 *) dl + dlb);
  }
}

/***************************************************************/
void mlib_v_ImageLookUpSI_S16_S16_4_DstOff0_D1(const mlib_s16 *src,
                                               mlib_s16       *dst,
                                               mlib_s32       xsize,
                                               const mlib_s16 **table)
{
  mlib_s16 *sp;              /* pointer to source data */
  mlib_s32 s0;               /* source data */
  mlib_s16 *dl;              /* pointer to start of destination */
  mlib_d64 *dp;              /* aligned pointer to destination */
  mlib_d64 t0, t1, t2, t3;   /* destination data */
  mlib_d64 acc;              /* destination data */
  mlib_s32 i;                /* loop variable */
  const mlib_s16 *tab0 = &table[0][32768];
  const mlib_s16 *tab1 = &table[1][32768];
  const mlib_s16 *tab2 = &table[2][32768];
  const mlib_s16 *tab3 = &table[3][32768];

  sp   = (void *)src;
  dl   = dst;
  dp   = (mlib_d64 *) dl;

  vis_alignaddr((void *) 0, 6);

  if (xsize >= 1) {

    s0 = (*sp++) << 1;

#pragma pipeloop(0)
    for(i = 0; i <= xsize - 2; i++) {
      t3 = VIS_LD_U16_I(tab3, s0);
      t2 = VIS_LD_U16_I(tab2, s0);
      t1 = VIS_LD_U16_I(tab1, s0);
      t0 = VIS_LD_U16_I(tab0, s0);
      acc = vis_faligndata(t3, acc);
      acc = vis_faligndata(t2, acc);
      acc = vis_faligndata(t1, acc);
      acc = vis_faligndata(t0, acc);
      s0 = (*sp++) << 1;
      *dp++ = acc;
    }

    t3 = VIS_LD_U16_I(tab3, s0);
    t2 = VIS_LD_U16_I(tab2, s0);
    t1 = VIS_LD_U16_I(tab1, s0);
    t0 = VIS_LD_U16_I(tab0, s0);
    acc = vis_faligndata(t3, acc);
    acc = vis_faligndata(t2, acc);
    acc = vis_faligndata(t1, acc);
    acc = vis_faligndata(t0, acc);
    *dp++ = acc;
  }
}

/***************************************************************/
void mlib_v_ImageLookUpSI_S16_S16_4_DstOff1_D1(const mlib_s16 *src,
                                               mlib_s16       *dst,
                                               mlib_s32       xsize,
                                               const mlib_s16 **table)
{
  mlib_s16 *sp;              /* pointer to source data */
  mlib_s32 s0, s1;           /* source data */
  mlib_s16 *dl;              /* pointer to start of destination */
  mlib_d64 *dp;              /* aligned pointer to destination */
  mlib_d64 t0, t1, t2, t3;   /* destination data */
  mlib_d64 acc;              /* destination data */
  mlib_s32 i;                /* loop variable */
  const mlib_s16 *tab0 = &table[0][32768];
  const mlib_s16 *tab1 = &table[1][32768];
  const mlib_s16 *tab2 = &table[2][32768];
  const mlib_s16 *tab3 = &table[3][32768];

  sp   = (void *)src;
  dl   = dst;
  dp   = (mlib_d64 *) dl;

  vis_alignaddr((void *) 0, 6);

  s0 = (*sp++) << 1;

  if (xsize >= 1) {

    s1 = (*sp++) << 1;

#pragma pipeloop(0)
    for(i = 0; i <= xsize - 2; i++) {
      t3 = VIS_LD_U16_I(tab0, s1);
      t2 = VIS_LD_U16_I(tab3, s0);
      t1 = VIS_LD_U16_I(tab2, s0);
      t0 = VIS_LD_U16_I(tab1, s0);
      acc = vis_faligndata(t3, acc);
      acc = vis_faligndata(t2, acc);
      acc = vis_faligndata(t1, acc);
      acc = vis_faligndata(t0, acc);
      s0 = s1;
      s1 = (*sp++) << 1;
      *dp++ = acc;
    }

    t3 = VIS_LD_U16_I(tab0, s1);
    t2 = VIS_LD_U16_I(tab3, s0);
    t1 = VIS_LD_U16_I(tab2, s0);
    t0 = VIS_LD_U16_I(tab1, s0);
    acc = vis_faligndata(t3, acc);
    acc = vis_faligndata(t2, acc);
    acc = vis_faligndata(t1, acc);
    acc = vis_faligndata(t0, acc);
    s0 = s1;
    *dp++ = acc;
  }

  dl = (mlib_s16*)dp;
  s0 >>= 1;

  dl[0] = tab1[s0];
  dl[1] = tab2[s0];
  dl[2] = tab3[s0];
}

/***************************************************************/
void mlib_v_ImageLookUpSI_S16_S16_4_DstOff2_D1(const mlib_s16 *src,
                                               mlib_s16       *dst,
                                               mlib_s32       xsize,
                                               const mlib_s16 **table)
{
  mlib_s16 *sp;              /* pointer to source data */
  mlib_s32 s0, s1;           /* source data */
  mlib_s16 *dl;              /* pointer to start of destination */
  mlib_d64 *dp;              /* aligned pointer to destination */
  mlib_d64 t0, t1, t2, t3;   /* destination data */
  mlib_d64 acc;              /* destination data */
  mlib_s32 i;                /* loop variable */
  const mlib_s16 *tab0 = &table[0][32768];
  const mlib_s16 *tab1 = &table[1][32768];
  const mlib_s16 *tab2 = &table[2][32768];
  const mlib_s16 *tab3 = &table[3][32768];

  sp   = (void *)src;
  dl   = dst;
  dp   = (mlib_d64 *) dl;

  vis_alignaddr((void *) 0, 6);

  s0 = (*sp++) << 1;

  if (xsize >= 1) {

    s1 = (*sp++) << 1;

#pragma pipeloop(0)
    for(i = 0; i <= xsize - 2; i++) {
      t3 = VIS_LD_U16_I(tab1, s1);
      t2 = VIS_LD_U16_I(tab0, s1);
      t1 = VIS_LD_U16_I(tab3, s0);
      t0 = VIS_LD_U16_I(tab2, s0);
      acc = vis_faligndata(t3, acc);
      acc = vis_faligndata(t2, acc);
      acc = vis_faligndata(t1, acc);
      acc = vis_faligndata(t0, acc);
      s0 = s1;
      s1 = (*sp++) << 1;
      *dp++ = acc;
    }

    t3 = VIS_LD_U16_I(tab1, s1);
    t2 = VIS_LD_U16_I(tab0, s1);
    t1 = VIS_LD_U16_I(tab3, s0);
    t0 = VIS_LD_U16_I(tab2, s0);
    acc = vis_faligndata(t3, acc);
    acc = vis_faligndata(t2, acc);
    acc = vis_faligndata(t1, acc);
    acc = vis_faligndata(t0, acc);
    s0 = s1;
    *dp++ = acc;
  }

  dl = (mlib_s16*)dp;
  s0 >>= 1;

  dl[0] = tab2[s0];
  dl[1] = tab3[s0];
}

/***************************************************************/
void mlib_v_ImageLookUpSI_S16_S16_4_DstOff3_D1(const mlib_s16 *src,
                                               mlib_s16       *dst,
                                               mlib_s32       xsize,
                                               const mlib_s16 **table)
{
  mlib_s16 *sp;              /* pointer to source data */
  mlib_s32 s0, s1;           /* source data */
  mlib_s16 *dl;              /* pointer to start of destination */
  mlib_d64 *dp;              /* aligned pointer to destination */
  mlib_d64 t0, t1, t2, t3;   /* destination data */
  mlib_d64 acc;              /* destination data */
  mlib_s32 i;                /* loop variable */
  const mlib_s16 *tab0 = &table[0][32768];
  const mlib_s16 *tab1 = &table[1][32768];
  const mlib_s16 *tab2 = &table[2][32768];
  const mlib_s16 *tab3 = &table[3][32768];

  sp   = (void *)src;
  dl   = dst;
  dp   = (mlib_d64 *) dl;

  vis_alignaddr((void *) 0, 6);

  s0 = (*sp++) << 1;

  if (xsize >= 1) {

    s1 = (*sp++) << 1;

#pragma pipeloop(0)
    for(i = 0; i <= xsize - 2; i++) {
      t3 = VIS_LD_U16_I(tab2, s1);
      t2 = VIS_LD_U16_I(tab1, s1);
      t1 = VIS_LD_U16_I(tab0, s1);
      t0 = VIS_LD_U16_I(tab3, s0);
      acc = vis_faligndata(t3, acc);
      acc = vis_faligndata(t2, acc);
      acc = vis_faligndata(t1, acc);
      acc = vis_faligndata(t0, acc);
      s0 = s1;
      s1 = (*sp++) << 1;
      *dp++ = acc;
    }

    t3 = VIS_LD_U16_I(tab2, s1);
    t2 = VIS_LD_U16_I(tab1, s1);
    t1 = VIS_LD_U16_I(tab0, s1);
    t0 = VIS_LD_U16_I(tab3, s0);
    acc = vis_faligndata(t3, acc);
    acc = vis_faligndata(t2, acc);
    acc = vis_faligndata(t1, acc);
    acc = vis_faligndata(t0, acc);
    s0 = s1;
    *dp++ = acc;
  }

  dl = (mlib_s16*)dp;
  s0 >>= 1;

  dl[0] = tab3[s0];
}

/***************************************************************/
void mlib_v_ImageLookUpSI_S16_S16_4(const mlib_s16 *src,
                                    mlib_s32       slb,
                                    mlib_s16       *dst,
                                    mlib_s32       dlb,
                                    mlib_s32       xsize,
                                    mlib_s32       ysize,
                                    const mlib_s16 **table)
{
  mlib_s16 *sl;
  mlib_s16 *dl;
  mlib_s32 j;
  const mlib_s16 *tab0 = &table[0][32768];
  const mlib_s16 *tab1 = &table[1][32768];
  const mlib_s16 *tab2 = &table[2][32768];

  sl = (void *)src;
  dl = dst;

  /* row loop */
  for (j = 0; j < ysize; j ++) {
    mlib_s16 *sp = sl;
    mlib_s16 *dp = dl;
    mlib_s32 off, s0, size = xsize;

    if (size > 0) {
      off =  ((8 - ((mlib_addr)dp & 7)) & 7) >> 1;

      if (off == 0) {
        mlib_v_ImageLookUpSI_S16_S16_4_DstOff0_D1(sp, dp, size, table);
      } else if (off == 1) {
        s0 = *sp;
        *dp++ = tab0[s0];
        size--;
        mlib_v_ImageLookUpSI_S16_S16_4_DstOff1_D1(sp, dp, size, table);
      } else if (off == 2) {
        s0 = *sp;
        *dp++ = tab0[s0];
        *dp++ = tab1[s0];
        size--;
        mlib_v_ImageLookUpSI_S16_S16_4_DstOff2_D1(sp, dp, size, table);
      } else if (off == 3) {
        s0 = *sp;
        *dp++ = tab0[s0];
        *dp++ = tab1[s0];
        *dp++ = tab2[s0];
        size--;
        mlib_v_ImageLookUpSI_S16_S16_4_DstOff3_D1(sp, dp, size, table);
      }
    }

    sl = (mlib_s16 *) ((mlib_u8 *) sl + slb);
    dl = (mlib_s16 *) ((mlib_u8 *) dl + dlb);
  }
}

/***************************************************************/
