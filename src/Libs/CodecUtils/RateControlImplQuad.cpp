/** @file

MODULE				: RateControlImplQuad

TAG						: RCIQ

FILE NAME			: RateControlImplQuad.cpp

DESCRIPTION		: A class to hold the model for the frame buffer rate rate control to
                match an average rate stream. This class models the RD curve with an
                inverse quadratic function.

COPYRIGHT			: (c)CSIR 2007-2015 all rights resevered

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
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#else
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#endif

#include "RateControlImplQuad.h"

/*
---------------------------------------------------------------------------
	Construction and destruction.
---------------------------------------------------------------------------
*/
int RateControlImplQuad::Create(int numOfFrames)
{
  /// Clean up first.
  Destroy();

  if( (!_meanDiffSamples.Create(numOfFrames))   || (!_totalRateSamples.Create(numOfFrames))  || 
      (!_coeffRateSamples.Create(numOfFrames))  || (!_distortionSamples.Create(numOfFrames))    ||
      (!_headerRateSamples.Create(numOfFrames)) || (!_modelRateDiffSamples.Create(numOfFrames)) ) 
    return(0);

  _numOfFrames = numOfFrames; 

  _meanDiffSamples.Clear(); 
  _totalRateSamples.Clear(); 
  _coeffRateSamples.Clear(); 
  _distortionSamples.Clear();
  _headerRateSamples.Clear();
  _modelRateDiffSamples.Clear();

  _prevMeanDiff       = 0.0;
  _prevTotalRate      = 0.0;
  _prevCoeffRate      = 0.0;
  _prevDistortion     = 0.0;
  _prevHeaderRate     = 0.0;
  _prevModelRateDiff  = 0.0;
  _movingAvg          = 0.0;
  _movingAvgCount     = 1;

  _prevCoeffRatePred  = 0.0;
  _outOfBoundsFlag    = false;

  _Sx   = 0.0;
  _Sy   = 0.0;
  _Sxy  = 0.0;
  _Sx2  = 0.0;

  _SEi  = (double)(numOfFrames*(numOfFrames+1))/2.0;
  _SEi2 = (double)(numOfFrames*(numOfFrames+1)*((2*numOfFrames)+1))/6.0;
  _SEy  = 0.0;
  _SEiy = 0.0;
  _a1   = 1.0;
  _a2   = 0.0;

  _Sbuff = 0.0;

  _SPy  = 0.0;
  _SPiy = 0.0;
  _p1   = 1.0;
  _p2   = 0.0;

  _SMy  = 0.0;
  _SMiy = 0.0;
  _m1   = 1.0;
  _m2   = 0.0;

  _RCTableLen = 0;
  _RCTablePos = 0;

#ifdef RCIQ_DUMP_RATECNTL
  _RCTableLen = 300;
	_RCTable.Create(11, _RCTableLen);

  _RCTable.SetHeading(0, "Frame");
  _RCTable.SetDataType(0, MeasurementTable::INT);
  _RCTable.SetHeading(1, "Dmax");
  _RCTable.SetDataType(1, MeasurementTable::INT);
  _RCTable.SetHeading(2, "Rate");
  _RCTable.SetDataType(2, MeasurementTable::DOUBLE);
  _RCTable.SetHeading(3, "Pred Coeff Rate");
  _RCTable.SetDataType(3, MeasurementTable::DOUBLE);
  _RCTable.SetHeading(4, "Coeff Rate");
  _RCTable.SetDataType(4, MeasurementTable::DOUBLE);
  _RCTable.SetHeading(5, "Pred Head Rate");
  _RCTable.SetDataType(5, MeasurementTable::DOUBLE);
  _RCTable.SetHeading(6, "Head Rate");
  _RCTable.SetDataType(6, MeasurementTable::DOUBLE);
  _RCTable.SetHeading(7, "Pred Mean Diff");
  _RCTable.SetDataType(7, MeasurementTable::DOUBLE);
  _RCTable.SetHeading(8, "Mean Diff");
  _RCTable.SetDataType(8, MeasurementTable::DOUBLE);
  _RCTable.SetHeading(9, "RD a");
  _RCTable.SetDataType(9, MeasurementTable::DOUBLE);
  _RCTable.SetHeading(10, "RD b");
  _RCTable.SetDataType(10, MeasurementTable::DOUBLE);

  _predMD           = 0.0;
  _MD               = 0.0;
  _predHeaderRate  = 0.0;
  _headerRate      = 0.0;
  _predDmax         = 0.0;
  _predCoeffRate    = 0.0;
  _coeffRate        = 0.0;
  _rate             = 0.0;
#endif

  return(1);
}//end Create.

void RateControlImplQuad::Destroy(void)
{
  _RCTable.Destroy();

  _modelRateDiffSamples.Destroy();
  _headerRateSamples.Destroy(); 
  _distortionSamples.Destroy(); 
  _coeffRateSamples.Destroy(); 
  _totalRateSamples.Destroy(); 
  _meanDiffSamples.Destroy();

  _numOfFrames = 0;

}//end Destroy.

void RateControlImplQuad::Reset(void)
{
  _modelRateDiffSamples.MarkAsEmpty();
  _headerRateSamples.MarkAsEmpty(); 
  _distortionSamples.MarkAsEmpty(); 
  _coeffRateSamples.MarkAsEmpty(); 
  _totalRateSamples.MarkAsEmpty(); 
  _meanDiffSamples.MarkAsEmpty();

}//end Reset.

void RateControlImplQuad::Dump(char* filename)
{
  if(_RCTablePos > 0)
    _RCTable.Save(filename, ",", 1);
  _RCTablePos = 0;
}//end Dump.

/*
---------------------------------------------------------------------------
	Public Interface Methods.
---------------------------------------------------------------------------
*/

///---------------------- Post-Encoding Measurements --------------------------

/// Add samples to the fifo buffers and hold the sample that is discarded. The
/// last discarded value is needed in the running sum update process.

/** Store model measurements after encoding a frame.
All rate measurements are in bpp.
@param rate       : Total rate of the frame including headers.
@param coeffrate  : Rate of the frame coeff encoding without headers.
@param distortion : Implementation specific distortion of the frame.
@param meandiff   : Implementation specific signal mean difference of the frame.
@return           : none.
*/
void RateControlImplQuad::StoreMeasurements(double rate, double coeffrate, double distortion, double mse, double mae)
{ 
  bool initialisationRequired = (bool)(!ValidData());  ///< Status prior to storing new measurement values.
  double meandiff = mae;

  /// Store all new measurements.
  PutMeanDiff(meandiff, initialisationRequired);      ///<  NB: Mean diff MUST be stored first as the others use its value.
  PutDistortion(distortion, initialisationRequired);  ///< Internally uses meandiff
  PutCoeffRate(coeffrate, initialisationRequired);
  ///< Model mismatch is difference between predicted and actual coeff rates.
  PutModelRateDiff(_prevCoeffRatePred - GetMostRecentCoeffRate(), initialisationRequired);
  PutTotalRate(rate, initialisationRequired);
  PutHeaderRate(GetMostRecentTotalRate() - GetMostRecentCoeffRate(), initialisationRequired);

  if(!initialisationRequired) ///< /// Update the running sums associated with the model. 
  { ///< ------------------ Update ----------------------------------------------
    /// Update ((md/Dmax), r) running sums.
    double x = (_distortionSamples.GetBuffer())[0];
    double y = (_coeffRateSamples.GetBuffer())[0];

    double x2   = x*x;
    double px2  = _prevDistortion*_prevDistortion;
    _Sx   = UpdateRunningSum(_prevDistortion, x, _Sx);
    _Sy   = UpdateRunningSum(_prevCoeffRate, y, _Sy);
    _Sxy  = UpdateRunningSum(_prevCoeffRate/_prevDistortion, y/x, _Sxy);  ///< Inverse of distortion is required here.
    _Sx2  = UpdateRunningSum(px2, x2, _Sx2);

    /// Solve for a and b with least square error over the buffer frames.
    double bb = (((double)_numOfFrames*_Sy) - (_Sxy*_Sx))/(((double)_numOfFrames*_Sx2) - (_Sx*_Sx));
    double aa = (_Sxy - (bb*_Sx))/(double)_numOfFrames;

    /// TODO: Check the validity of aa and bb before assigning.

    _a = aa;
    _b = bb;

    /// Update mean diff running sums.
    _SEy  = UpdateRunningSum(_prevMeanDiff, (_meanDiffSamples.GetBuffer())[0], _SEy);
    _SEiy = 0.0;
    for(int i = 0; i < _numOfFrames; i++)
      _SEiy  += (_numOfFrames-i)*(_meanDiffSamples.GetBuffer())[i]; ///< The sample time series are in reverse order in the fifo.
    /// Update linear constants. Cramer's Rule.
    CramersRuleSoln(_SEi2, _SEi, _SEi, (double)_numOfFrames, _SEiy, _SEy, &_a1, &_a2);

    /// Update model diff running sums and moving average.
    _SMy  = UpdateRunningSum(_prevModelRateDiff, (_modelRateDiffSamples.GetBuffer())[0], _SMy);
    _SMiy = 0.0;
    for(int i = 0; i < _numOfFrames; i++)
      _SMiy  += (_numOfFrames-i)*(_modelRateDiffSamples.GetBuffer())[i]; ///< The sample time series are in reverse order in the fifo.
    /// Update linear constants. Cramer's Rule.
    CramersRuleSoln(_SEi2, _SEi, _SEi, (double)_numOfFrames, _SMiy, _SMy, &_m1, &_m2);
    _movingAvgCount++;
    if(_movingAvgCount > (1000 * _numOfFrames))   ///< Long term reset.
      _movingAvgCount = 1;    ///< Reset and leave the current moving avg unchanged.
    else
      _movingAvg = ((_modelRateDiffSamples.GetBuffer())[0] + ((double)(_movingAvgCount-1) * _movingAvg))/(double)_movingAvgCount;

    /// Update total rate running sums.
    _Sbuff = UpdateRunningSum(_prevTotalRate, (_totalRateSamples.GetBuffer())[0], _Sbuff);

    /// Update header rate running sums.
    _SPy  = UpdateRunningSum(_prevHeaderRate, (_headerRateSamples.GetBuffer())[0], _SPy);
    _SPiy = 0.0;
    for(int i = 0; i < _numOfFrames; i++)
      _SPiy  += (_numOfFrames-i)*(_headerRateSamples.GetBuffer())[i]; ///< The sample time series are in reverse order in the fifo.
    /// Update header rate linear constants. Cramer's Rule.
    CramersRuleSoln(_SEi2, _SEi, _SEi, (double)_numOfFrames, _SPiy, _SPy, &_p1, &_p2);

  }//end if !initialisationRequired...
  else  ///< Indicates that there was no valid data in the model before storing these first samples.
  { ///< ------------------ Initialise ----------------------------------------------
    /// Initialise the fifo buffers for the R-D model, Mean Diff and Model Diff prediction models.
    _Sx   = 0;
    _Sy   = 0;
    _Sxy  = 0;    ///< Quadratic R-D.
    _Sx2  = 0;

    _SEy  = 0.0;  ///< Linear Mean Diff
    _SEiy = 0.0;

    _SMy  = 0.0;  ///< Linear Model Diff
    _SMiy = 0.0;

    _Sbuff = 0.0; ///< Frame bit buffer.

    _SPy  = 0.0;  ///< Linear header rate
    _SPiy = 0.0;

    for(int i = 0; i < _numOfFrames; i++)
    {
      ///< Quadratic R-D.
      double x = (_distortionSamples.GetBuffer())[i];
      double y = (_coeffRateSamples.GetBuffer())[i];

      double x2 = x*x;
      _Sx   += x;
      _Sy   += y;
      _Sxy  += y/x;
      _Sx2  += x2;

      ///< Linear Mean Diff
      x = (_meanDiffSamples.GetBuffer())[i];
      /// _SEi and _SEi2 are constants.
      _SEy   += x;
      /// The sample time series are in reverse order in the fifo. x[fifo(0)] is x[_numOfFrames-1].
      /// The independent axis of the model describes samples at [1..N] where 1 is the oldest and N 
      /// from the last frame. Extrapolation will predict x[N+1].
      _SEiy  += (_numOfFrames-i)*x; ///< The sample time series are in reverse order in the fifo.

      ///< Linear Model Diff
      y = (_modelRateDiffSamples.GetBuffer())[i];
      /// _SEi and _SEi2 are constants.
      _SMy   += y;
      _SMiy  += (_numOfFrames-i)*y; ///< The sample time series are in reverse order in the fifo.
      _movingAvg = (_modelRateDiffSamples.GetBuffer())[0];
      _movingAvgCount = 1;

      /// Frame bit buffer.
      _Sbuff += (_totalRateSamples.GetBuffer())[i];

      /// Header rate.
      x = (_headerRateSamples.GetBuffer())[i];
      /// _SEi and _SEi2 are constants.
      _SPy   += x;
      /// The sample time series are in reverse order in the fifo. x[fifo(0)] is x[_numOfFrames-1].
      /// The independent axis of the model describes samples at [1..N] where 1 is the oldest and N 
      /// from the last frame. Extrapolation will predict x[N+1].
      _SPiy  += (_numOfFrames-i)*x; ///< The sample time series are in reverse order in the fifo.

    }//end for i...

  }//end else...

#ifdef RCIQ_DUMP_RATECNTL
  _MD           = GetMostRecentMeanDiff();
  _coeffRate    = GetMostRecentCoeffRate();
  _rate         = GetMostRecentTotalRate();
  _headerRate   = GetMostRecentHeaderRate();

  /// This method is the last to be called after processing a frame. Therefore dump here.
  if(_RCTablePos < _RCTableLen)
  {
    _RCTable.WriteItem(0, _RCTablePos, _RCTablePos);  ///< P-Frame
    _RCTable.WriteItem(1, _RCTablePos, (int)(0.5 + _predDmax)); ///< Dmax
    _RCTable.WriteItem(2, _RCTablePos, _rate);  ///< Rate
    _RCTable.WriteItem(3, _RCTablePos, _predCoeffRate);  ///< Pred Coeff Rate
    _RCTable.WriteItem(4, _RCTablePos, _coeffRate);  ///< Coeff Rate
    _RCTable.WriteItem(5, _RCTablePos, _predHeaderRate);  ///< Pred Head Rate
    _RCTable.WriteItem(6, _RCTablePos, _headerRate);  ///< Head Rate
    _RCTable.WriteItem(7, _RCTablePos, _predMD);  ///< Pred Mean Diff
    _RCTable.WriteItem(8, _RCTablePos, _MD);  ///< Mean Diff
    _RCTable.WriteItem(9, _RCTablePos, _a);  ///< Model parameter a
    _RCTable.WriteItem(10, _RCTablePos, _b);  ///< Model parameter b

    _RCTablePos++;
  }//end if _RCTablePos... 
#endif

}//end StoreMeasurements.

///---------------------- Pre-Encoding Predictions --------------------------

/** Predict the distortion for the next frame from the desired average rate.
The distortion measure is predicted from the coeff rate (total rate - header rate) using
an appropriate R-D model. The header rate and the signal mean diff are predicted from the
previous measured frame data using linear extrapolation.
@param  targetAvgRate : Total targeted average rate (including headers).
@param  rateLimit     : Upper limit to predicted rate.
@return               : Predicted distortion.
*/
int RateControlImplQuad::PredictDistortion(double targetAvgRate, double rateLimit)
{
  _outOfBoundsFlag = false;

  /// Default settings for the non-valid data case.
  double targetCoeffRate      = targetAvgRate;
  double targetCoeffRateLimit = rateLimit;
  double predHeaderRate       = 0.02;

  ///------------------ Rate Prediction --------------------------------------
  if(ValidData())
  {
    /// Buffer averaging model: What must the target rate for the
    /// next N frames be to make the average over the rate buffer equal
    /// to the average target rate specified in the parameter?

    /// N past frames and recover in k frames: target rate = target avg rate*(N+k)/k - total buff/k.
    int numRecoverFrames = _numOfFrames;
    double targetRate = ((targetAvgRate * (double)(_numOfFrames + numRecoverFrames))/(double)numRecoverFrames) - (_Sbuff/(double)numRecoverFrames);

    /// Check that the result is reasonable. If not then force it to some arbitrary minimum.
    if(targetRate < 0.0)  
      targetRate = (_headerRateSamples.GetBuffer())[0] + _rateLower;

    /// Predict the header rate with linear extrapolation model.
    predHeaderRate = (_p1*(double)(_numOfFrames+1)) + _p2;
     /// Check that the result is reasonable. If not, use the last entry.
    if( (predHeaderRate > targetAvgRate) || (predHeaderRate < 0.0) ) 
      predHeaderRate = (_headerRateSamples.GetBuffer())[0];          

    /// Remove the header rate to predict the coeff rate.
    targetCoeffRate = targetRate - predHeaderRate;
    targetCoeffRateLimit = rateLimit - predHeaderRate;

    /// Predict model mismatch diff with the active mismatch function.
    double mismatchPred;
    if(_modelMismatchType == LINEAR)  ///< Linear extrapolation model.
      mismatchPred = (_m1*(double)(_numOfFrames+1)) + _m2;
    else if(_modelMismatchType == CONSTANT)  ///< Moving average model (converging to a constant).
      mismatchPred = _movingAvg;
    else ///< if(_modelMismatchType == NONE)
      mismatchPred = 0.0;

    /// Modify the target rate with the predicted mismatch.
    targetCoeffRate = targetCoeffRate + mismatchPred;

  }//end if ValidData...
  else  ///< There is no valid data in the fifo buffers yet.
  {
    if(targetAvgRate > 0.02) ///< Guess header rate at 0.02 bpp.
      targetCoeffRate = targetAvgRate - 0.02;
  }//end else...

  /// Keep the target rate within the upper rate limit.
  if(targetCoeffRate > targetCoeffRateLimit)
    targetCoeffRate = targetCoeffRateLimit;

  /// No less than the coeff floor rate.
  if(targetCoeffRate < 0.00)
    targetCoeffRate = _rateLower;
  _prevCoeffRatePred = targetCoeffRate;

  ///--------------- Distortion Prediction -----------------------------------------

  /// Predict Mean Diff with linear extrapolation model.
  double predMD = (_a1*(double)(_numOfFrames+1)) + _a2;
  /// Can never be negative. Use last value if prediction is negative.
  if(predMD < 0.0)
    predMD = (_meanDiffSamples.GetBuffer())[0]; ///< Mean diff samples are in reverse order.

//  double lastDist = (_meanDiffSamples.GetBuffer())[0]*exp((_distortionSamples.GetBuffer())[0]);

  /// Make prediction.
  double distd = 0.0;
  double core = (_a*_a) + (4.0*_b*targetCoeffRate);
  if(core >= 0.0)
    distd = ModelDistortion(targetCoeffRate, _a, _b, predMD);
  else
    distd = (predMD*_a)/targetCoeffRate;

  /// Check bounds on distortion.
  if(distd > _distUpper)
  {
    distd = _distUpper;
    _outOfBoundsFlag = true;
  }//end if distd...
  /// Cannot be less than 16x16x(2^2)
  if(distd < _distLower)
  {
    distd = _distLower;
    _outOfBoundsFlag = true;
  }//end if distd...

  /// Update the target coeff rate with changes in Dmax boundaries.
  if(_outOfBoundsFlag)
  {
    targetCoeffRate = ModelRate(distd, _a, _b, predMD);
    _prevCoeffRatePred = targetCoeffRate;
  }//end if _outOfBoundsFlag...

#ifdef RCIQ_DUMP_RATECNTL
  _predHeaderRate   = predHeaderRate;
  _predCoeffRate    = targetCoeffRate;
  _predMD           = predMD;
  _predDmax         = distd;
#endif

  return((int)(distd + 0.5));
}//end PredictDistortion.

