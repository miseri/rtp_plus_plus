/** @file

MODULE				: BitStreamBase

TAG						: BSB

FILE NAME			: BitStreamBase.h

DESCRIPTION		: A base class to contain a contiguous mem array to be bit accessible. The 
                mem is not owned by the class. Derived classes handle the reading and writing, 
								BitStreamReader and BitStreamWriter.

COPYRIGHT			:	(c)CSIR 2007-2013 all rights resevered

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

=========================================================================================
*/
#ifndef _BITSTREAMBASE_H
#define _BITSTREAMBASE_H

#pragma once

/*
---------------------------------------------------------------------------
	Class definition.
---------------------------------------------------------------------------
*/
class BitStreamBase
{
public:
	BitStreamBase()	
		{_bitStream = NULL; _bitSize = 0; _bytePos = 0; _bitPos = 0; }
	virtual ~BitStreamBase()	{ }

	virtual void SetStream(void* stream, int bitSize)	
		{ _bitStream = (unsigned char *)stream; 
			_bitSize = bitSize; 
			_bytePos = 0; _bitPos = 0; 
		}//end SetStream.

  virtual void* GetStream(void) { return( (void *)_bitStream ); }

public:
	/// Interface implementation.
	virtual void Reset(void) 
		{ _bytePos = 0; _bitPos = 0; }

	virtual int Seek(int streamBitPos) 
		{ if(streamBitPos >= _bitSize) 
				return(0);
			_bytePos	= streamBitPos / 8; 
			_bitPos		= streamBitPos % 8; 
			return(1); 
		}//end Seek.

	virtual int		GetStreamBitPos(void)					{ return( (_bytePos << 3) +  _bitPos); }
	virtual int		GetStreamBytePos(void)				{ return(_bytePos); }
	virtual void	SetStreamBitSize(int bitSize) { _bitSize = bitSize; }
	virtual int		GetStreamBitSize(void)				{ return(_bitSize); }

protected:
	unsigned char*	_bitStream;		///< Reference to byte array.
	int							_bitSize;			///< Bits in stream.
	/// Current location.
	int							_bytePos;
	int							_bitPos;

};// end class BitStreamBase.

#endif	// _BITSTREAMBASE_H
