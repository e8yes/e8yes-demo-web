// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message_queue_stats.proto

#include "message_queue_stats.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace e8 {
class MessageQueueStatsDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<MessageQueueStats>
      _instance;
} _MessageQueueStats_default_instance_;
}  // namespace e8
namespace protobuf_message_5fqueue_5fstats_2eproto {
static void InitDefaultsMessageQueueStats() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::e8::_MessageQueueStats_default_instance_;
    new (ptr) ::e8::MessageQueueStats();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::e8::MessageQueueStats::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_MessageQueueStats =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsMessageQueueStats}, {}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_MessageQueueStats.base);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::e8::MessageQueueStats, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::e8::MessageQueueStats, total_num_queues_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::e8::MessageQueueStats, num_queues_length_0_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::e8::MessageQueueStats, num_queues_length_1_10_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::e8::MessageQueueStats, num_queues_length_11_100_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::e8::MessageQueueStats, num_queues_length_101_1000_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::e8::MessageQueueStats, num_queues_length_gte_1001_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::e8::MessageQueueStats)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::e8::_MessageQueueStats_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "message_queue_stats.proto", schemas, file_default_instances, TableStruct::offsets,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\031message_queue_stats.proto\022\002e8\"\324\001\n\021Mess"
      "ageQueueStats\022\030\n\020total_num_queues\030\001 \001(\005\022"
      "\033\n\023num_queues_length_0\030\002 \001(\005\022\036\n\026num_queu"
      "es_length_1_10\030\003 \001(\005\022 \n\030num_queues_lengt"
      "h_11_100\030\004 \001(\005\022\"\n\032num_queues_length_101_"
      "1000\030\005 \001(\005\022\"\n\032num_queues_length_gte_1001"
      "\030\006 \001(\005b\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 254);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "message_queue_stats.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_message_5fqueue_5fstats_2eproto
namespace e8 {

// ===================================================================

void MessageQueueStats::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int MessageQueueStats::kTotalNumQueuesFieldNumber;
const int MessageQueueStats::kNumQueuesLength0FieldNumber;
const int MessageQueueStats::kNumQueuesLength110FieldNumber;
const int MessageQueueStats::kNumQueuesLength11100FieldNumber;
const int MessageQueueStats::kNumQueuesLength1011000FieldNumber;
const int MessageQueueStats::kNumQueuesLengthGte1001FieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

MessageQueueStats::MessageQueueStats()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_message_5fqueue_5fstats_2eproto::scc_info_MessageQueueStats.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:e8.MessageQueueStats)
}
MessageQueueStats::MessageQueueStats(const MessageQueueStats& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&total_num_queues_, &from.total_num_queues_,
    static_cast<size_t>(reinterpret_cast<char*>(&num_queues_length_gte_1001_) -
    reinterpret_cast<char*>(&total_num_queues_)) + sizeof(num_queues_length_gte_1001_));
  // @@protoc_insertion_point(copy_constructor:e8.MessageQueueStats)
}

void MessageQueueStats::SharedCtor() {
  ::memset(&total_num_queues_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&num_queues_length_gte_1001_) -
      reinterpret_cast<char*>(&total_num_queues_)) + sizeof(num_queues_length_gte_1001_));
}

MessageQueueStats::~MessageQueueStats() {
  // @@protoc_insertion_point(destructor:e8.MessageQueueStats)
  SharedDtor();
}

void MessageQueueStats::SharedDtor() {
}

void MessageQueueStats::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* MessageQueueStats::descriptor() {
  ::protobuf_message_5fqueue_5fstats_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_message_5fqueue_5fstats_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const MessageQueueStats& MessageQueueStats::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_message_5fqueue_5fstats_2eproto::scc_info_MessageQueueStats.base);
  return *internal_default_instance();
}


void MessageQueueStats::Clear() {
// @@protoc_insertion_point(message_clear_start:e8.MessageQueueStats)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&total_num_queues_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&num_queues_length_gte_1001_) -
      reinterpret_cast<char*>(&total_num_queues_)) + sizeof(num_queues_length_gte_1001_));
  _internal_metadata_.Clear();
}

bool MessageQueueStats::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:e8.MessageQueueStats)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 total_num_queues = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &total_num_queues_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 num_queues_length_0 = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &num_queues_length_0_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 num_queues_length_1_10 = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &num_queues_length_1_10_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 num_queues_length_11_100 = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(32u /* 32 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &num_queues_length_11_100_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 num_queues_length_101_1000 = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(40u /* 40 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &num_queues_length_101_1000_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 num_queues_length_gte_1001 = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(48u /* 48 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &num_queues_length_gte_1001_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:e8.MessageQueueStats)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:e8.MessageQueueStats)
  return false;
#undef DO_
}

void MessageQueueStats::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:e8.MessageQueueStats)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 total_num_queues = 1;
  if (this->total_num_queues() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->total_num_queues(), output);
  }

  // int32 num_queues_length_0 = 2;
  if (this->num_queues_length_0() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->num_queues_length_0(), output);
  }

  // int32 num_queues_length_1_10 = 3;
  if (this->num_queues_length_1_10() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->num_queues_length_1_10(), output);
  }

  // int32 num_queues_length_11_100 = 4;
  if (this->num_queues_length_11_100() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->num_queues_length_11_100(), output);
  }

  // int32 num_queues_length_101_1000 = 5;
  if (this->num_queues_length_101_1000() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(5, this->num_queues_length_101_1000(), output);
  }

  // int32 num_queues_length_gte_1001 = 6;
  if (this->num_queues_length_gte_1001() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(6, this->num_queues_length_gte_1001(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:e8.MessageQueueStats)
}

::google::protobuf::uint8* MessageQueueStats::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:e8.MessageQueueStats)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 total_num_queues = 1;
  if (this->total_num_queues() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->total_num_queues(), target);
  }

  // int32 num_queues_length_0 = 2;
  if (this->num_queues_length_0() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->num_queues_length_0(), target);
  }

  // int32 num_queues_length_1_10 = 3;
  if (this->num_queues_length_1_10() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->num_queues_length_1_10(), target);
  }

  // int32 num_queues_length_11_100 = 4;
  if (this->num_queues_length_11_100() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->num_queues_length_11_100(), target);
  }

  // int32 num_queues_length_101_1000 = 5;
  if (this->num_queues_length_101_1000() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(5, this->num_queues_length_101_1000(), target);
  }

  // int32 num_queues_length_gte_1001 = 6;
  if (this->num_queues_length_gte_1001() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(6, this->num_queues_length_gte_1001(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:e8.MessageQueueStats)
  return target;
}

size_t MessageQueueStats::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:e8.MessageQueueStats)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // int32 total_num_queues = 1;
  if (this->total_num_queues() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->total_num_queues());
  }

  // int32 num_queues_length_0 = 2;
  if (this->num_queues_length_0() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->num_queues_length_0());
  }

  // int32 num_queues_length_1_10 = 3;
  if (this->num_queues_length_1_10() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->num_queues_length_1_10());
  }

  // int32 num_queues_length_11_100 = 4;
  if (this->num_queues_length_11_100() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->num_queues_length_11_100());
  }

  // int32 num_queues_length_101_1000 = 5;
  if (this->num_queues_length_101_1000() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->num_queues_length_101_1000());
  }

  // int32 num_queues_length_gte_1001 = 6;
  if (this->num_queues_length_gte_1001() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->num_queues_length_gte_1001());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void MessageQueueStats::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:e8.MessageQueueStats)
  GOOGLE_DCHECK_NE(&from, this);
  const MessageQueueStats* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const MessageQueueStats>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:e8.MessageQueueStats)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:e8.MessageQueueStats)
    MergeFrom(*source);
  }
}

void MessageQueueStats::MergeFrom(const MessageQueueStats& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:e8.MessageQueueStats)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.total_num_queues() != 0) {
    set_total_num_queues(from.total_num_queues());
  }
  if (from.num_queues_length_0() != 0) {
    set_num_queues_length_0(from.num_queues_length_0());
  }
  if (from.num_queues_length_1_10() != 0) {
    set_num_queues_length_1_10(from.num_queues_length_1_10());
  }
  if (from.num_queues_length_11_100() != 0) {
    set_num_queues_length_11_100(from.num_queues_length_11_100());
  }
  if (from.num_queues_length_101_1000() != 0) {
    set_num_queues_length_101_1000(from.num_queues_length_101_1000());
  }
  if (from.num_queues_length_gte_1001() != 0) {
    set_num_queues_length_gte_1001(from.num_queues_length_gte_1001());
  }
}

void MessageQueueStats::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:e8.MessageQueueStats)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MessageQueueStats::CopyFrom(const MessageQueueStats& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:e8.MessageQueueStats)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MessageQueueStats::IsInitialized() const {
  return true;
}

void MessageQueueStats::Swap(MessageQueueStats* other) {
  if (other == this) return;
  InternalSwap(other);
}
void MessageQueueStats::InternalSwap(MessageQueueStats* other) {
  using std::swap;
  swap(total_num_queues_, other->total_num_queues_);
  swap(num_queues_length_0_, other->num_queues_length_0_);
  swap(num_queues_length_1_10_, other->num_queues_length_1_10_);
  swap(num_queues_length_11_100_, other->num_queues_length_11_100_);
  swap(num_queues_length_101_1000_, other->num_queues_length_101_1000_);
  swap(num_queues_length_gte_1001_, other->num_queues_length_gte_1001_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata MessageQueueStats::GetMetadata() const {
  protobuf_message_5fqueue_5fstats_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_message_5fqueue_5fstats_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace e8
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::e8::MessageQueueStats* Arena::CreateMaybeMessage< ::e8::MessageQueueStats >(Arena* arena) {
  return Arena::CreateInternal< ::e8::MessageQueueStats >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
