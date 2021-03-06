/** @file

MODULE				: FastForwardDC4x4ITImpl1

TAG						: FFDC4ITI1

FILE NAME			: FastForwardDC4x4ITImpl1.cpp

DESCRIPTION		: A class to implement a fast forward 4x4 2-D integer
								Hardamard transform defined by the H.264 (03/2005) 
								standard on the input. It implements the 
								IForwardTransform interface. The scaling is part of 
								the quantisation process.

COPYRIGHT			:	(c)CSIR 2007-2009 all rights resevered

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

===========================================================================
*/
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#else
#include <stdio.h>
#endif

#include <string.h>
#include <math.h>
#include "FastForwardDC4x4ITImpl1.h"

/*
---------------------------------------------------------------------------
	Constants.
---------------------------------------------------------------------------
*/

const int FastForwardDC4x4ITImpl1::NormAdjust[6] =
{
	13107, 
	11916, 
	10082, 
	 9362, 
	 8192, 
	 7282
};

/*
---------------------------------------------------------------------------
	Construction and Destruction.
---------------------------------------------------------------------------
*/
FastForwardDC4x4ITImpl1::FastForwardDC4x4ITImpl1()
{ 
	_mode		= TransformAndQuant; 
	_q			= 1;
	_qm			= _q % 6;
	_qe			= _q/6;
//	_f			= (1 << (15+_qe))/6;
//	_scale	= 15+_qe;
	_f			= 2 * ((1 << (15+_qe))/3);
	_scale	= 16 + _qe;
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
void FastForwardDC4x4ITImpl1::Transform(void* ptr)
{
	short* block = (short *)ptr;
	int j;

	/// Only do IT modes = 0 & 1.
	if(_mode != QuantOnly)	///< 2 = Q only.
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
			block[j+2]	= (short)(s0 - s1);
			block[j+1]	= (short)(s2 + s3);
			block[j+3]	= (short)(s3 - s2);
		}//end for j...

		/// 1-D forward vert direction.

		if(_mode == TransformAndQuant)	///< 0 = IT+Q.
		{
			for(j = 0; j < 4; j++)
			{
				/// 1st stage transform.
				int s0 = (int)block[j]		+ (int)block[j+12];
				int s3 = (int)block[j]		- (int)block[j+12];
				int s1 = (int)block[j+4]	+ (int)block[j+8];
				int s2 = (int)block[j+4]	- (int)block[j+8];

				/// 2nd stage transform with scaling and quantisation.
				int x0 = (s0 + s1) >> 1;
				if(x0 < 0)
					block[j] = (short)(-( (((-x0) * NormAdjust[_qm]) + _f) >> _scale ));
				else
					block[j] = (short)( ((x0 * NormAdjust[_qm]) + _f) >> _scale );

				int x2 = (s0 - s1) >> 1;
				if(x2 < 0)
					block[j+8] = (short)(-( (((-x2) * NormAdjust[_qm]) + _f) >> _scale ));
				else
					block[j+8] = (short)( ((x2 * NormAdjust[_qm]) + _f) >> _scale );

				int x1 = (s2 + s3) >> 1;
				if(x1 < 0)
					block[j+4] = (short)(-( (((-x1) * NormAdjust[_qm]) + _f) >> _scale ));
				else
					block[j+4] = (short)( ((x1 * NormAdjust[_qm]) + _f) >> _scale );

				int x3 = (s3 - s2) >> 1;
				if(x3 < 0)
					block[j+12] = (short)(-( (((-x3) * NormAdjust[_qm]) + _f) >> _scale ));
				else
					block[j+12] = (short)( ((x3 * NormAdjust[_qm]) + _f) >> _scale );
			}//end for j...
		}//end if _mode...
		else	///< mode = 1 i.e. IT only.
		{
			for(j = 0; j < 4; j++)
			{
				/// 1st stage transform.
				int s0 = (int)(block[j]		+ block[j+12]);
				int s3 = (int)(block[j]		- block[j+12]);
				int s1 = (int)(block[j+4] + block[j+8]);
				int s2 = (int)(block[j+4] - block[j+8]);

				/// 2nd stage transform.
				block[j]		= (short)((s0 + s1) >> 1);
				block[j+8]	= (short)((s0 - s1) >> 1);
				block[j+4]	= (short)((s2 + s3) >> 1);
				block[j+12]	= (short)((s3 - s2) >> 1);
			}//end for j...
		}//end else...
	}//end if _mode != 2...
	else ///< mode = 2 i.e. Q only.
	{
		/// Scaling and quantisation are combined.
		int i;
		for(i = 0; i < 4; i++)
			for(j = 0; j < 4; j++)
			{
				int x = (int)block[i*4 + j];
				if( x < 0 )
					block[i*4 + j] = (short)(-( (((-x) * NormAdjust[_qm]) + _f) >> _scale ));
				else
					block[i*4 + j] = (short)( ((x * NormAdjust[_qm]) + _f) >> _scale );
			}//end for i & j...
	}//end else...

}//end Transform.

/** Transfer forward IT.
The IT is performed on the input and the coeffs are written to 
the output.
@param pIn		: Input data.
@param pCoeff	: Output coeffs.
@return				:	none.
*/
void FastForwardDC4x4ITImpl1::Transform(void* pIn, void* pCoeff)
{
	/// Copy to output and then do in-place inverse transform.
	memcpy(pCoeff, pIn, sizeof(short) * 16);
	Transform(pCoeff);
}//end Transform.

/** Set and get parameters for the implementation.
An quantisation parameter and quantisation parameter are the only 
requirement for this implementation.
@param paramID	: Parameter to set/get.
@param paramVal	: Parameter value.
@return					: None (Set) or the param value (Get).
*/
void FastForwardDC4x4ITImpl1::SetParameter(int paramID, int paramVal)
{
	switch(paramID)
	{
		case QUANT_ID:
			{
				// TODO: Put these calcs into a table.
				if(paramVal != _q)	///< Do we need to change member values or keep previous?
				{
					_qm			= paramVal % 6;
					_qe			= paramVal/6;
//					_f			= (1 << (15+_qe))/6;
//					_scale	= 15+_qe;
        	_f			= 2 * ((1 << (15+_qe))/3);
	        _scale	= 16 + _qe;
					_q			= paramVal;
				}//end if q...
			}//end case QUANT_ID...
			break;
		default :
			/// Do nothing.
			break;
	}//end switch paramID...
}//end SetParameter.

int FastForwardDC4x4ITImpl1::GetParameter(int paramID)
{
	int ret;
	switch(paramID)
	{
		case QUANT_ID:
			ret = _q;
			break;
		default :
			ret = _q;	///< Quant value is returned as default.
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
int FastForwardDC4x4ITImpl1::QuantiseValue(short val, int pos, int qp)
{
  _q = qp;
	_qm			= qp % 6;
	_qe			= qp/6;
  _f			= 2 * ((1 << (15+_qe))/3);
	_scale	= 16 + _qe;

  short quantVal = 0;
	if( val < 0 )
		quantVal = (short)(-( (((-val) * NormAdjust[_qm]) + _f) >> _scale ));
	else
		quantVal = (short)( ((val * NormAdjust[_qm]) + _f) >> _scale );

  return((int)quantVal);
}//end QuantiseValue. 


