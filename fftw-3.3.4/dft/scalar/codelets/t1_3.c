/*
 * Copyright (c) 2003, 2007-14 Matteo Frigo
 * Copyright (c) 2003, 2007-14 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Tue Mar  4 13:45:51 EST 2014 */

#include "codelet-dft.h"

#ifdef HAVE_FMA

/* Generated by: ../../../genfft/gen_twiddle.native -fma -reorder-insns -schedule-for-pipeline -compact -variables 4 -pipeline-latency 4 -n 3 -name t1_3 -include t.h */

/*
 * This function contains 16 FP additions, 14 FP multiplications,
 * (or, 6 additions, 4 multiplications, 10 fused multiply/add),
 * 21 stack variables, 2 constants, and 12 memory accesses
 */
#include "t.h"

static void t1_3(R *ri, R *ii, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DK(KP866025403, +0.866025403784438646763723170752936183471402627);
     DK(KP500000000, +0.500000000000000000000000000000000000000000000);
     {
	  INT m;
	  for (m = mb, W = W + (mb * 4); m < me; m = m + 1, ri = ri + ms, ii = ii + ms, W = W + 4, MAKE_VOLATILE_STRIDE(6, rs)) {
	       E T1, Tm, T9, Tc, Tb, Th, T7, Ti, Ta, Tj, Td;
	       T1 = ri[0];
	       Tm = ii[0];
	       {
		    E T3, T6, T2, T5, Tg, T4, T8;
		    T3 = ri[WS(rs, 1)];
		    T6 = ii[WS(rs, 1)];
		    T2 = W[0];
		    T5 = W[1];
		    T9 = ri[WS(rs, 2)];
		    Tc = ii[WS(rs, 2)];
		    Tg = T2 * T6;
		    T4 = T2 * T3;
		    T8 = W[2];
		    Tb = W[3];
		    Th = FNMS(T5, T3, Tg);
		    T7 = FMA(T5, T6, T4);
		    Ti = T8 * Tc;
		    Ta = T8 * T9;
	       }
	       Tj = FNMS(Tb, T9, Ti);
	       Td = FMA(Tb, Tc, Ta);
	       {
		    E Tk, Te, To, Tn, Tl, Tf;
		    Tk = Th - Tj;
		    Tl = Th + Tj;
		    Te = T7 + Td;
		    To = Td - T7;
		    ii[0] = Tl + Tm;
		    Tn = FNMS(KP500000000, Tl, Tm);
		    ri[0] = T1 + Te;
		    Tf = FNMS(KP500000000, Te, T1);
		    ii[WS(rs, 1)] = FMA(KP866025403, To, Tn);
		    ii[WS(rs, 2)] = FNMS(KP866025403, To, Tn);
		    ri[WS(rs, 2)] = FNMS(KP866025403, Tk, Tf);
		    ri[WS(rs, 1)] = FMA(KP866025403, Tk, Tf);
	       }
	  }
     }
}

static const tw_instr twinstr[] = {
     {TW_FULL, 0, 3},
     {TW_NEXT, 1, 0}
};

static const ct_desc desc = { 3, "t1_3", twinstr, &GENUS, {6, 4, 10, 0}, 0, 0, 0 };

void X(codelet_t1_3) (planner *p) {
     X(kdft_dit_register) (p, t1_3, &desc);
}
#else				/* HAVE_FMA */

/* Generated by: ../../../genfft/gen_twiddle.native -compact -variables 4 -pipeline-latency 4 -n 3 -name t1_3 -include t.h */

/*
 * This function contains 16 FP additions, 12 FP multiplications,
 * (or, 10 additions, 6 multiplications, 6 fused multiply/add),
 * 15 stack variables, 2 constants, and 12 memory accesses
 */
#include "t.h"

static void t1_3(R *ri, R *ii, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DK(KP866025403, +0.866025403784438646763723170752936183471402627);
     DK(KP500000000, +0.500000000000000000000000000000000000000000000);
     {
	  INT m;
	  for (m = mb, W = W + (mb * 4); m < me; m = m + 1, ri = ri + ms, ii = ii + ms, W = W + 4, MAKE_VOLATILE_STRIDE(6, rs)) {
	       E T1, Ti, T6, Te, Tb, Tf, Tc, Th;
	       T1 = ri[0];
	       Ti = ii[0];
	       {
		    E T3, T5, T2, T4;
		    T3 = ri[WS(rs, 1)];
		    T5 = ii[WS(rs, 1)];
		    T2 = W[0];
		    T4 = W[1];
		    T6 = FMA(T2, T3, T4 * T5);
		    Te = FNMS(T4, T3, T2 * T5);
	       }
	       {
		    E T8, Ta, T7, T9;
		    T8 = ri[WS(rs, 2)];
		    Ta = ii[WS(rs, 2)];
		    T7 = W[2];
		    T9 = W[3];
		    Tb = FMA(T7, T8, T9 * Ta);
		    Tf = FNMS(T9, T8, T7 * Ta);
	       }
	       Tc = T6 + Tb;
	       Th = Te + Tf;
	       ri[0] = T1 + Tc;
	       ii[0] = Th + Ti;
	       {
		    E Td, Tg, Tj, Tk;
		    Td = FNMS(KP500000000, Tc, T1);
		    Tg = KP866025403 * (Te - Tf);
		    ri[WS(rs, 2)] = Td - Tg;
		    ri[WS(rs, 1)] = Td + Tg;
		    Tj = KP866025403 * (Tb - T6);
		    Tk = FNMS(KP500000000, Th, Ti);
		    ii[WS(rs, 1)] = Tj + Tk;
		    ii[WS(rs, 2)] = Tk - Tj;
	       }
	  }
     }
}

static const tw_instr twinstr[] = {
     {TW_FULL, 0, 3},
     {TW_NEXT, 1, 0}
};

static const ct_desc desc = { 3, "t1_3", twinstr, &GENUS, {10, 6, 6, 0}, 0, 0, 0 };

void X(codelet_t1_3) (planner *p) {
     X(kdft_dit_register) (p, t1_3, &desc);
}
#endif				/* HAVE_FMA */
