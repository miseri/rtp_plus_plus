# CMakeLists.txt in Codec Utils dir

INCLUDE_DIRECTORIES(
${artistIncludes}
${artist_SOURCE_DIR}/libs/GeneralUtils
${artist_SOURCE_DIR}/libs/ImageUtils
${artist_SOURCE_DIR}/libs/StreamUtils
${artist_SOURCE_DIR}/codecs/V264v2Codec
)

# Adds a library called Shared (libRtvcCodecUtils.a under Linux)
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_BUILD_FOR_SHORT")

IF(BUILD_VCODEC)
SET(VCODEC_SRCS
VectorQuantiserBaseImpl1.cpp
VectorQuantiserCbImpl1.cpp
VectorQuantiserCrImpl1.cpp
VectorQuantiserLumImpl1.cpp
)
ELSE(BUILD_VCODEC)
SET(VCODEC_SRCS
)
ENDIF(BUILD_VCODEC)

SET(CODEC_UTIL_HDRS
AdvancedIntraDctQuantiserImpl.h
AdvancedIntraDctQuantiserImplLookUp.h
AdvancedIntraModeH263VlcDecoder.h
AdvancedIntraModeH263VlcEncoder.h
BitStreamBase.h
BitStreamBaseMSB.h
BitStreamReader.h
BitStreamReaderMSB.h
BitStreamWriter.h
BitStreamWriterMSB.h
BlockH263.h
BlockH264.h
CAVLCH264Impl2.h
CAVLCH264Impl.h
CodecDistortionDef.h
CodecUtilsPch.h
CodedBlkPatternH264VlcDecoder.h
CodedBlkPatternH264VlcEncoder.h
CoeffTokenH264VlcDecoder.h
CoeffTokenH264VlcEncoder.h
ColourPlaneDecoding.h
ColourPlaneEncoding.h
DQuantH263VlcDecoder.h
DQuantH263VlcEncoder.h
DualMotionVectorTypeStruct.h
ExpGolombSignedVlcDecoder.h
ExpGolombSignedVlcEncoder.h
ExpGolombTruncVlcDecoder.h
ExpGolombTruncVlcEncoder.h
ExpGolombUnsignedVlcDecoder.h
ExpGolombUnsignedVlcEncoder.h
FastForward4x4ITImpl1.h
FastForward4x4ITImpl2.h
FastForward4x4On16x16ITImpl1.h
FastForwardDC2x2ITImpl1.h
FastForwardDC4x4ITImpl1.h
FastForwardDctImpl1.h
FastForwardDctImpl2.h
FastInterLastRunLevelH263VlcEncoder.h
FastIntraLastRunLevelH263VlcEncoder.h
FastInverse4x4ITImpl1.h
FastInverse4x4On16x16ITImpl1.h
FastInverseDC2x2ITImpl1.h
FastInverseDC4x4ITImpl1.h
FastInverseDctImpl1.h
FastInverseDctImplZDet2.h
FastInverseDctImplZDet.h
FastMotionVectorVlcDecoderImpl1.h
FastMotionVectorVlcDecoderImpl2.h
FastRunLengthVlcDecoderImpl1.h
FastRunLengthVlcDecoderImpl2.h
FastVectorQuantiserVlcDecoderImpl1.h
FastVectorQuantiserVlcDecoderImpl2.h
H264MbImgCache.h
H263MotionVectorPredictorImpl1.h
H264MotionVectorPredictorImpl1.h
H264RawFileHandler.h
IBitStreamReader.h
IBitStreamWriter.h
ICodecInnerAccess.h
ICodecv2.h
IContextAwareRunLevelCodec.h
IForwardDct.h
IForwardTransform.h
IInverseDct.h
IInverseTransform.h
ImagePlaneDecoder.h
ImagePlaneDecoderIntraImpl.h
ImagePlaneDecoderStdImpl.h
ImagePlaneEncoderConstQImpl.h
ImagePlaneEncoderFastImpl1.h
ImagePlaneEncoderFastImpl2.h
ImagePlaneEncoder.h
ImagePlaneEncoderIntraImpl.h
ImagePlaneEncoderStdImpl.h
IMotionCompensator.h
IMotionEstimator.h
IMotionVectorPredictor.h
InterPCBPYH263VlcDecoder.h
InterPCBPYH263VlcEncoder.h
InterPDctQuantiserImpl.h
InterPDctQuantiserImplLookUp.h
InterPLastRunLevH263Impl.h
InterPMCBPCH263VlcDecoder.h
InterPMCBPCH263VlcEncoder.h
IntraCBPYH263VlcDecoder.h
IntraCBPYH263VlcEncoder.h
IntraDCH263VlcDecoder.h
IntraDCH263VlcEncoder.h
IntraDctQuantiserImpl.h
IntraDctQuantiserImplLookUp.h
IntraLastRunLevelH263VlcDecoder.h
IntraLastRunLevelH263VlcEncoder.h
IntraLastRunLevH263Impl.h
IntraMCBPCH263VlcDecoder.h
IntraMCBPCH263VlcEncoder.h
IRateControl.h
IReEntry.h
IRunLengthCodec.h
IScalarQuantiser.h
IStreamHeader.h
IStreamHeaderReader.h
IVectorQuantiser.h
IVectorQuantiserVer2.h
IVlcDecoder.h
IVlcEncoder.h
IVQCodebookDim16.h
LastRunLevelH263List.h
LastRunLevelH263VlcDecoder.h
LastRunLevelH263VlcEncoder.h
LastRunLevelTypeStruct.h
MacroBlockH263.h
MacroBlockH264.h
ModDQuantH263VlcDecoder.h
ModDQuantH263VlcEncoder.h
ModQuantExtEscLevelH263VlcDecoder.h
ModQuantExtEscLevelH263VlcEncoder.h
MotionCompensatorH263ImplStd.h
MotionCompensatorH263ImplUnres.h
MotionCompensatorH264ImplMultires.h
MotionCompensatorH264ImplStd.h
MotionCompensatorImpl1.h
MotionEstimatorH263ImplE3SS.h
MotionEstimatorH263ImplMultiresCross.h
MotionEstimatorH263ImplMultiresCrossVer2.h
MotionEstimatorH263ImplStd.h
MotionEstimatorH263ImplUnres.h
MotionEstimatorH264ImplMultiresCross.h
MotionEstimatorH264ImplMultiresCrossVer2.h
MotionEstimatorH264ImplMultires.h
MotionEstimatorH264ImplFull.h
MotionEstimatorH264ImplTest.h
MotionEstimatorImpl1.h
MotionEstimatorImpl2.h
MotionEstimatorH264ImplMultiresCrossVer2.h
MotionVectorH263VlcDecoderImplRev.h
MotionVectorH263VlcDecoderImplStd.h
MotionVectorH263VlcEncoderImplRev.h
MotionVectorH263VlcEncoderImplStd.h
MotionVectorVlcDecoder.h
MotionVectorVlcEncoder.h
NalHeaderH264.h
PicParamSetH264.h
PrefixH264VlcDecoderImpl1.h
PrefixH264VlcEncoderImpl1.h
RateControlImpl1.h
RateControlImplPow.h
RateControlImplLog.h
RateControlImplMultiModel.h
RateControlImplQuad.h
RawFileHandlerBase.h
RleMotionVectorCodec.h
RleMotionVectorList.h
RunBeforeH264VlcDecoder.h
RunBeforeH264VlcEncoder.h
RunLengthVlcDecoder.h
RunLengthVlcEncoder.h
SeqParamSetH264.h
SimpleMotionVectorList.h
SliceHeaderH264.h
StreamSwitcher2.h
StreamSwitcher.h
StreamSwitcherVer3.h
SwitchCodecHeaderVer1.h
TotalZeros2x2H264VlcDecoder.h
TotalZeros2x2H264VlcEncoder.h
TotalZeros2x4H264VlcDecoder.h
TotalZeros2x4H264VlcEncoder.h
TotalZeros4x4H264VlcDecoder.h
TotalZeros4x4H264VlcEncoder.h
VectorList.h
VectorQuantiserBaseImpl1.h
VectorQuantiserCbImpl1.h
VectorQuantiserCrImpl1.h
VectorQuantiserDim16Impl1.h
VectorQuantiserDim16Impl2.h
VectorQuantiserDim16Impl3.h
VectorQuantiserLumImpl1.h
VectorQuantiserVer2Dim16Impl1.h
VectorQuantiserVlcDecoder.h
VectorQuantiserVlcEncoder.h
VectorStructList.h
YuvRawFileHandler.h
)

SET(CODEC_UTILS_SRCS
AdvancedIntraDctQuantiserImpl.cpp
AdvancedIntraDctQuantiserImplLookUp.cpp
AdvancedIntraModeH263VlcDecoder.cpp
AdvancedIntraModeH263VlcEncoder.cpp
BitStreamReader.cpp
BitStreamReaderMSB.cpp
BitStreamWriter.cpp
BitStreamWriterMSB.cpp
BlockH263.cpp
BlockH264.cpp
CAVLCH264Impl.cpp
CodecUtilsPch.cpp
CodedBlkPatternH264VlcDecoder.cpp
CodedBlkPatternH264VlcEncoder.cpp
#CodeFragmentForH264BlockLayerTest.cpp
CoeffTokenH264VlcDecoder.cpp
CoeffTokenH264VlcEncoder.cpp
ColourPlaneDecoding.cpp
ColourPlaneEncoding.cpp
DQuantH263VlcDecoder.cpp
DQuantH263VlcEncoder.cpp
ExpGolombUnsignedVlcDecoder.cpp
ExpGolombUnsignedVlcEncoder.cpp
FastForward4x4ITImpl1.cpp
FastForward4x4ITImpl2.cpp
FastForward4x4On16x16ITImpl1.cpp
FastForwardDC2x2ITImpl1.cpp
FastForwardDC4x4ITImpl1.cpp
FastForwardDctImpl1.cpp
FastForwardDctImpl2.cpp
FastInterLastRunLevelH263VlcEncoder.cpp
FastIntraLastRunLevelH263VlcEncoder.cpp
FastInverse4x4ITImpl1.cpp
FastInverse4x4On16x16ITImpl1.cpp
FastInverseDC2x2ITImpl1.cpp
FastInverseDC4x4ITImpl1.cpp
FastInverseDctImpl1.cpp
FastInverseDctImplZDet2.cpp
FastInverseDctImplZDet.cpp
FastMotionVectorVlcDecoderImpl1.cpp
FastMotionVectorVlcDecoderImpl2.cpp
FastRunLengthVlcDecoderImpl1.cpp
FastRunLengthVlcDecoderImpl2.cpp
FastVectorQuantiserVlcDecoderImpl1.cpp
FastVectorQuantiserVlcDecoderImpl2.cpp
H264MbImgCache.cpp
H264RawFileHandler.cpp
ImagePlaneDecoder.cpp
ImagePlaneDecoderIntraImpl.cpp
ImagePlaneDecoderStdImpl.cpp
ImagePlaneEncoderConstQImpl.cpp
ImagePlaneEncoder.cpp
ImagePlaneEncoderFastImpl1.cpp
ImagePlaneEncoderFastImpl2.cpp
ImagePlaneEncoderIntraImpl.cpp
ImagePlaneEncoderStdImpl.cpp
InterPCBPYH263VlcDecoder.cpp
InterPCBPYH263VlcEncoder.cpp
InterPDctQuantiserImpl.cpp
InterPDctQuantiserImplLookUp.cpp
InterPLastRunLevH263Impl.cpp
InterPMCBPCH263VlcDecoder.cpp
InterPMCBPCH263VlcEncoder.cpp
IntraCBPYH263VlcDecoder.cpp
IntraCBPYH263VlcEncoder.cpp
IntraDCH263VlcDecoder.cpp
IntraDCH263VlcEncoder.cpp
IntraDctQuantiserImpl.cpp
IntraDctQuantiserImplLookUp.cpp
IntraLastRunLevelH263VlcDecoder.cpp
IntraLastRunLevelH263VlcEncoder.cpp
IntraLastRunLevH263Impl.cpp
IntraMCBPCH263VlcDecoder.cpp
IntraMCBPCH263VlcEncoder.cpp
LastRunLevelH263List.cpp
LastRunLevelH263VlcDecoder.cpp
LastRunLevelH263VlcEncoder.cpp
MacroBlockH263.cpp
MacroBlockH264.cpp
ModDQuantH263VlcDecoder.cpp
ModDQuantH263VlcEncoder.cpp
ModQuantExtEscLevelH263VlcDecoder.cpp
ModQuantExtEscLevelH263VlcEncoder.cpp
MotionCompensatorH263ImplStd.cpp
MotionCompensatorH263ImplUnres.cpp
MotionCompensatorH264ImplStd.cpp
MotionCompensatorImpl1.cpp
MotionEstimatorH263ImplE3SS.cpp
MotionEstimatorH263ImplMultiresCross.cpp
MotionEstimatorH263ImplStd.cpp
MotionEstimatorH263ImplUnres.cpp
MotionEstimatorH264ImplTest.cpp
MotionEstimatorH264ImplFull.cpp
MotionEstimatorH264ImplMultires.cpp
MotionEstimatorH264ImplMultiresCross.cpp
MotionEstimatorH264ImplMultiresCrossVer2.cpp
MotionEstimatorImpl1.cpp
MotionEstimatorImpl2.cpp
MotionVectorH263VlcDecoderImplRev.cpp
MotionVectorH263VlcDecoderImplStd.cpp
MotionVectorH263VlcEncoderImplRev.cpp
MotionVectorH263VlcEncoderImplStd.cpp
MotionVectorVlcDecoder.cpp
MotionVectorVlcEncoder.cpp
NalHeaderH264.cpp
PicParamSetH264.cpp
RateControlImpl1.cpp
RateControlImplPow.cpp
RateControlImplLog.cpp
RateControlImplMultiModel.cpp
RateControlImplQuad.cpp
RawFileHandlerBase.cpp
RleMotionVectorCodec.cpp
RleMotionVectorList.cpp
RunBeforeH264VlcDecoder.cpp
RunBeforeH264VlcEncoder.cpp
RunLengthVlcDecoder.cpp
RunLengthVlcEncoder.cpp
SeqParamSetH264.cpp
SimpleMotionVectorList.cpp
SliceHeaderH264.cpp
StreamSwitcherVer3.cpp
SwitchCodecHeaderVer1.cpp
TotalZeros2x2H264VlcDecoder.cpp
TotalZeros2x2H264VlcEncoder.cpp
TotalZeros2x4H264VlcDecoder.cpp
TotalZeros2x4H264VlcEncoder.cpp
TotalZeros4x4H264VlcDecoder.cpp
TotalZeros4x4H264VlcEncoder.cpp
VectorList.cpp
VectorQuantiserDim16Impl1.cpp
VectorQuantiserDim16Impl2.cpp
VectorQuantiserDim16Impl3.cpp
VectorQuantiserVlcDecoder.cpp
VectorQuantiserVlcEncoder.cpp
VectorStructList.cpp
H263MotionVectorPredictorImpl1.cpp
MotionEstimatorH263ImplMultiresCrossVer2.cpp
YuvRawFileHandler.cpp
${VCODEC_SRCS}
)

ADD_LIBRARY( RtvcCodecUtils STATIC ${CODEC_UTILS_SRCS} ${CODEC_UTIL_HDRS})

if (CMAKE_BUILD_TYPE STREQUAL Debug)
SET_TARGET_PROPERTIES(RtvcCodecUtils PROPERTIES DEBUG_POSTFIX "D" )
message("CodecUtils Debug Build")
else (CMAKE_BUILD_TYPE STREQUAL Debug)
SET_TARGET_PROPERTIES(RtvcCodecUtils PROPERTIES DEBUG_POSTFIX "")
message("CodecUtils Release Build")
endif (CMAKE_BUILD_TYPE STREQUAL Debug)

