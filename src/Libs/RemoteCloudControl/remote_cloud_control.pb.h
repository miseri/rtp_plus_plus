// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: remote_cloud_control.proto

#ifndef PROTOBUF_remote_5fcloud_5fcontrol_2eproto__INCLUDED
#define PROTOBUF_remote_5fcloud_5fcontrol_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace rtp_plus_plus {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_remote_5fcloud_5fcontrol_2eproto();
void protobuf_AssignDesc_remote_5fcloud_5fcontrol_2eproto();
void protobuf_ShutdownFile_remote_5fcloud_5fcontrol_2eproto();

class CalleeInfo;
class Response;

// ===================================================================

class CalleeInfo : public ::google::protobuf::Message {
 public:
  CalleeInfo();
  virtual ~CalleeInfo();

  CalleeInfo(const CalleeInfo& from);

  inline CalleeInfo& operator=(const CalleeInfo& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CalleeInfo& default_instance();

  void Swap(CalleeInfo* other);

  // implements Message ----------------------------------------------

  inline CalleeInfo* New() const { return New(NULL); }

  CalleeInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CalleeInfo& from);
  void MergeFrom(const CalleeInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(CalleeInfo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string sip_uri = 1;
  inline void clear_sip_uri();
  static const int kSipUriFieldNumber = 1;
  inline const ::std::string& sip_uri() const;
  inline void set_sip_uri(const ::std::string& value);
  inline void set_sip_uri(const char* value);
  inline void set_sip_uri(const char* value, size_t size);
  inline ::std::string* mutable_sip_uri();
  inline ::std::string* release_sip_uri();
  inline void set_allocated_sip_uri(::std::string* sip_uri);

  // optional int32 call_duration = 2;
  inline void clear_call_duration();
  static const int kCallDurationFieldNumber = 2;
  inline ::google::protobuf::int32 call_duration() const;
  inline void set_call_duration(::google::protobuf::int32 value);

  // optional int32 transport = 3;
  inline void clear_transport();
  static const int kTransportFieldNumber = 3;
  inline ::google::protobuf::int32 transport() const;
  inline void set_transport(::google::protobuf::int32 value);

  // optional string video_media_type = 4;
  inline void clear_video_media_type();
  static const int kVideoMediaTypeFieldNumber = 4;
  inline const ::std::string& video_media_type() const;
  inline void set_video_media_type(const ::std::string& value);
  inline void set_video_media_type(const char* value);
  inline void set_video_media_type(const char* value, size_t size);
  inline ::std::string* mutable_video_media_type();
  inline ::std::string* release_video_media_type();
  inline void set_allocated_video_media_type(::std::string* video_media_type);

  // optional string video_device = 5;
  inline void clear_video_device();
  static const int kVideoDeviceFieldNumber = 5;
  inline const ::std::string& video_device() const;
  inline void set_video_device(const ::std::string& value);
  inline void set_video_device(const char* value);
  inline void set_video_device(const char* value, size_t size);
  inline ::std::string* mutable_video_device();
  inline ::std::string* release_video_device();
  inline void set_allocated_video_device(::std::string* video_device);

  // optional int32 fps = 6;
  inline void clear_fps();
  static const int kFpsFieldNumber = 6;
  inline ::google::protobuf::int32 fps() const;
  inline void set_fps(::google::protobuf::int32 value);

  // optional int32 kbps = 7;
  inline void clear_kbps();
  static const int kKbpsFieldNumber = 7;
  inline ::google::protobuf::int32 kbps() const;
  inline void set_kbps(::google::protobuf::int32 value);

  // optional int32 rtx_time = 8;
  inline void clear_rtx_time();
  static const int kRtxTimeFieldNumber = 8;
  inline ::google::protobuf::int32 rtx_time() const;
  inline void set_rtx_time(::google::protobuf::int32 value);

  // optional int32 buf_lat = 9;
  inline void clear_buf_lat();
  static const int kBufLatFieldNumber = 9;
  inline ::google::protobuf::int32 buf_lat() const;
  inline void set_buf_lat(::google::protobuf::int32 value);

  // optional int32 rapid_sync_mode = 10;
  inline void clear_rapid_sync_mode();
  static const int kRapidSyncModeFieldNumber = 10;
  inline ::google::protobuf::int32 rapid_sync_mode() const;
  inline void set_rapid_sync_mode(::google::protobuf::int32 value);

  // optional int32 rtcp_sr = 11;
  inline void clear_rtcp_sr();
  static const int kRtcpSrFieldNumber = 11;
  inline ::google::protobuf::int32 rtcp_sr() const;
  inline void set_rtcp_sr(::google::protobuf::int32 value);

  // optional string rtcp_fb = 12;
  inline void clear_rtcp_fb();
  static const int kRtcpFbFieldNumber = 12;
  inline const ::std::string& rtcp_fb() const;
  inline void set_rtcp_fb(const ::std::string& value);
  inline void set_rtcp_fb(const char* value);
  inline void set_rtcp_fb(const char* value, size_t size);
  inline ::std::string* mutable_rtcp_fb();
  inline ::std::string* release_rtcp_fb();
  inline void set_allocated_rtcp_fb(::std::string* rtcp_fb);

  // @@protoc_insertion_point(class_scope:rtp_plus_plus.CalleeInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr sip_uri_;
  ::google::protobuf::int32 call_duration_;
  ::google::protobuf::int32 transport_;
  ::google::protobuf::internal::ArenaStringPtr video_media_type_;
  ::google::protobuf::internal::ArenaStringPtr video_device_;
  ::google::protobuf::int32 fps_;
  ::google::protobuf::int32 kbps_;
  ::google::protobuf::int32 rtx_time_;
  ::google::protobuf::int32 buf_lat_;
  ::google::protobuf::int32 rapid_sync_mode_;
  ::google::protobuf::int32 rtcp_sr_;
  ::google::protobuf::internal::ArenaStringPtr rtcp_fb_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_remote_5fcloud_5fcontrol_2eproto();
  friend void protobuf_AssignDesc_remote_5fcloud_5fcontrol_2eproto();
  friend void protobuf_ShutdownFile_remote_5fcloud_5fcontrol_2eproto();

  void InitAsDefaultInstance();
  static CalleeInfo* default_instance_;
};
// -------------------------------------------------------------------

class Response : public ::google::protobuf::Message {
 public:
  Response();
  virtual ~Response();

  Response(const Response& from);

  inline Response& operator=(const Response& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Response& default_instance();

  void Swap(Response* other);

  // implements Message ----------------------------------------------

  inline Response* New() const { return New(NULL); }

  Response* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Response& from);
  void MergeFrom(const Response& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Response* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 response_code = 1;
  inline void clear_response_code();
  static const int kResponseCodeFieldNumber = 1;
  inline ::google::protobuf::int32 response_code() const;
  inline void set_response_code(::google::protobuf::int32 value);

  // optional string description = 2;
  inline void clear_description();
  static const int kDescriptionFieldNumber = 2;
  inline const ::std::string& description() const;
  inline void set_description(const ::std::string& value);
  inline void set_description(const char* value);
  inline void set_description(const char* value, size_t size);
  inline ::std::string* mutable_description();
  inline ::std::string* release_description();
  inline void set_allocated_description(::std::string* description);

  // @@protoc_insertion_point(class_scope:rtp_plus_plus.Response)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr description_;
  ::google::protobuf::int32 response_code_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_remote_5fcloud_5fcontrol_2eproto();
  friend void protobuf_AssignDesc_remote_5fcloud_5fcontrol_2eproto();
  friend void protobuf_ShutdownFile_remote_5fcloud_5fcontrol_2eproto();

  void InitAsDefaultInstance();
  static Response* default_instance_;
};
// ===================================================================


// ===================================================================

// CalleeInfo

// optional string sip_uri = 1;
inline void CalleeInfo::clear_sip_uri() {
  sip_uri_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CalleeInfo::sip_uri() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.sip_uri)
  return sip_uri_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CalleeInfo::set_sip_uri(const ::std::string& value) {
  
  sip_uri_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.sip_uri)
}
inline void CalleeInfo::set_sip_uri(const char* value) {
  
  sip_uri_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:rtp_plus_plus.CalleeInfo.sip_uri)
}
inline void CalleeInfo::set_sip_uri(const char* value, size_t size) {
  
  sip_uri_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:rtp_plus_plus.CalleeInfo.sip_uri)
}
inline ::std::string* CalleeInfo::mutable_sip_uri() {
  
  // @@protoc_insertion_point(field_mutable:rtp_plus_plus.CalleeInfo.sip_uri)
  return sip_uri_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CalleeInfo::release_sip_uri() {
  
  return sip_uri_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CalleeInfo::set_allocated_sip_uri(::std::string* sip_uri) {
  if (sip_uri != NULL) {
    
  } else {
    
  }
  sip_uri_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), sip_uri);
  // @@protoc_insertion_point(field_set_allocated:rtp_plus_plus.CalleeInfo.sip_uri)
}

// optional int32 call_duration = 2;
inline void CalleeInfo::clear_call_duration() {
  call_duration_ = 0;
}
inline ::google::protobuf::int32 CalleeInfo::call_duration() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.call_duration)
  return call_duration_;
}
inline void CalleeInfo::set_call_duration(::google::protobuf::int32 value) {
  
  call_duration_ = value;
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.call_duration)
}

// optional int32 transport = 3;
inline void CalleeInfo::clear_transport() {
  transport_ = 0;
}
inline ::google::protobuf::int32 CalleeInfo::transport() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.transport)
  return transport_;
}
inline void CalleeInfo::set_transport(::google::protobuf::int32 value) {
  
  transport_ = value;
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.transport)
}

// optional string video_media_type = 4;
inline void CalleeInfo::clear_video_media_type() {
  video_media_type_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CalleeInfo::video_media_type() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.video_media_type)
  return video_media_type_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CalleeInfo::set_video_media_type(const ::std::string& value) {
  
  video_media_type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.video_media_type)
}
inline void CalleeInfo::set_video_media_type(const char* value) {
  
  video_media_type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:rtp_plus_plus.CalleeInfo.video_media_type)
}
inline void CalleeInfo::set_video_media_type(const char* value, size_t size) {
  
  video_media_type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:rtp_plus_plus.CalleeInfo.video_media_type)
}
inline ::std::string* CalleeInfo::mutable_video_media_type() {
  
  // @@protoc_insertion_point(field_mutable:rtp_plus_plus.CalleeInfo.video_media_type)
  return video_media_type_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CalleeInfo::release_video_media_type() {
  
  return video_media_type_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CalleeInfo::set_allocated_video_media_type(::std::string* video_media_type) {
  if (video_media_type != NULL) {
    
  } else {
    
  }
  video_media_type_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), video_media_type);
  // @@protoc_insertion_point(field_set_allocated:rtp_plus_plus.CalleeInfo.video_media_type)
}

// optional string video_device = 5;
inline void CalleeInfo::clear_video_device() {
  video_device_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CalleeInfo::video_device() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.video_device)
  return video_device_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CalleeInfo::set_video_device(const ::std::string& value) {
  
  video_device_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.video_device)
}
inline void CalleeInfo::set_video_device(const char* value) {
  
  video_device_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:rtp_plus_plus.CalleeInfo.video_device)
}
inline void CalleeInfo::set_video_device(const char* value, size_t size) {
  
  video_device_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:rtp_plus_plus.CalleeInfo.video_device)
}
inline ::std::string* CalleeInfo::mutable_video_device() {
  
  // @@protoc_insertion_point(field_mutable:rtp_plus_plus.CalleeInfo.video_device)
  return video_device_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CalleeInfo::release_video_device() {
  
  return video_device_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CalleeInfo::set_allocated_video_device(::std::string* video_device) {
  if (video_device != NULL) {
    
  } else {
    
  }
  video_device_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), video_device);
  // @@protoc_insertion_point(field_set_allocated:rtp_plus_plus.CalleeInfo.video_device)
}

// optional int32 fps = 6;
inline void CalleeInfo::clear_fps() {
  fps_ = 0;
}
inline ::google::protobuf::int32 CalleeInfo::fps() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.fps)
  return fps_;
}
inline void CalleeInfo::set_fps(::google::protobuf::int32 value) {
  
  fps_ = value;
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.fps)
}

// optional int32 kbps = 7;
inline void CalleeInfo::clear_kbps() {
  kbps_ = 0;
}
inline ::google::protobuf::int32 CalleeInfo::kbps() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.kbps)
  return kbps_;
}
inline void CalleeInfo::set_kbps(::google::protobuf::int32 value) {
  
  kbps_ = value;
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.kbps)
}

// optional int32 rtx_time = 8;
inline void CalleeInfo::clear_rtx_time() {
  rtx_time_ = 0;
}
inline ::google::protobuf::int32 CalleeInfo::rtx_time() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.rtx_time)
  return rtx_time_;
}
inline void CalleeInfo::set_rtx_time(::google::protobuf::int32 value) {
  
  rtx_time_ = value;
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.rtx_time)
}

// optional int32 buf_lat = 9;
inline void CalleeInfo::clear_buf_lat() {
  buf_lat_ = 0;
}
inline ::google::protobuf::int32 CalleeInfo::buf_lat() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.buf_lat)
  return buf_lat_;
}
inline void CalleeInfo::set_buf_lat(::google::protobuf::int32 value) {
  
  buf_lat_ = value;
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.buf_lat)
}

// optional int32 rapid_sync_mode = 10;
inline void CalleeInfo::clear_rapid_sync_mode() {
  rapid_sync_mode_ = 0;
}
inline ::google::protobuf::int32 CalleeInfo::rapid_sync_mode() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.rapid_sync_mode)
  return rapid_sync_mode_;
}
inline void CalleeInfo::set_rapid_sync_mode(::google::protobuf::int32 value) {
  
  rapid_sync_mode_ = value;
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.rapid_sync_mode)
}

// optional int32 rtcp_sr = 11;
inline void CalleeInfo::clear_rtcp_sr() {
  rtcp_sr_ = 0;
}
inline ::google::protobuf::int32 CalleeInfo::rtcp_sr() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.rtcp_sr)
  return rtcp_sr_;
}
inline void CalleeInfo::set_rtcp_sr(::google::protobuf::int32 value) {
  
  rtcp_sr_ = value;
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.rtcp_sr)
}

// optional string rtcp_fb = 12;
inline void CalleeInfo::clear_rtcp_fb() {
  rtcp_fb_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CalleeInfo::rtcp_fb() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.CalleeInfo.rtcp_fb)
  return rtcp_fb_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CalleeInfo::set_rtcp_fb(const ::std::string& value) {
  
  rtcp_fb_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:rtp_plus_plus.CalleeInfo.rtcp_fb)
}
inline void CalleeInfo::set_rtcp_fb(const char* value) {
  
  rtcp_fb_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:rtp_plus_plus.CalleeInfo.rtcp_fb)
}
inline void CalleeInfo::set_rtcp_fb(const char* value, size_t size) {
  
  rtcp_fb_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:rtp_plus_plus.CalleeInfo.rtcp_fb)
}
inline ::std::string* CalleeInfo::mutable_rtcp_fb() {
  
  // @@protoc_insertion_point(field_mutable:rtp_plus_plus.CalleeInfo.rtcp_fb)
  return rtcp_fb_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CalleeInfo::release_rtcp_fb() {
  
  return rtcp_fb_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CalleeInfo::set_allocated_rtcp_fb(::std::string* rtcp_fb) {
  if (rtcp_fb != NULL) {
    
  } else {
    
  }
  rtcp_fb_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), rtcp_fb);
  // @@protoc_insertion_point(field_set_allocated:rtp_plus_plus.CalleeInfo.rtcp_fb)
}

// -------------------------------------------------------------------

// Response

// optional int32 response_code = 1;
inline void Response::clear_response_code() {
  response_code_ = 0;
}
inline ::google::protobuf::int32 Response::response_code() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.Response.response_code)
  return response_code_;
}
inline void Response::set_response_code(::google::protobuf::int32 value) {
  
  response_code_ = value;
  // @@protoc_insertion_point(field_set:rtp_plus_plus.Response.response_code)
}

// optional string description = 2;
inline void Response::clear_description() {
  description_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Response::description() const {
  // @@protoc_insertion_point(field_get:rtp_plus_plus.Response.description)
  return description_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Response::set_description(const ::std::string& value) {
  
  description_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:rtp_plus_plus.Response.description)
}
inline void Response::set_description(const char* value) {
  
  description_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:rtp_plus_plus.Response.description)
}
inline void Response::set_description(const char* value, size_t size) {
  
  description_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:rtp_plus_plus.Response.description)
}
inline ::std::string* Response::mutable_description() {
  
  // @@protoc_insertion_point(field_mutable:rtp_plus_plus.Response.description)
  return description_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Response::release_description() {
  
  return description_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Response::set_allocated_description(::std::string* description) {
  if (description != NULL) {
    
  } else {
    
  }
  description_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), description);
  // @@protoc_insertion_point(field_set_allocated:rtp_plus_plus.Response.description)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace rtp_plus_plus

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_remote_5fcloud_5fcontrol_2eproto__INCLUDED
