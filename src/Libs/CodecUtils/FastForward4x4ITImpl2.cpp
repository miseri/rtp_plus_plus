/** @file

MODULE				: FastForward4x4ITImpl2

TAG						: FF4ITI2

FILE NAME			: FastForward4x4ITImpl2.cpp

DESCRIPTION		: A class to implement a fast forward 4x4 2-D integer

COPYRIGHT			: (c)CSIR 2007-2011 all rights resevered

LICENSE				: Software License Agreement (BSD License)

RESTRICTIONS	: Redistribution and use in source and binary forms, with or without 
								modification, are permitted provided that the following conditions 
								are met:

								* Redistributions of source code must retain the above copyright notice, 
								this list of conditions and the following disclaimer.
								* Redistributions in binary form must reproduce the above copyright notice, 
								this list of conditions and the following disclaimer in the documentation 
								and/or other materials provided with the distribution.
								* Neither the name of the CSIR nor the names of its contributors may be used 
								to endorse or promote products derived from this software without specific 
								prior written permission.

								THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
								"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
								LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
								A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
								CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
								EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
								PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
								PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
								LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
								NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
								SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

======================================================================================
*/
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#else
#include <stdio.h>
#endif

#include <string.h>
#include "FastForward4x4ITImpl2.h"

#define FF4ITI2_UNROLL_LOOPS_AND_INTERLEAVE

/*
---------------------------------------------------------------------------
	Constants.
---------------------------------------------------------------------------
*/
const int FastForward4x4ITImpl2::NormAdjust[6][3] =
{
	{13107, 5243, 8066 },
	{11916, 4660, 7490 },
	{10082, 4194, 6554 },
	{ 9362, 3647, 5825 },
	{ 8192, 3355, 5243 },
	{ 7282, 2893, 4559 }
};

const int FastForward4x4ITImpl2::ColSelector[16] =
{
	0, 2, 0, 2 ,
	2, 1, 2, 1 ,
	0, 2, 0, 2 ,
	2, 1, 2, 1 
};

/*
---------------------------------------------------------------------------
	Construction.
---------------------------------------------------------------------------
*/
FastForward4x4ITImpl2::FastForward4x4ITImpl2()	
{ 
	_mode		    = TransformAndQuant; 
	_intra	    = 1;
	_q			= 1;
	_qm			= _q % 6;
	_qe			= _q/6;
	if(_intra)
		_f = (1 << (15+_qe))/3;
	else
		_f = (1 << (15+_qe))/6;
	_scale	= 15+_qe;

}//end constructor.

/*
---------------------------------------------------------------------------
	Interface Methods.
---------------------------------------------------------------------------
*/
/** In-place forward Integer Transform.
The 2-D IT is performed on the input and replaces it with the coeffs. A 1-D
transform is performed on the rows first and then the cols.
@param ptr	: Data to transform.
@return			:	none.
*/
#ifdef FF4ITI2_UNROLL_LOOPS_AND_INTERLEAVE

void FastForward4x4ITImpl2::Transform(void* ptr)
{
	short* block = (short *)ptr;

	if(_mode == IForwardTransform::TransformOnly)
	{
		register int s0,s1,s2,s3;
		register int v0,v1,v2,v3;
		register int r0,r1,r2,r3;
		register int u0,u1,u2,u3;

		/// 1-D forward horiz direction.

		/// 1st stage transform.
		s0 = (int)(block[0]	 + block[3]);
		s3 = (int)(block[0]	 - block[3]);
		s1 = (int)(block[1]  + block[2]);
		s2 = (int)(block[1]  - block[2]);
		v0 = (int)(block[4]	 + block[7]);
		v3 = (int)(block[4]	 - block[7]);
		v1 = (int)(block[5]  + block[6]);
		v2 = (int)(block[5]  - block[6]);
		r0 = (int)(block[8]	 + block[11]);
		r3 = (int)(block[8]	 - block[11]);
		r1 = (int)(block[9]  + block[10]);
		r2 = (int)(block[9]  - block[10]);
		u0 = (int)(block[12] + block[15]);
		u3 = (int)(block[12] - block[15]);
		u1 = (int)(block[13] + block[14]);
		u2 = (int)(block[13] - block[14]);

		/// 2nd stage transform.
		block[0]	= (short)(s0 + s1);
		block[1]	= (short)(s2 + (s3 << 1));
		block[2]	= (short)(s0 - s1);
		block[3]	= (short)(s3 - (s2 << 1));
		block[4]	= (short)(v0 + v1);
		block[5]	= (short)(v2 + (v3 << 1));
		block[6]	= (short)(v0 - v1);
		block[7]	= (short)(v3 - (v2 << 1));
		block[8]	= (short)(r0 + r1);
		block[9]	= (short)(r2 + (r3 << 1));
		block[10]	= (short)(r0 - r1);
		block[11]	= (short)(r3 - (r2 << 1));
		block[12]	= (short)(u0 + u1);
		block[13]	= (short)(u2 + (u3 << 1));
		block[14]	= (short)(u0 - u1);
		block[15]	= (short)(u3 - (u2 << 1));

		/// 1-D forward vert direction.

		/// 1st stage transform.
		s0 = (int)(block[0]	+ block[12]);
		s3 = (int)(block[0]	- block[12]);
		v0 = (int)(block[1]	+ block[13]);
		v3 = (int)(block[1]	- block[13]);
		r0 = (int)(block[2]	+ block[14]);
		r3 = (int)(block[2]	- block[14]);
		u0 = (int)(block[3]	+ block[15]);
		u3 = (int)(block[3]	- block[15]);
		s1 = (int)(block[4] + block[8]);
		s2 = (int)(block[4] - block[8]);
		v1 = (int)(block[5] + block[9]);
		v2 = (int)(block[5] - block[9]);
		r1 = (int)(block[6] + block[10]);
		r2 = (int)(block[6] - block[10]);
		u1 = (int)(block[7] + block[11]);
		u2 = (int)(block[7] - block[11]);

		/// 2nd stage transform.
		block[0]	= (short)(s0 + s1);
		block[1]	= (short)(v0 + v1);
		block[2]	= (short)(r0 + r1);
		block[3]	= (short)(u0 + u1);
		block[4]	= (short)(s2 + (s3 << 1));
		block[5]	= (short)(v2 + (v3 << 1));
		block[6]	= (short)(r2 + (r3 << 1));
		block[7]	= (short)(u2 + (u3 << 1));
		block[8]	= (short)(s0 - s1);
		block[9]	= (short)(v0 - v1);
		block[10]	= (short)(r0 - r1);
		block[11]	= (short)(u0 - u1);
		block[12]	= (short)(s3 - (s2 << 1));
		block[13]	= (short)(v3 - (v2 << 1));
		block[14]	= (short)(r3 - (r2 << 1));
		block[15]	= (short)(u3 - (u2 << 1));

	}//end if TransformOnly...
	else if(_mode == IForwardTransform::QuantOnly)
	{
		/// Scaling and quantisation are combined.
		const int* pNorm = NormAdjust[_qm];

		/// [0]
		if( block[0] >= 0 )
			block[0] = (short)( (((int)block[0] * pNorm[0]) + _f) >> _scale );
		else
			block[0] = (short)(-( (((-(int)block[0]) * pNorm[0]) + _f) >> _scale ));
		/// [1]
		if( block[1] >= 0 )
			block[1] = (short)( (((int)block[1] * pNorm[2]) + _f) >> _scale );
		else
			block[1] = (short)(-( (((-(int)block[1]) * pNorm[2]) + _f) >> _scale ));
		/// [2]
		if( block[2] >= 0 )
			block[2] = (short)( (((int)block[2] * pNorm[0]) + _f) >> _scale );
		else
			block[2] = (short)(-( (((-(int)block[2]) * pNorm[0]) + _f) >> _scale ));
		/// [3]
		if( block[3] >= 0 )
			block[3] = (short)( (((int)block[3] * pNorm[2]) + _f) >> _scale );
		else
			block[3] = (short)(-( (((-(int)block[3]) * pNorm[2]) + _f) >> _scale ));
		/// [4]
		if( block[4] >= 0 )
			block[4] = (short)( (((int)block[4] * pNorm[2]) + _f) >> _scale );
		else
			block[4] = (short)(-( (((-(int)block[4]) * pNorm[2]) + _f) >> _scale ));
		/// [5]
		if( block[5] >= 0 )
			block[5] = (short)( (((int)block[5] * pNorm[1]) + _f) >> _scale );
		else
			block[5] = (short)(-( (((-(int)block[5]) * pNorm[1]) + _f) >> _scale ));
		/// [6]
		if( block[6] >= 0 )
			block[6] = (short)( (((int)block[6] * pNorm[2]) + _f) >> _scale );
		else
			block[6] = (short)(-( (((-(int)block[6]) * pNorm[2]) + _f) >> _scale ));
		/// [7]
		if( block[7] >= 0 )
			block[7] = (short)( (((int)block[7] * pNorm[1]) + _f) >> _scale );
		else
			block[7] = (short)(-( (((-(int)block[7]) * pNorm[1]) + _f) >> _scale ));
		/// [8]
		if( block[8] >= 0 )
			block[8] = (short)( (((int)block[8] * pNorm[0]) + _f) >> _scale );
		else
			block[8] = (short)(-( (((-(int)block[8]) * pNorm[0]) + _f) >> _scale ));
		/// [9]
		if( block[9] >= 0 )
			block[9] = (short)( (((int)block[9] * pNorm[2]) + _f) >> _scale );
		else
			block[9] = (short)(-( (((-(int)block[9]) * pNorm[2]) + _f) >> _scale ));
		/// [10]
		if( block[10] >= 0 )
			block[10] = (short)( (((int)block[10] * pNorm[0]) + _f) >> _scale );
		else
			block[10] = (short)(-( (((-(int)block[10]) * pNorm[0]) + _f) >> _scale ));
		/// [11]
		if( block[11] >= 0 )
			block[11] = (short)( (((int)block[11] * pNorm[2]) + _f) >> _scale );
		else
			block[11] = (short)(-( (((-(int)block[11]) * pNorm[2]) + _f) >> _scale ));
		/// [12]
		if( block[12] >= 0 )
			block[12] = (short)( (((int)block[12] * pNorm[2]) + _f) >> _scale );
		else
			block[12] = (short)(-( (((-(int)block[12]) * pNorm[2]) + _f) >> _scale ));
		/// [13]
		if( block[13] >= 0 )
			block[13] = (short)( (((int)block[13] * pNorm[1]) + _f) >> _scale );
		else
			block[13] = (short)(-( (((-(int)block[13]) * pNorm[1]) + _f) >> _scale ));
		/// [14]
		if( block[14] >= 0 )
			block[14] = (short)( (((int)block[14] * pNorm[2]) + _f) >> _scale );
		else
			block[14] = (short)(-( (((-(int)block[14]) * pNorm[2]) + _f) >> _scale ));
		/// [15]
		if( block[15] >= 0 )
			block[15] = (short)( (((int)block[15] * pNorm[1]) + _f) >> _scale );
		else
			block[15] = (short)(-( (((-(int)block[15]) * pNorm[1]) + _f) >> _scale ));

	}//end if QuantOnly...
	else ///< if (_mode == IForwardTransform::TransformAndQuant)
	{
		register int s0,s1,s2,s3;
		register int v0,v1,v2,v3;
		register int r0,r1,r2,r3;
		register int u0,u1,u2,u3;

		/// 1-D forward horiz direction.

		/// 1st stage transform.
		s0 = (int)(block[0]	 + block[3]);
		s3 = (int)(block[0]	 - block[3]);
		s1 = (int)(block[1]  + block[2]);
		s2 = (int)(block[1]  - block[2]);
		v0 = (int)(block[4]	 + block[7]);
		v3 = (int)(block[4]	 - block[7]);
		v1 = (int)(block[5]  + block[6]);
		v2 = (int)(block[5]  - block[6]);
		r0 = (int)(block[8]	 + block[11]);
		r3 = (int)(block[8]	 - block[11]);
		r1 = (int)(block[9]  + block[10]);
		r2 = (int)(block[9]  - block[10]);
		u0 = (int)(block[12] + block[15]);
		u3 = (int)(block[12] - block[15]);
		u1 = (int)(block[13] + block[14]);
		u2 = (int)(block[13] - block[14]);

		/// 2nd stage transform.
		block[0]	= (short)(s0 + s1);
		block[1]	= (short)(s2 + (s3 << 1));
		block[2]	= (short)(s0 - s1);
		block[3]	= (short)(s3 - (s2 << 1));
		block[4]	= (short)(v0 + v1);
		block[5]	= (short)(v2 + (v3 << 1));
		block[6]	= (short)(v0 - v1);
		block[7]	= (short)(v3 - (v2 << 1));
		block[8]	= (short)(r0 + r1);
		block[9]	= (short)(r2 + (r3 << 1));
		block[10]	= (short)(r0 - r1);
		block[11]	= (short)(r3 - (r2 << 1));
		block[12]	= (short)(u0 + u1);
		block[13]	= (short)(u2 + (u3 << 1));
		block[14]	= (short)(u0 - u1);
		block[15]	= (short)(u3 - (u2 << 1));

		/// 1-D forward vert direction.

		/// 1-D forward vert direction.

		/// 1st stage transform.
		s0 = (int)(block[0]	+ block[12]);
		s3 = (int)(block[0]	- block[12]);
		v0 = (int)(block[1]	+ block[13]);
		v3 = (int)(block[1]	- block[13]);
		r0 = (int)(block[2]	+ block[14]);
		r3 = (int)(block[2]	- block[14]);
		u0 = (int)(block[3]	+ block[15]);
		u3 = (int)(block[3]	- block[15]);
		s1 = (int)(block[4] + block[8]);
		s2 = (int)(block[4] - block[8]);
		v1 = (int)(block[5] + block[9]);
		v2 = (int)(block[5] - block[9]);
		r1 = (int)(block[6] + block[10]);
		r2 = (int)(block[6] - block[10]);
		u1 = (int)(block[7] + block[11]);
		u2 = (int)(block[7] - block[11]);

		/// 2nd stage transform.

		const int* pNorm = NormAdjust[_qm];

		/// [0]
		if( (s0 + s1) >= 0 )
			block[0] = (short)( (((s0 + s1) * pNorm[0]) + _f) >> _scale );
		else
			block[0] = (short)(-( (((-(s0 + s1)) * pNorm[0]) + _f) >> _scale ));
		/// [1]
		if( (v0 + v1) >= 0 )
			block[1] = (short)( (((v0 + v1) * pNorm[2]) + _f) >> _scale );
		else
			block[1] = (short)(-( (((-(v0 + v1)) * pNorm[2]) + _f) >> _scale ));
		/// [2]
		if( (r0 + r1) >= 0 )
			block[2] = (short)( (((r0 + r1) * pNorm[0]) + _f) >> _scale );
		else
			block[2] = (short)(-( (((-(r0 + r1)) * pNorm[0]) + _f) >> _scale ));
		/// [3]
		if( (u0 + u1) >= 0 )
			block[3] = (short)( (((u0 + u1) * pNorm[2]) + _f) >> _scale );
		else
			block[3] = (short)(-( (((-(u0 + u1)) * pNorm[2]) + _f) >> _scale ));

		/// [4]
		register int x4	= s2 + (s3 << 1);
		if( x4 >= 0 )
			block[4] = (short)( ((x4 * pNorm[2]) + _f) >> _scale );
		else
			block[4] = (short)(-( (((-x4) * pNorm[2]) + _f) >> _scale ));
		/// [5]
		register int x5	= v2 + (v3 << 1);
		if( x5 >= 0 )
			block[5] = (short)( ((x5 * pNorm[1]) + _f) >> _scale );
		else
			block[5] = (short)(-( (((-x5) * pNorm[1]) + _f) >> _scale ));
		/// [6]
		register int x6	= r2 + (r3 << 1);
		if( x6 >= 0 )
			block[6] = (short)( ((x6 * pNorm[2]) + _f) >> _scale );
		else
			block[6] = (short)(-( (((-x6) * pNorm[2]) + _f) >> _scale ));
		/// [7]
		register int x7	= u2 + (u3 << 1);
		if( x7 >= 0 )
			block[7] = (short)( ((x7 * pNorm[1]) + _f) >> _scale );
		else
			block[7] = (short)(-( (((-x7) * pNorm[1]) + _f) >> _scale ));

		/// [8]
		if( (s0 - s1) >= 0 )
			block[8] = (short)( (((s0 - s1) * pNorm[0]) + _f) >> _scale );
		else
			block[8] = (short)(-( (((-(s0 - s1)) * pNorm[0]) + _f) >> _scale ));
		/// [9]
		if( (v0 - v1) >= 0 )
			block[9] = (short)( (((v0 - v1) * pNorm[2]) + _f) >> _scale );
		else
			block[9] = (short)(-( (((-(v0 - v1)) * pNorm[2]) + _f) >> _scale ));
		/// [10]
		if( (r0 - r1) >= 0 )
			block[10] = (short)( (((r0 - r1) * pNorm[0]) + _f) >> _scale );
		else
			block[10] = (short)(-( (((-(r0 - r1)) * pNorm[0]) + _f) >> _scale ));
		/// [11]
		if( (u0 - u1) >= 0 )
			block[11] = (short)( (((u0 - u1) * pNorm[2]) + _f) >> _scale );
		else
			block[11] = (short)(-( (((-(u0 - u1)) * pNorm[2]) + _f) >> _scale ));

		/// [12]
		register int x12	= s3 - (s2 << 1);
		if( x12 >= 0 )
			block[12] = (short)( ((x12 * pNorm[2]) + _f) >> _scale );
		else
			block[12] = (short)(-( (((-x12) * pNorm[2]) + _f) >> _scale ));
		/// [13]
		register int x13	= v3 - (v2 << 1);
		if( x13 >= 0 )
			block[13] = (short)( ((x13 * pNorm[1]) + _f) >> _scale );
		else
			block[13] = (short)(-( (((-x13) * pNorm[1]) + _f) >> _scale ));
		/// [14]
		register int x14	= r3 - (r2 << 1);
		if( x14 >= 0 )
			block[14] = (short)( ((x14 * pNorm[2]) + _f) >> _scale );
		else
			block[14] = (short)(-( (((-x14) * pNorm[2]) + _f) >> _scale ));
		/// [15]
		register int x15	= u3 - (u2 << 1);
		if( x15 >= 0 )
			block[15] = (short)( ((x15 * pNorm[1]) + _f) >> _scale );
		else
			block[15] = (short)(-( (((-x15) * pNorm[1]) + _f) >> _scale ));

	}//end else...

}//end Transform.

#else ///< !FF4ITI2_UNROLL_LOOPS_AND_INTERLEAVE

void FastForward4x4ITImpl2::Transform(void* ptr)
{
	short* block = (short *)ptr;
	int j;

	if(_mode == IForwardTransform::TransformOnly)
	{
		/// 1-D forward horiz direction.
		for(j = 0; j < 16; j += 4)
		{
			/// 1st stage transform.
			int s0 = (int)(block[j]		+ block[j+3]);
			int s3 = (int)(block[j]		- block[j+3]);
			int s1 = (int)(block[j+1] + block[j+2]);
			int s2 = (int)(block[j+1] - block[j+2]);

			/// 2nd stage transform.
			block[j]		= (short)(s0 + s1);
			block[j+1]	= (short)(s2 + (s3 << 1));
			block[j+2]	= (short)(s0 - s1);
			block[j+3]	= (short)(s3 - (s2 << 1));
		}//end for j...

		/// 1-D forward vert direction.
		for(j = 0; j < 4; j++)
		{
			/// 1st stage transform.
			int s0 = (int)(block[j]		+ block[j+12]);
			int s3 = (int)(block[j]		- block[j+12]);
			int s1 = (int)(block[j+4] + block[j+8]);
			int s2 = (int)(block[j+4] - block[j+8]);

			/// 2nd stage transform.
			block[j]		= (short)(s0 + s1);
			block[j+4]	= (short)(s2 + (s3 << 1));
			block[j+8]	= (short)(s0 - s1);
			block[j+12]	= (short)(s3 - (s2 << 1));
		}//end for j...

	}//end if TransformOnly...
	else if(_mode == IForwardTransform::QuantOnly)
	{
		/// Scaling and quantisation are combined.
		const int* pNorm = NormAdjust[_qm];

		for(j = 0; j < 16; j++)
		{
			if( block[j] >= 0 )
				block[j] = (short)( (((int)block[j] * pNorm[ColSelector[j]]) + _f) >> _scale );
			else
				block[j] = (short)(-( (((-(int)block[j]) * pNorm[ColSelector[j]]) + _f) >> _scale ));
		}//end for j...
	}//end if QuantOnly...
	else ///< if (_mode == IForwardTransform::TransformAndQuant)
	{
		const int* pNorm = NormAdjust[_qm];

		/// 1-D forward horiz direction.
		for(j = 0; j < 16; j += 4)
		{
			/// 1st stage transform.
			int s0 = (int)(block[j]		+ block[j+3]);
			int s3 = (int)(block[j]		- block[j+3]);
			int s1 = (int)(block[j+1] + block[j+2]);
			int s2 = (int)(block[j+1] - block[j+2]);

			/// 2nd stage transform.
			block[j]		= (short)(s0 + s1);
			block[j+1]	= (short)(s2 + (s3 << 1));
			block[j+2]	= (short)(s0 - s1);
			block[j+3]	= (short)(s3 - (s2 << 1));
		}//end for j...

		/// 1-D forward vert direction.
		for(j = 0; j < 4; j++)
		{
			/// 1st stage transform.
			int s0 = (int)block[j]		+ (int)block[j+12];
			int s3 = (int)block[j]		- (int)block[j+12];
			int s1 = (int)block[j+4]	+ (int)block[j+8];
			int s2 = (int)block[j+4]	- (int)block[j+8];

			/// 2nd stage transform with scaling and quantisation.
			int x0 = s0 + s1;
			int x1 = s2 + (s3 << 1);
			int x2 = s0 - s1;
			int x3 = s3 - (s2 << 1);

			if(x0 < 0)
				block[j] = (short)(-( (((-x0) * pNorm[ColSelector[j]]) + _f) >> _scale ));
			else
				block[j] = (short)( ((x0 * pNorm[ColSelector[j]]) + _f) >> _scale );

			if(x1 < 0)
				block[j+4] = (short)(-( (((-x1) * pNorm[ColSelector[j+4]]) + _f) >> _scale ));
			else
				block[j+4] = (short)( ((x1 * pNorm[ColSelector[j+4]]) + _f) >> _scale );

			if(x2 < 0)
				block[j+8] = (short)(-( (((-x2) * pNorm[ColSelector[j+8]]) + _f) >> _scale ));
			else
				block[j+8] = (short)( ((x2 * pNorm[ColSelector[j+8]]) + _f) >> _scale );

			if(x3 < 0)
				block[j+12] = (short)(-( (((-x3) * pNorm[ColSelector[j+12]]) + _f) >> _scale ));
			else
				block[j+12] = (short)( ((x3 * pNorm[ColSelector[j+12]]) + _f) >> _scale );
		}//end for j...
	}//end else...

}//end Transform.

#endif 

/** Transfer forward IT.
The IT is performed on the input and the coeffs are written to 
the output.
@param pIn		: Input data.
@param pCoeff	: Output coeffs.
@return				:	none.
*/
void FastForward4x4ITImpl2::Transform(void* pIn, void* pCoeff)
{
	/// Copy to output and then do in-place inverse transform.
	memcpy(pCoeff, pIn, sizeof(short) * 16);
	Transform(pCoeff);
}//end Transform.

/** Set and get parameters for the implementation.
An Intra and Inter parameter and quantisation parameter are the only 
requirement for this implementation.
@param paramID	: Parameter to set/get.
@param paramVal	: Parameter value.
@return					: None (Set) or the param value (Get).
*/
void FastForward4x4ITImpl2::SetParameter(int paramID, int paramVal)
{
	switch(paramID)
	{
		case QUANT_ID:
			{
				// TODO: Put these calcs into a table.
				if(paramVal != _q)	///< Do we need to change member values or keep previous?
				{
					_q = paramVal;
          SetNewQP();
				}//end if q...
			}//end case QUANT_ID...
			break;
		case INTRA_FLAG_ID:
			_intra = paramVal;
      SetNewQP();
			break;
		default :
			/// Do nothing.
			break;
	}//end switch paramID...
}//end SetParameter.

int FastForward4x4ITImpl2::GetParameter(int paramID)
{
	int ret;
	switch(paramID)
	{
		case QUANT_ID:
			ret = _q;
			break;
		case INTRA_FLAG_ID:
			ret = _intra;
			break;
		default :
			ret = _intra;	///< Intra flag is returned as default.
			break;
	}//end switch paramID...

	return(ret);
}//end GetParameter.

/** Quantise a single value in a block.
Implement quantising a value in a specified position of the coeffs at a
specified QP value.
@param val  : Value to quantise.
@param pos  : Postion of value in 1-D array of 2-D data.
@param qp   : Quant param to use.
@return     : Result of quantisation.
*/
int FastForward4x4ITImpl2::QuantiseValue(short val, int pos, int qp)
{
  _q = qp;
  SetNewQP();

  int norm = 0;
  switch(pos)
  {
    /*  By default norm is 0.
    case 0:
    case 2:
    case 8:
    case 10:
      norm = 0;
      break;
    */

    case 5:
    case 7:
    case 13:
    case 15:
      norm = 1;
      break;

    case 1:
    case 3:
    case 4:
    case 6:
    case 9:
    case 11:
    case 12:
    case 14:
      norm = 2;
      break;
  }//end switch...

  short quantVal = 0;
	if( val >= 0 )
		quantVal = (short)( (((int)val * NormAdjust[_qm][norm]) + _f) >> _scale );
	else
		quantVal = (short)(-( (((-(int)val) * NormAdjust[_qm][norm]) + _f) >> _scale ));

  return((int)quantVal);
}//end QuantiseValue. 

/** Set internal quant members based on _q.
@return: none.
*/
void FastForward4x4ITImpl2::SetNewQP(void)
{
	_qm			= _q % 6;
	_qe			= _q/6;
	if(_intra)
		_f = (1 << (15+_qe))/3;
	else
		_f = (1 << (15+_qe))/6;
	_scale	= 15+_qe;
}//end SetNewQP.
