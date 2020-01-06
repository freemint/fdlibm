/* @(#)k_rem_pio2.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * __kernel_rem_pio2(x,y,e0,nx,prec,ipio2)
 * double x[],y[]; int e0,nx,prec; int ipio2[];
 *
 * __kernel_rem_pio2 return the last three digits of N with
 *		y = x - N*pi/2
 * so that |y| < pi/2.
 *
 * The method is to compute the integer (mod 8) and fraction parts of
 * (2/pi)*x without doing the full multiplication. In general we
 * skip the part of the product that are known to be a huge integer (
 * more accurately, = 0 mod 8 ). Thus the number of operations are
 * independent of the exponent of the input.
 *
 * (2/pi) is represented by an array of 24-bit integers in ipio2[].
 *
 * Input parameters:
 * 	x[]	The input value (must be positive) is broken into nx
 *		pieces of 24-bit integers in double precision format.
 *		x[i] will be the i-th 24 bit of x. The scaled exponent
 *		of x[0] is given in input parameter e0 (i.e., x[0]*2^e0
 *		match x's up to 24 bits.
 *
 *		Example of breaking a double positive z into x[0]+x[1]+x[2]:
 *			e0 = ilogb(z)-23
 *			z  = scalbn(z,-e0)
 *		for i = 0,1,2
 *			x[i] = floor(z)
 *			z    = (z-x[i])*2**24
 *
 *
 *	y[]	ouput result in an array of double precision numbers.
 *		The dimension of y[] is:
 *			24-bit  precision	1
 *			53-bit  precision	2
 *			64-bit  precision	2
 *			113-bit precision	3
 *		The actual value is the sum of them. Thus for 113-bit
 *		precision, one may have to do something like:
 *
 *		long double t,w,r_head, r_tail;
 *		t = (long double)y[2] + (long double)y[1];
 *		w = (long double)y[0];
 *		r_head = t+w;
 *		r_tail = w - (r_head - t);
 *
 *	e0	The exponent of x[0]
 *
 *	nx	dimension of x[]
 *
 *  	prec	an integer indicating the precision:
 *			0	24  bits (single)
 *			1	53  bits (double)
 *			2	64  bits (extended)
 *			3	113 bits (quad)
 *
 *	ipio2[]
 *		integer array, contains the (24*i)-th to (24*i+23)-th
 *		bit of 2/pi after binary point. The corresponding
 *		floating value is
 *
 *			ipio2[i] * 2^(-24(i+1)).
 *
 * External function:
 *	double scalbn(), floor();
 *
 *
 * Here is the description of some local variables:
 *
 * 	jk	jk+1 is the initial number of terms of ipio2[] needed
 *		in the computation. The recommended value is 2,3,4,
 *		6 for single, double, extended,and quad.
 *
 * 	jz	local integer variable indicating the number of
 *		terms of ipio2[] used.
 *
 *	jx	nx - 1
 *
 *	jv	index for pointing to the suitable ipio2[] for the
 *		computation. In general, we want
 *			( 2^e0*x[0] * ipio2[jv-1]*2^(-24jv) )/8
 *		is an integer. Thus
 *			e0-3-24*jv >= 0 or (e0-3)/24 >= jv
 *		Hence jv = max(0,(e0-3)/24).
 *
 *	jp	jp+1 is the number of terms in PIo2[] needed, jp = jk.
 *
 * 	q[]	double array with integral value, representing the
 *		24-bits chunk of the product of x and 2/pi.
 *
 *	q0	the corresponding exponent of q[0]. Note that the
 *		exponent for q[i] would be q0-24*i.
 *
 *	PIo2[]	double precision array, obtained by cutting pi/2
 *		into 24 bits chunks.
 *
 *	f[]	ipio2[] in floating point
 *
 *	iq[]	integer array by breaking up q[] in 24-bits chunk.
 *
 *	fq[]	final product of x*(2/pi) in fq[0],..,fq[jk]
 *
 *	ih	integer. If >0 it indicates q[] is >= 0.5, hence
 *		it also indicates the *sign* of the result.
 *
 */


#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif


int32_t __kernel_rem_pio2l(double *x, double *y, int32_t e0, int32_t nx, int prec)
{
	int32_t jz, jx, jv, jp, jk, carry, n, iq[20], i, j, k, m, q0, ih;
	double z, fw, f[20], fq[20], q[20];

	static const int init_jk[] = { 2, 3, 4, 6 };	/* initial value for jk */
	
	/*
	 * Constants:
	 * The hexadecimal values are the intended ones for the following
	 * constants. The decimal values may be used, provided that the
	 * compiler will convert from decimal to binary accurately enough
	 * to produce the hexadecimal values shown.
	 */
	
	static const double PIo2[] = {
		1.57079625129699707031e+00,			/* 0x3FF921FB, 0x40000000 */
		7.54978941586159635335e-08,			/* 0x3E74442D, 0x00000000 */
		5.39030252995776476554e-15,			/* 0x3CF84698, 0x80000000 */
		3.28200341580791294123e-22,			/* 0x3B78CC51, 0x60000000 */
		1.27065575308067607349e-29,			/* 0x39F01B83, 0x80000000 */
		1.22933308981111328932e-36,			/* 0x387A2520, 0x40000000 */
		2.73370053816464559624e-44,			/* 0x36E38222, 0x80000000 */
		2.16741683877804819444e-51			/* 0x3569F31D, 0x00000000 */
	};
	
	/*  Table of constants for 2/pi, 5628 hexadecimal digits of 2/pi.  */
	static const int32_t two_over_pi[] = {
		IC(0xa2f983), IC(0x6e4e44), IC(0x1529fc), IC(0x2757d1), IC(0xf534dd), IC(0xc0db62),
		IC(0x95993c), IC(0x439041), IC(0xfe5163), IC(0xabdebb), IC(0xc561b7), IC(0x246e3a),
		IC(0x424dd2), IC(0xe00649), IC(0x2eea09), IC(0xd1921c), IC(0xfe1deb), IC(0x1cb129),
		IC(0xa73ee8), IC(0x8235f5), IC(0x2ebb44), IC(0x84e99c), IC(0x7026b4), IC(0x5f7e41),
		IC(0x3991d6), IC(0x398353), IC(0x39f49c), IC(0x845f8b), IC(0xbdf928), IC(0x3b1ff8),
		IC(0x97ffde), IC(0x05980f), IC(0xef2f11), IC(0x8b5a0a), IC(0x6d1f6d), IC(0x367ecf),
		IC(0x27cb09), IC(0xb74f46), IC(0x3f669e), IC(0x5fea2d), IC(0x7527ba), IC(0xc7ebe5),
		IC(0xf17b3d), IC(0x0739f7), IC(0x8a5292), IC(0xea6bfb), IC(0x5fb11f), IC(0x8d5d08),
		IC(0x560330), IC(0x46fc7b), IC(0x6babf0), IC(0xcfbc20), IC(0x9af436), IC(0x1da9e3),
		IC(0x91615e), IC(0xe61b08), IC(0x659985), IC(0x5f14a0), IC(0x68408d), IC(0xffd880),
		IC(0x4d7327), IC(0x310606), IC(0x1556ca), IC(0x73a8c9), IC(0x60e27b), IC(0xc08c6b),
		IC(0x47c419), IC(0xc367cd), IC(0xdce809), IC(0x2a8359), IC(0xc4768b), IC(0x961ca6),
		IC(0xddaf44), IC(0xd15719), IC(0x053ea5), IC(0xff0705), IC(0x3f7e33), IC(0xe832c2),
		IC(0xde4f98), IC(0x327dbb), IC(0xc33d26), IC(0xef6b1e), IC(0x5ef89f), IC(0x3a1f35),
		IC(0xcaf27f), IC(0x1d87f1), IC(0x21907c), IC(0x7c246a), IC(0xfa6ed5), IC(0x772d30),
		IC(0x433b15), IC(0xc614b5), IC(0x9d19c3), IC(0xc2c4ad), IC(0x414d2c), IC(0x5d000c),
		IC(0x467d86), IC(0x2d71e3), IC(0x9ac69b), IC(0x006233), IC(0x7cd2b4), IC(0x97a7b4),
		IC(0xd55537), IC(0xf63ed7), IC(0x1810a3), IC(0xfc764d), IC(0x2a9d64), IC(0xabd770),
		IC(0xf87c63), IC(0x57b07a), IC(0xe71517), IC(0x5649c0), IC(0xd9d63b), IC(0x3884a7),
		IC(0xcb2324), IC(0x778ad6), IC(0x23545a), IC(0xb91f00), IC(0x1b0af1), IC(0xdfce19),
		IC(0xff319f), IC(0x6a1e66), IC(0x615799), IC(0x47fbac), IC(0xd87f7e), IC(0xb76522),
		IC(0x89e832), IC(0x60bfe6), IC(0xcdc4ef), IC(0x09366c), IC(0xd43f5d), IC(0xd7de16),
		IC(0xde3b58), IC(0x929bde), IC(0x2822d2), IC(0xe88628), IC(0x4d58e2), IC(0x32cac6),
		IC(0x16e308), IC(0xcb7de0), IC(0x50c017), IC(0xa71df3), IC(0x5be018), IC(0x34132e),
		IC(0x621283), IC(0x014883), IC(0x5b8ef5), IC(0x7fb0ad), IC(0xf2e91e), IC(0x434a48),
		IC(0xd36710), IC(0xd8ddaa), IC(0x425fae), IC(0xce616a), IC(0xa4280a), IC(0xb499d3),
		IC(0xf2a606), IC(0x7f775c), IC(0x83c2a3), IC(0x883c61), IC(0x78738a), IC(0x5a8caf),
		IC(0xbdd76f), IC(0x63a62d), IC(0xcbbff4), IC(0xef818d), IC(0x67c126), IC(0x45ca55),
		IC(0x36d9ca), IC(0xd2a828), IC(0x8d61c2), IC(0x77c912), IC(0x142604), IC(0x9b4612),
		IC(0xc459c4), IC(0x44c5c8), IC(0x91b24d), IC(0xf31700), IC(0xad43d4), IC(0xe54929),
		IC(0x10d5fd), IC(0xfcbe00), IC(0xcc941e), IC(0xeece70), IC(0xf53e13), IC(0x80f1ec),
		IC(0xc3e7b3), IC(0x28f8c7), IC(0x940593), IC(0x3e71c1), IC(0xb3092e), IC(0xf3450b),
		IC(0x9c1288), IC(0x7b20ab), IC(0x9fb52e), IC(0xc29247), IC(0x2f327b), IC(0x6d550c),
		IC(0x90a772), IC(0x1fe76b), IC(0x96cb31), IC(0x4a1679), IC(0xe27941), IC(0x89dff4),
		IC(0x9794e8), IC(0x84e6e2), IC(0x973199), IC(0x6bed88), IC(0x365f5f), IC(0x0efdbb),
		IC(0xb49a48), IC(0x6ca467), IC(0x427271), IC(0x325d8d), IC(0xb8159f), IC(0x09e5bc),
		IC(0x25318d), IC(0x3974f7), IC(0x1c0530), IC(0x010c0d), IC(0x68084b), IC(0x58ee2c),
		IC(0x90aa47), IC(0x02e774), IC(0x24d6bd), IC(0xa67df7), IC(0x72486e), IC(0xef169f),
		IC(0xa6948e), IC(0xf691b4), IC(0x5153d1), IC(0xf20acf), IC(0x339820), IC(0x7e4bf5),
		IC(0x6863b2), IC(0x5f3edd), IC(0x035d40), IC(0x7f8985), IC(0x295255), IC(0xc06437),
		IC(0x10d86d), IC(0x324832), IC(0x754c5b), IC(0xd4714e), IC(0x6e5445), IC(0xc1090b),
		IC(0x69f52a), IC(0xd56614), IC(0x9d0727), IC(0x50045d), IC(0xdb3bb4), IC(0xc576ea),
		IC(0x17f987), IC(0x7d6b49), IC(0xba271d), IC(0x296996), IC(0xacccc6), IC(0x5414ad),
		IC(0x6ae290), IC(0x89d988), IC(0x50722c), IC(0xbea404), IC(0x940777), IC(0x7030f3),
		IC(0x27fc00), IC(0xa871ea), IC(0x49c266), IC(0x3de064), IC(0x83dd97), IC(0x973fa3),
		IC(0xfd9443), IC(0x8c860d), IC(0xde4131), IC(0x9d3992), IC(0x8c70dd), IC(0xe7b717),
		IC(0x3bdf08), IC(0x2b3715), IC(0xa0805c), IC(0x93805a), IC(0x921110), IC(0xd8e80f),
		IC(0xaf806c), IC(0x4bffdb), IC(0x0f9038), IC(0x761859), IC(0x15a562), IC(0xbbcb61),
		IC(0xb989c7), IC(0xbd4010), IC(0x04f2d2), IC(0x277549), IC(0xf6b6eb), IC(0xbb22db),
		IC(0xaa140a), IC(0x2f2689), IC(0x768364), IC(0x333b09), IC(0x1a940e), IC(0xaa3a51),
		IC(0xc2a31d), IC(0xaeedaf), IC(0x12265c), IC(0x4dc26d), IC(0x9c7a2d), IC(0x9756c0),
		IC(0x833f03), IC(0xf6f009), IC(0x8c402b), IC(0x99316d), IC(0x07b439), IC(0x15200c),
		IC(0x5bc3d8), IC(0xc492f5), IC(0x4badc6), IC(0xa5ca4e), IC(0xcd37a7), IC(0x36a9e6),
		IC(0x9492ab), IC(0x6842dd), IC(0xde6319), IC(0xef8c76), IC(0x528b68), IC(0x37dbfc),
		IC(0xaba1ae), IC(0x3115df), IC(0xa1ae00), IC(0xdafb0c), IC(0x664d64), IC(0xb705ed),
		IC(0x306529), IC(0xbf5657), IC(0x3aff47), IC(0xb9f96a), IC(0xf3be75), IC(0xdf9328),
		IC(0x3080ab), IC(0xf68c66), IC(0x15cb04), IC(0x0622fa), IC(0x1de4d9), IC(0xa4b33d),
		IC(0x8f1b57), IC(0x09cd36), IC(0xe9424e), IC(0xa4be13), IC(0xb52333), IC(0x1aaaf0),
		IC(0xa8654f), IC(0xa5c1d2), IC(0x0f3f0b), IC(0xcd785b), IC(0x76f923), IC(0x048b7b),
		IC(0x721789), IC(0x53a6c6), IC(0xe26e6f), IC(0x00ebef), IC(0x584a9b), IC(0xb7dac4),
		IC(0xba66aa), IC(0xcfcf76), IC(0x1d02d1), IC(0x2df1b1), IC(0xc1998c), IC(0x77adc3),
		IC(0xda4886), IC(0xa05df7), IC(0xf480c6), IC(0x2ff0ac), IC(0x9aecdd), IC(0xbc5c3f),
		IC(0x6dded0), IC(0x1fc790), IC(0xb6db2a), IC(0x3a25a3), IC(0x9aaf00), IC(0x9353ad),
		IC(0x0457b6), IC(0xb42d29), IC(0x7e804b), IC(0xa707da), IC(0x0eaa76), IC(0xa1597b),
		IC(0x2a1216), IC(0x2db7dc), IC(0xfde5fa), IC(0xfedb89), IC(0xfdbe89), IC(0x6c76e4),
		IC(0xfca906), IC(0x70803e), IC(0x156e85), IC(0xff87fd), IC(0x073e28), IC(0x336761),
		IC(0x86182a), IC(0xeabd4d), IC(0xafe7b3), IC(0x6e6d8f), IC(0x396795), IC(0x5bbf31),
		IC(0x48d784), IC(0x16df30), IC(0x432dc7), IC(0x356125), IC(0xce70c9), IC(0xb8cb30),
		IC(0xfd6cbf), IC(0xa200a4), IC(0xe46c05), IC(0xa0dd5a), IC(0x476f21), IC(0xd21262),
		IC(0x845cb9), IC(0x496170), IC(0xe0566b), IC(0x015299), IC(0x375550), IC(0xb7d51e),
		IC(0xc4f133), IC(0x5f6e13), IC(0xe4305d), IC(0xa92e85), IC(0xc3b21d), IC(0x3632a1),
		IC(0xa4b708), IC(0xd4b1ea), IC(0x21f716), IC(0xe4698f), IC(0x77ff27), IC(0x80030c),
		IC(0x2d408d), IC(0xa0cd4f), IC(0x99a520), IC(0xd3a2b3), IC(0x0a5d2f), IC(0x42f9b4),
		IC(0xcbda11), IC(0xd0be7d), IC(0xc1db9b), IC(0xbd17ab), IC(0x81a2ca), IC(0x5c6a08),
		IC(0x17552e), IC(0x550027), IC(0xf0147f), IC(0x8607e1), IC(0x640b14), IC(0x8d4196),
		IC(0xdebe87), IC(0x2afdda), IC(0xb6256b), IC(0x34897b), IC(0xfef305), IC(0x9ebfb9),
		IC(0x4f6a68), IC(0xa82a4a), IC(0x5ac44f), IC(0xbcf82d), IC(0x985ad7), IC(0x95c7f4),
		IC(0x8d4d0d), IC(0xa63a20), IC(0x5f57a4), IC(0xb13f14), IC(0x953880), IC(0x0120cc),
		IC(0x86dd71), IC(0xb6dec9), IC(0xf560bf), IC(0x11654d), IC(0x6b0701), IC(0xacb08c),
		IC(0xd0c0b2), IC(0x485551), IC(0x0efb1e), IC(0xc37295), IC(0x3b06a3), IC(0x3540c0),
		IC(0x7bdc06), IC(0xcc45e0), IC(0xfa294e), IC(0xc8cad6), IC(0x41f3e8), IC(0xde647c),
		IC(0xd8649b), IC(0x31bed9), IC(0xc397a4), IC(0xd45877), IC(0xc5e369), IC(0x13daf0),
		IC(0x3c3aba), IC(0x461846), IC(0x5f7555), IC(0xf5bdd2), IC(0xc6926e), IC(0x5d2eac),
		IC(0xed440e), IC(0x423e1c), IC(0x87c461), IC(0xe9fd29), IC(0xf3d6e7), IC(0xca7c22),
		IC(0x35916f), IC(0xc5e008), IC(0x8dd7ff), IC(0xe26a6e), IC(0xc6fdb0), IC(0xc10893),
		IC(0x745d7c), IC(0xb2ad6b), IC(0x9d6ecd), IC(0x7b723e), IC(0x6a11c6), IC(0xa9cff7),
		IC(0xdf7329), IC(0xbac9b5), IC(0x5100b7), IC(0x0db2e2), IC(0x24ba74), IC(0x607de5),
		IC(0x8ad874), IC(0x2c150d), IC(0x0c1881), IC(0x94667e), IC(0x162901), IC(0x767a9f),
		IC(0xbefdfd), IC(0xef4556), IC(0x367ed9), IC(0x13d9ec), IC(0xb9ba8b), IC(0xfc97c4),
		IC(0x27a831), IC(0xc36ef1), IC(0x36c594), IC(0x56a8d8), IC(0xb5a8b4), IC(0x0ecccf),
		IC(0x2d8912), IC(0x34576f), IC(0x89562c), IC(0xe3ce99), IC(0xb920d6), IC(0xaa5e6b),
		IC(0x9c2a3e), IC(0xcc5f11), IC(0x4a0bfd), IC(0xfbf4e1), IC(0x6d3b8e), IC(0x2c86e2),
		IC(0x84d4e9), IC(0xa9b4fc), IC(0xd1eeef), IC(0xc9352e), IC(0x61392f), IC(0x442138),
		IC(0xc8d91b), IC(0x0afc81), IC(0x6a4afb), IC(0xd81c2f), IC(0x84b453), IC(0x8c994e),
		IC(0xcc2254), IC(0xdc552a), IC(0xd6c6c0), IC(0x96190b), IC(0xb8701a), IC(0x649569),
		IC(0x605a26), IC(0xee523f), IC(0x0f117f), IC(0x11b5f4), IC(0xf5cbfc), IC(0x2dbc34),
		IC(0xeebc34), IC(0xcc5de8), IC(0x605edd), IC(0x9b8e67), IC(0xef3392), IC(0xb817c9),
		IC(0x9b5861), IC(0xbc57e1), IC(0xc68351), IC(0x103ed8), IC(0x4871dd), IC(0xdd1c2d),
		IC(0xa118af), IC(0x462c21), IC(0xd7f359), IC(0x987ad9), IC(0xc0549e), IC(0xfa864f),
		IC(0xfc0656), IC(0xae79e5), IC(0x362289), IC(0x22ad38), IC(0xdc9367), IC(0xaae855),
		IC(0x382682), IC(0x9be7ca), IC(0xa40d51), IC(0xb13399), IC(0x0ed7a9), IC(0x480569),
		IC(0xf0b265), IC(0xa7887f), IC(0x974c88), IC(0x36d1f9), IC(0xb39221), IC(0x4a827b),
		IC(0x21cf98), IC(0xdc9f40), IC(0x5547dc), IC(0x3a74e1), IC(0x42eb67), IC(0xdf9dfe),
		IC(0x5fd45e), IC(0xa4677b), IC(0x7aacba), IC(0xa2f655), IC(0x23882b), IC(0x55ba41),
		IC(0x086e59), IC(0x862a21), IC(0x834739), IC(0xe6e389), IC(0xd49ee5), IC(0x40fb49),
		IC(0xe956ff), IC(0xca0f1c), IC(0x8a59c5), IC(0x2bfa94), IC(0xc5c1d3), IC(0xcfc50f),
		IC(0xae5adb), IC(0x86c547), IC(0x624385), IC(0x3b8621), IC(0x94792c), IC(0x876110),
		IC(0x7b4c2a), IC(0x1a2c80), IC(0x12bf43), IC(0x902688), IC(0x893c78), IC(0xe4c4a8),
		IC(0x7bdbe5), IC(0xc23ac4), IC(0xeaf426), IC(0x8a67f7), IC(0xbf920d), IC(0x2ba365),
		IC(0xb1933d), IC(0x0b7cbd), IC(0xdc51a4), IC(0x63dd27), IC(0xdde169), IC(0x19949a),
		IC(0x9529a8), IC(0x28ce68), IC(0xb4ed09), IC(0x209f44), IC(0xca984e), IC(0x638270),
		IC(0x237c7e), IC(0x32b90f), IC(0x8ef5a7), IC(0xe75614), IC(0x08f121), IC(0x2a9db5),
		IC(0x4d7e6f), IC(0x5119a5), IC(0xabf9b5), IC(0xd6df82), IC(0x61dd96), IC(0x023616),
		IC(0x9f3ac4), IC(0xa1a283), IC(0x6ded72), IC(0x7a8d39), IC(0xa9b882), IC(0x5c326b),
		IC(0x5b2746), IC(0xed3400), IC(0x7700d2), IC(0x55f4fc), IC(0x4d5901), IC(0x8071e0),
		IC(0xe13f89), IC(0xb295f3), IC(0x64a8f1), IC(0xaea74b), IC(0x38fc4c), IC(0xeab2bb),
		IC(0x47270b), IC(0xabc3a7), IC(0x34ba60), IC(0x52dd34), IC(0xf8563a), IC(0xeb7e8a),
		IC(0x31bb36), IC(0x5895b7), IC(0x47f7a9), IC(0x94c3aa), IC(0xd39225), IC(0x1e7f3e),
		IC(0xd8974e), IC(0xbba94f), IC(0xd8ae01), IC(0xe661b4), IC(0x393d8e), IC(0xa523aa),
		IC(0x33068e), IC(0x1633b5), IC(0x3bb188), IC(0x1d3a9d), IC(0x4013d0), IC(0xcc1be5),
		IC(0xf862e7), IC(0x3bf28f), IC(0x39b5bf), IC(0x0bc235), IC(0x22747e), IC(0xa247c0),
		IC(0xd52d1f), IC(0x19add3), IC(0x9094df), IC(0x9311d0), IC(0xb42b25), IC(0x496db2),
		IC(0xe264b2), IC(0x5ef135), IC(0x3bc6a4), IC(0x1a4ad0), IC(0xaac92e), IC(0x64e886),
		IC(0x573091), IC(0x982cfb), IC(0x311b1a), IC(0x08728b), IC(0xbdcee1), IC(0x60e142),
		IC(0xeb641d), IC(0xd0bba3), IC(0xe559d4), IC(0x597b8c), IC(0x2a4483), IC(0xf332ba),
		IC(0xf84867), IC(0x2c8d1b), IC(0x2fa9b0), IC(0x50f3dd), IC(0xf9f573), IC(0xdb61b4),
		IC(0xfe233e), IC(0x6c41a6), IC(0xeea318), IC(0x775a26), IC(0xbc5e5c), IC(0xcea708),
		IC(0x94dc57), IC(0xe20196), IC(0xf1e839), IC(0xbe4851), IC(0x5d2d2f), IC(0x4e9555),
		IC(0xd96ec2), IC(0xe7d755), IC(0x6304e0), IC(0xc02e0e), IC(0xfc40a0), IC(0xbbf9b3),
		IC(0x7125a7), IC(0x222dfb), IC(0xf619d8), IC(0x838c1c), IC(0x6619e6), IC(0xb20d55),
		IC(0xbb5137), IC(0x79e809), IC(0xaf9149), IC(0x0d73de), IC(0x0b0da5), IC(0xce7f58),
		IC(0xac1934), IC(0x724667), IC(0x7a1a13), IC(0x9e26bc), IC(0x4555e7), IC(0x585cb5),
		IC(0x711d14), IC(0x486991), IC(0x480d60), IC(0x56adab), IC(0xd62f64), IC(0x96ee0c),
		IC(0x212ff3), IC(0x5d6d88), IC(0xa67684), IC(0x95651e), IC(0xab9e0a), IC(0x4ddefe),
		IC(0x571010), IC(0x836a39), IC(0xf8ea31), IC(0x9e381d), IC(0xeac8b1), IC(0xcac96b),
		IC(0x37f21e), IC(0xd505e9), IC(0x984743), IC(0x9fc56c), IC(0x0331b7), IC(0x3b8bf8),
		IC(0x86e56a), IC(0x8dc343), IC(0x6230e7), IC(0x93cfd5), IC(0x6a8f2d), IC(0x733005),
		IC(0x1af021), IC(0xa09fcb), IC(0x7415a1), IC(0xd56b23), IC(0x6ff725), IC(0x2f4bc7),
		IC(0xb8a591), IC(0x7fac59), IC(0x5c55de), IC(0x212c38), IC(0xb13296), IC(0x5cff50),
		IC(0x366262), IC(0xfa7b16), IC(0xf4d9a6), IC(0x2acfe7), IC(0xf07403), IC(0xd4d604),
		IC(0x6fd916), IC(0x31b1bf), IC(0xcbb450), IC(0x5bd7c8), IC(0x0ce194), IC(0x6bd643),
		IC(0x4fd91c), IC(0xdf4543), IC(0x5f3453), IC(0xe2b5aa), IC(0xc9aec8), IC(0x131485),
		IC(0xf9d2bf), IC(0xbadb9e), IC(0x76f5b9), IC(0xaf15cf), IC(0xca3182), IC(0x14b56d),
		IC(0xe9fe4d), IC(0x50fc35), IC(0xf5aed5), IC(0xa2d0c1), IC(0xc96057), IC(0x192eb6),
		IC(0xe91d92), IC(0x07d144), IC(0xaea3c6), IC(0x343566), IC(0x26d5b4), IC(0x3161e2),
		IC(0x37f1a2), IC(0x209eff), IC(0x958e23), IC(0x493798), IC(0x35f4a6), IC(0x4bdc02),
		IC(0xc2be13), IC(0xbe80a0), IC(0x0b72a3), IC(0x115c5f), IC(0x1e1bd1), IC(0x0db4d3),
		IC(0x869e85), IC(0x96976b), IC(0x2ac91f), IC(0x8a26c2), IC(0x3070f0), IC(0x041412),
		IC(0xfc9fa5), IC(0xf72a38), IC(0x9c6878), IC(0xe2aa76), IC(0x50cfe1), IC(0x559274),
		IC(0x934e38), IC(0x0a92f7), IC(0x5533f0), IC(0xa63db4), IC(0x399971), IC(0xe2b755),
		IC(0xa98a7c), IC(0x008f19), IC(0xac54d2), IC(0x2ea0b4), IC(0xf5f3e0), IC(0x60c849),
		IC(0xffd269), IC(0xae52ce), IC(0x7a5fdd), IC(0xe9ce06), IC(0xfb0ae8), IC(0xa50cce),
		IC(0xea9d3e), IC(0x3766dd), IC(0xb834f5), IC(0x0da090), IC(0x846f88), IC(0x4ae3d5),
		IC(0x099a03), IC(0x2eae2d), IC(0xfcb40a), IC(0xfb9b33), IC(0xe281dd), IC(0x1b16ba),
		IC(0xd8c0af), IC(0xd96b97), IC(0xb52dc9), IC(0x9c277f), IC(0x5951d5), IC(0x21ccd6),
		IC(0xb6496b), IC(0x584562), IC(0xb3baf2), IC(0xa1a5c4), IC(0x7ca2cf), IC(0xa9b93d),
		IC(0x7b7b89), IC(0x483d38)
	};

	static const double zero = 0.0;
	static const double one = 1.0;
	static const double two24 = 1.67772160000000000000e+07;		/* 0x41700000, 0x00000000 */
	static const double twon24 = 5.96046447753906250000e-08;	/* 0x3E700000, 0x00000000 */

	/* initialize jk */
	jk = init_jk[prec];
	jp = jk;

	/* determine jx,jv,q0, note that 3>q0 */
	jx = nx - 1;
	jv = (e0 - 3) / 24;
	if (jv < 0)
		jv = 0;
	q0 = e0 - 24 * (jv + 1);

	/* set up f[0] to f[jx+jk] where f[jx+jk] = two_over_pi[jv+jk] */
	j = jv - jx;
	m = jx + jk;
	for (i = 0; i <= m; i++, j++)
		f[i] = (j < 0) ? zero : (double) two_over_pi[j];

	/* compute q[0],q[1],...q[jk] */
	for (i = 0; i <= jk; i++)
	{
		for (j = 0, fw = 0.0; j <= jx; j++)
			fw += x[j] * f[jx + i - j];
		q[i] = fw;
	}

	jz = jk;
  recompute:
	/* distill q[] into iq[] reversingly */
	for (i = 0, j = jz, z = q[jz]; j > 0; i++, j--)
	{
		fw = (double) ((int32_t) (twon24 * z));
		iq[i] = (int32_t) (z - two24 * fw);
		z = q[j - 1] + fw;
	}

	/* compute n */
	z = __ieee754_scalbn(z, (int)q0);			/* actual value of z */
	z -= 8.0 * __ieee754_floor(z * 0.125);		/* trim off integer >= 8 */
	n = (int32_t) z;
	z -= (double) n;
	ih = 0;
	if (q0 > 0)
	{									/* need iq[jz-1] to determine n */
		i = (iq[jz - 1] >> (24 - q0));
		n += i;
		iq[jz - 1] -= i << (24 - q0);
		ih = iq[jz - 1] >> (23 - q0);
	} else if (q0 == 0)
	{
		ih = iq[jz - 1] >> 23;
	} else if (z >= 0.5)
	{
		ih = 2;
	}
	
	if (ih > 0)
	{									/* q > 0.5 */
		n += 1;
		carry = 0;
		for (i = 0; i < jz; i++)
		{								/* compute 1-q */
			j = iq[i];
			if (carry == 0)
			{
				if (j != 0)
				{
					carry = 1;
					iq[i] = IC(0x1000000) - j;
				}
			} else
				iq[i] = IC(0xffffff) - j;
		}
		if (q0 > 0)
		{								/* rare case: chance is 1 in 12 */
			switch ((int)q0)
			{
			case 1:
				iq[jz - 1] &= IC(0x7fffff);
				break;
			case 2:
				iq[jz - 1] &= IC(0x3fffff);
				break;
			}
		}
		if (ih == 2)
		{
			z = one - z;
			if (carry != 0)
				z -= __ieee754_scalbn(one, (int)q0);
		}
	}

	/* check if recomputation is needed */
	if (z == zero)
	{
		j = 0;
		for (i = jz - 1; i >= jk; i--)
			j |= iq[i];
		if (j == 0)
		{								/* need recomputation */
			for (k = 1; iq[jk - k] == 0; k++) ;	/* k = no. of terms needed */

			for (i = jz + 1; i <= jz + k; i++)
			{							/* add q[jz+1] to q[jz+k] */
				f[jx + i] = (double) two_over_pi[jv + i];
				for (j = 0, fw = 0.0; j <= jx; j++)
					fw += x[j] * f[jx + i - j];
				q[i] = fw;
			}
			jz += k;
			goto recompute;
		}
	}

	/* chop off zero terms */
	if (z == 0.0)
	{
		jz -= 1;
		q0 -= 24;
		while (iq[jz] == 0)
		{
			jz--;
			q0 -= 24;
		}
	} else
	{									/* break z into 24-bit if necessary */
		z = __ieee754_scalbn(z, (int)-q0);
		if (z >= two24)
		{
			fw = (double) ((int32_t) (twon24 * z));
			iq[jz] = (int32_t) (z - two24 * fw);
			jz += 1;
			q0 += 24;
			iq[jz] = (int32_t) fw;
		} else
		{
			iq[jz] = (int32_t) z;
		}
	}

	/* convert integer "bit" chunk to floating-point value */
	fw = __ieee754_scalbn(one, (int)q0);
	for (i = jz; i >= 0; i--)
	{
		q[i] = fw * (double) iq[i];
		fw *= twon24;
	}

	/* compute PIo2[0,...,jp]*q[jz,...,0] */
	for (i = jz; i >= 0; i--)
	{
		for (fw = 0.0, k = 0; k <= jp && k <= jz - i; k++)
			fw += PIo2[k] * q[i + k];
		fq[jz - i] = fw;
	}

	/* compress fq[] into y[] */
	switch (prec)
	{
	case 0:
		fw = 0.0;
		for (i = jz; i >= 0; i--)
			fw += fq[i];
		y[0] = (ih == 0) ? fw : -fw;
		break;
	case 1:
	case 2:
		{
			volatile double fv = 0.0;
	
			for (i = jz; i >= 0; i--)
				fv += fq[i];
			y[0] = (ih == 0) ? fv : -fv;
			fv = fq[0] - fv;
			for (i = 1; i <= jz; i++)
				fv += fq[i];
			y[1] = (ih == 0) ? fv : -fv;
		}
		break;
	case 3:							/* painful */
		for (i = jz; i > 0; i--)
		{
			volatile double fv = (double) (fq[i - 1] + fq[i]);

			fq[i] += fq[i - 1] - fv;
			fq[i - 1] = fv;
		}
		for (i = jz; i > 1; i--)
		{
			volatile double fv = (double) (fq[i - 1] + fq[i]);

			fq[i] += fq[i - 1] - fv;
			fq[i - 1] = fv;
		}
		for (fw = 0.0, i = jz; i >= 2; i--)
			fw += fq[i];
		if (ih == 0)
		{
			y[0] = fq[0];
			y[1] = fq[1];
			y[2] = fw;
		} else
		{
			y[0] = -fq[0];
			y[1] = -fq[1];
			y[2] = -fw;
		}
	}
	return n & 7;
}
