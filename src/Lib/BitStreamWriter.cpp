/** @file

MODULE						: BitStreamWriter

TAG								: BSW

FILE NAME					: BitStreamWriter.cpp

DESCRIPTION				: A bit stream writer implementation of the BitStreamBase
										base class. Add the functionality to do the writing.

REVISION HISTORY	:
									: 

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
===========================================================================
*/
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#else
#include <stdio.h>
#endif

#include "BitStreamWriter.h"

BitStreamWriter::BitStreamWriter()
{
}//end constructor.

BitStreamWriter::~BitStreamWriter()
{
}//end destructor.

/** Write a single bit.
Write to the current bit position in the stream.
@param val	: Bit value to write.
@return			: none.
*/
void BitStreamWriter::Write(int val)
{
  // Strip out the LSB bit of the input and write it to the bit position.
  if(val & 1)
		_bitStream[_bytePos] = _bitStream[_bytePos] | (1 << _bitPos);
  else
    _bitStream[_bytePos] = _bitStream[_bytePos] & ~(1 << _bitPos);

  // Point to next available bit.
  if(_bitPos < 7)
    _bitPos++;
	else
  {
		// Update the bit stream location.
    _bitPos = 0;
    _bytePos++;
  }//end else...
}//end Write.

/** Write bits to the stream.
Write multiple bits from the least significant bit upwards
into the current stream position.
@param numBits	: No. of bits to write.
@param val			: Bit value to write.
@return					: none.
*/
void BitStreamWriter::Write(int numBits, int val)
{
  int pos = _bitPos;
  int b		= val;
	unsigned char cachedByte = _bitStream[_bytePos];

  for(int i = numBits; i > 0; i--)
  {
    // Strip out the LSB bit of the input and write it to bit position pos.
    if(b & 1)
			cachedByte = cachedByte | (1 << pos);
    else
      cachedByte = cachedByte & ~(1 << pos);

    // Point to next available bit.
    if(pos < 7)
			pos++;
		else
    {
			// Update the bit stream from cache.
			_bitStream[_bytePos] = cachedByte;
      pos = 0;
      _bytePos++;
			// Renew the cache.
			cachedByte = _bitStream[_bytePos];
    }//end else...

    // Update the bits still to add.
    b = b >> 1;
  }//end for i...

	// Flush the cache.
	_bitStream[_bytePos] = cachedByte;
  // Update the global next bit position.
  _bitPos = pos;
}//end Write.

/** Poke bits to the stream.
Write multiple bits from the least significant bit upwards
into the specified stream position without disturbing the
current stream position.
@param bitLoc		: Bit pos in stream.
@param numBits	: No. of bits to write.
@param val			: Bit value to write.
@return					: none.
*/
void BitStreamWriter::Poke(int bitLoc, int numBits, int val)
{
	int bytePos = bitLoc / 8;
  int bitPos	= bitLoc % 8;
  int b				= val;

  for(int i = numBits; i > 0; i--)
  {
    // Strip out the LSB bit of the input and write it to bit position.
    if(b & 1)
			_bitStream[bytePos] = _bitStream[bytePos] | (1 << bitPos);
    else
      _bitStream[bytePos] = _bitStream[bytePos] & ~(1 << bitPos);

    // Point to next available bit.
    if(bitPos < 7)
			bitPos++;
		else
    {
      bitPos = 0;
      bytePos++;
    }//end else...

    // Update the bits still to add.
    b = b >> 1;
  }//end for i...

}//end Poke.


