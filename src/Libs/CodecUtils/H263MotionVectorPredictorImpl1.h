/** @file

MODULE				: H263MotionVectorPredictorImpl1

TAG						: H263MVPI1

FILE NAME			: H263MotionVectorPredictorImpl1.h

DESCRIPTION		: A class to predicting motion vectors from the surrounding motion
                vectors of previously encoded/decoded macroblocks. Implements the 
                IMotionVectorPredictor() interface.

COPYRIGHT			: (c)CSIR 2007-2012 all rights resevered

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
#ifndef _H263MOTIONVECTORPREDICTOR_H
#define _H263MOTIONVECTORPREDICTOR_H

#include "IMotionVectorPredictor.h"

/*
---------------------------------------------------------------------------
	Class definition.
---------------------------------------------------------------------------
*/
class H263MotionVectorPredictorImpl1 : public IMotionVectorPredictor
{
/// Construction.
public:

  H263MotionVectorPredictorImpl1(int imgWidth, int imgHeight) { _widthInMbs = imgWidth/16; _heightInMbs = imgHeight/16; }

  virtual ~H263MotionVectorPredictorImpl1(void) {}

/// IMotionVectorPredictor Interface.
public:
  virtual int	Create(void) { return(1); }

	/** Get the 16x16 2-D prediction for the macroblock/block.
	@param pList	: Input list of motion vectors.
	@param blk		: Macroblock/block number to get the prediction for.
  @param predX  : Output predicted X coordinate
  @param predX  : Output predicted Y coordinate
	@return	      :	1 = success, 0 = failure.
	*/
	virtual int Get16x16Prediction(	void* pList, int blk, int* predX, int* predY);

/// Local methods.
protected:

  /** Calc the median of 3 numbers.
  @param x	:	1st num.
  @param y	:	2nd num.
  @param z	:	3rd num.
  @return		: The median of x, y and z.
  */
  int Median(int x, int y, int z);

/// Local members.
protected:

	// Parameters must remain const for the life time of this instantiation.
	int	_widthInMbs;		///< Width of the img in macroblock units.
	int	_heightInMbs;	  ///< Height of the img in macroblock units.

};//end H263MotionVectorPredictorImpl1.

#endif // !_H263MOTIONVECTORPREDICTOR_H

