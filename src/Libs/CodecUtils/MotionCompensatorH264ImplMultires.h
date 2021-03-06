/** @file

MODULE				: MotionCompensatorH264ImplMultires

TAG						: MCH264IM

FILE NAME			: MotionCompensatorH264ImplMultires.h

DESCRIPTION		: A fast unrestricted motion compensator implementation for 
								Recommendation H.264 (03/2005) with both absolute error 
								difference and square error measure. Access is via an 
								IMotionCompensator interface. There are 2 mode levels of 
								execution speed vs. quality. The boundary is extended to 
								accomodate the selected motion range.

COPYRIGHT			: (c)CSIR 2007-2010 all rights resevered

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
#ifndef _MOTIONCOMPENSATORH264IMPLMULTIRES_H
#define _MOTIONCOMPENSATORH264IMPLMULTIRES_H

#include "IMotionCompensator.h"
#include "VectorStructList.h"
#include "OverlayMem2Dv2.h"
#include "OverlayExtMem2Dv2.h"

/*
---------------------------------------------------------------------------
	Class definition.
---------------------------------------------------------------------------
*/
class MotionCompensatorH264ImplMultires : public IMotionCompensator
{
	// Construction.
	public:

		MotionCompensatorH264ImplMultires(int range);
		virtual ~MotionCompensatorH264ImplMultires(void);

	// IMotionCompensator Interface.
	public:
		
		virtual int	Create(	void* ref, 
												int imgWidth,	int imgHeight, 
												int macroBlkWidth, int macroBlkHeight);
		virtual void	Destroy(void);
		virtual void	Reset(void);

		/** Motion compensate to the reference.
		Do the compensation with the block sizes and image sizes defined in
		the implementation and set in Create().
		@param pMotionList	: The list of motion vectors.
		@return							: None.
		*/
		virtual void Compensate(void* pMotionList);

		/** Motion compensate a single vector to the reference.
		Do the compensation with the block sizes and image sizes defined in
		the implementation and set in Create(). The vector coords are in 
		half pel units for this implementation. NOTE: The temp image must
		hold a copy of the ref BEFORE using this method and is should be done
		in PrepareForSingleVectorMode().
		@param tlx	: Top left x coord of block.
		@param tly	: Top left y coord of block.
		@param mvx	: X coord of the motion vector.
		@param mvy	: Y coord of the motion vector.
		@return			: None.
		*/
		virtual void Compensate(int tlx, int tly, int mvx, int mvy);

		/** Prepare the ref for single motion vector compensation mode.
		Should be used to copy the ref into a temp location from which to
		do the compensation to the ref. Prevents interference and double
		compensation.
		@return : none.
		*/
		virtual void PrepareForSingleVectorMode(void);

	/// Local methods.
	protected:

		/// Parameters must remain const for the life time of this instantiation.
		int	_imgWidth;				// Width of the ref images. 
		int	_imgHeight;				// Height of the ref images.
		int _chrWidth;
		int _chrHeight;
		int	_macroBlkWidth;		// Width of the motion block.
		int	_macroBlkHeight;	// Height of the motion block.
		int _chrMacroBlkWidth;
		int _chrMacroBlkHeight;
		int _range;						// Max range of the motion [-_range ... (_range-1)].

		int _refSize;					// Total size (in mcisType) of the contiguous Lum, ChrU, ChrV.
		/// Ref image holders.
		short*		_pRefLum;		// References to the images at creation.
		short*		_pRefChrU;
		short*		_pRefChrV;

		// Overlays to the ref image.
		OverlayMem2Dv2*	_pRefLumOver;
		OverlayMem2Dv2*	_pRefChrUOver;
		OverlayMem2Dv2*	_pRefChrVOver;

		// Extended temp ref mem created by ExtendBoundary() calls for each colour
		// component and an extended boundary overlay for each.
		short*							_pExtTmpLum;
		OverlayExtMem2Dv2*	_pExtTmpLumOver;
		int									_extLumWidth;
		int									_extLumHeight;
		short*							_pExtTmpChrU;
		OverlayExtMem2Dv2*	_pExtTmpChrUOver;
		short*							_pExtTmpChrV;
		OverlayExtMem2Dv2*	_pExtTmpChrVOver;
		int									_extChrWidth;
		int									_extChrHeight;

		// A work block.
		short*					_pMBlk;
		OverlayMem2Dv2* _pMBlkOver;
};//end MotionCompensatorH264ImplMultires.


#endif	// _MOTIONCOMPENSATORH264IMPLMULTIRES_H
