// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_HEADER_SURELOG_CACHE_H_
#define FLATBUFFERS_GENERATED_HEADER_SURELOG_CACHE_H_

#include "flatbuffers/flatbuffers.h"

namespace SURELOG {
namespace CACHE {

struct Header;

struct Error;

struct Location;

struct TimeInfo;

struct Header FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_M_SL_VERSION = 4,
    VT_M_FLB_VERSION = 6,
    VT_M_SL_DATE_COMPILED = 8,
    VT_M_FILE_DATE_COMPILED = 10,
    VT_M_FILE = 12
  };
  const flatbuffers::String *m_sl_version() const {
    return GetPointer<const flatbuffers::String *>(VT_M_SL_VERSION);
  }
  const flatbuffers::String *m_flb_version() const {
    return GetPointer<const flatbuffers::String *>(VT_M_FLB_VERSION);
  }
  const flatbuffers::String *m_sl_date_compiled() const {
    return GetPointer<const flatbuffers::String *>(VT_M_SL_DATE_COMPILED);
  }
  const flatbuffers::String *m_file_date_compiled() const {
    return GetPointer<const flatbuffers::String *>(VT_M_FILE_DATE_COMPILED);
  }
  const flatbuffers::String *m_file() const {
    return GetPointer<const flatbuffers::String *>(VT_M_FILE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_M_SL_VERSION) &&
           verifier.Verify(m_sl_version()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_M_FLB_VERSION) &&
           verifier.Verify(m_flb_version()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_M_SL_DATE_COMPILED) &&
           verifier.Verify(m_sl_date_compiled()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_M_FILE_DATE_COMPILED) &&
           verifier.Verify(m_file_date_compiled()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_M_FILE) &&
           verifier.Verify(m_file()) &&
           verifier.EndTable();
  }
};

struct HeaderBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_m_sl_version(flatbuffers::Offset<flatbuffers::String> m_sl_version) {
    fbb_.AddOffset(Header::VT_M_SL_VERSION, m_sl_version);
  }
  void add_m_flb_version(flatbuffers::Offset<flatbuffers::String> m_flb_version) {
    fbb_.AddOffset(Header::VT_M_FLB_VERSION, m_flb_version);
  }
  void add_m_sl_date_compiled(flatbuffers::Offset<flatbuffers::String> m_sl_date_compiled) {
    fbb_.AddOffset(Header::VT_M_SL_DATE_COMPILED, m_sl_date_compiled);
  }
  void add_m_file_date_compiled(flatbuffers::Offset<flatbuffers::String> m_file_date_compiled) {
    fbb_.AddOffset(Header::VT_M_FILE_DATE_COMPILED, m_file_date_compiled);
  }
  void add_m_file(flatbuffers::Offset<flatbuffers::String> m_file) {
    fbb_.AddOffset(Header::VT_M_FILE, m_file);
  }
  HeaderBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  HeaderBuilder &operator=(const HeaderBuilder &);
  flatbuffers::Offset<Header> Finish() {
    const auto end = fbb_.EndTable(start_, 5);
    auto o = flatbuffers::Offset<Header>(end);
    return o;
  }
};

inline flatbuffers::Offset<Header> CreateHeader(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> m_sl_version = 0,
    flatbuffers::Offset<flatbuffers::String> m_flb_version = 0,
    flatbuffers::Offset<flatbuffers::String> m_sl_date_compiled = 0,
    flatbuffers::Offset<flatbuffers::String> m_file_date_compiled = 0,
    flatbuffers::Offset<flatbuffers::String> m_file = 0) {
  HeaderBuilder builder_(_fbb);
  builder_.add_m_file(m_file);
  builder_.add_m_file_date_compiled(m_file_date_compiled);
  builder_.add_m_sl_date_compiled(m_sl_date_compiled);
  builder_.add_m_flb_version(m_flb_version);
  builder_.add_m_sl_version(m_sl_version);
  return builder_.Finish();
}

inline flatbuffers::Offset<Header> CreateHeaderDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *m_sl_version = nullptr,
    const char *m_flb_version = nullptr,
    const char *m_sl_date_compiled = nullptr,
    const char *m_file_date_compiled = nullptr,
    const char *m_file = nullptr) {
  return SURELOG::CACHE::CreateHeader(
      _fbb,
      m_sl_version ? _fbb.CreateString(m_sl_version) : 0,
      m_flb_version ? _fbb.CreateString(m_flb_version) : 0,
      m_sl_date_compiled ? _fbb.CreateString(m_sl_date_compiled) : 0,
      m_file_date_compiled ? _fbb.CreateString(m_file_date_compiled) : 0,
      m_file ? _fbb.CreateString(m_file) : 0);
}

struct Error FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_M_LOCATIONS = 4,
    VT_M_ERRORID = 6
  };
  const flatbuffers::Vector<flatbuffers::Offset<Location>> *m_locations() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Location>> *>(VT_M_LOCATIONS);
  }
  uint32_t m_errorId() const {
    return GetField<uint32_t>(VT_M_ERRORID, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_M_LOCATIONS) &&
           verifier.Verify(m_locations()) &&
           verifier.VerifyVectorOfTables(m_locations()) &&
           VerifyField<uint32_t>(verifier, VT_M_ERRORID) &&
           verifier.EndTable();
  }
};

struct ErrorBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_m_locations(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Location>>> m_locations) {
    fbb_.AddOffset(Error::VT_M_LOCATIONS, m_locations);
  }
  void add_m_errorId(uint32_t m_errorId) {
    fbb_.AddElement<uint32_t>(Error::VT_M_ERRORID, m_errorId, 0);
  }
  ErrorBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ErrorBuilder &operator=(const ErrorBuilder &);
  flatbuffers::Offset<Error> Finish() {
    const auto end = fbb_.EndTable(start_, 2);
    auto o = flatbuffers::Offset<Error>(end);
    return o;
  }
};

inline flatbuffers::Offset<Error> CreateError(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Location>>> m_locations = 0,
    uint32_t m_errorId = 0) {
  ErrorBuilder builder_(_fbb);
  builder_.add_m_errorId(m_errorId);
  builder_.add_m_locations(m_locations);
  return builder_.Finish();
}

inline flatbuffers::Offset<Error> CreateErrorDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Location>> *m_locations = nullptr,
    uint32_t m_errorId = 0) {
  return SURELOG::CACHE::CreateError(
      _fbb,
      m_locations ? _fbb.CreateVector<flatbuffers::Offset<Location>>(*m_locations) : 0,
      m_errorId);
}

struct Location FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_M_FILEID = 4,
    VT_M_LINE = 6,
    VT_M_COLUMN = 8,
    VT_M_OBJECT = 10
  };
  uint64_t m_fileId() const {
    return GetField<uint64_t>(VT_M_FILEID, 0);
  }
  uint32_t m_line() const {
    return GetField<uint32_t>(VT_M_LINE, 0);
  }
  uint16_t m_column() const {
    return GetField<uint16_t>(VT_M_COLUMN, 0);
  }
  uint64_t m_object() const {
    return GetField<uint64_t>(VT_M_OBJECT, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_M_FILEID) &&
           VerifyField<uint32_t>(verifier, VT_M_LINE) &&
           VerifyField<uint16_t>(verifier, VT_M_COLUMN) &&
           VerifyField<uint64_t>(verifier, VT_M_OBJECT) &&
           verifier.EndTable();
  }
};

struct LocationBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_m_fileId(uint64_t m_fileId) {
    fbb_.AddElement<uint64_t>(Location::VT_M_FILEID, m_fileId, 0);
  }
  void add_m_line(uint32_t m_line) {
    fbb_.AddElement<uint32_t>(Location::VT_M_LINE, m_line, 0);
  }
  void add_m_column(uint16_t m_column) {
    fbb_.AddElement<uint16_t>(Location::VT_M_COLUMN, m_column, 0);
  }
  void add_m_object(uint64_t m_object) {
    fbb_.AddElement<uint64_t>(Location::VT_M_OBJECT, m_object, 0);
  }
  LocationBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  LocationBuilder &operator=(const LocationBuilder &);
  flatbuffers::Offset<Location> Finish() {
    const auto end = fbb_.EndTable(start_, 4);
    auto o = flatbuffers::Offset<Location>(end);
    return o;
  }
};

inline flatbuffers::Offset<Location> CreateLocation(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t m_fileId = 0,
    uint32_t m_line = 0,
    uint16_t m_column = 0,
    uint64_t m_object = 0) {
  LocationBuilder builder_(_fbb);
  builder_.add_m_object(m_object);
  builder_.add_m_fileId(m_fileId);
  builder_.add_m_line(m_line);
  builder_.add_m_column(m_column);
  return builder_.Finish();
}

struct TimeInfo FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_M_TYPE = 4,
    VT_M_FILEID = 6,
    VT_M_LINE = 8,
    VT_M_TIMEUNIT = 10,
    VT_M_TIMEUNITVALUE = 12,
    VT_M_TIMEPRECISION = 14,
    VT_M_TIMEPRECISIONVALUE = 16
  };
  uint16_t m_type() const {
    return GetField<uint16_t>(VT_M_TYPE, 0);
  }
  uint64_t m_fileId() const {
    return GetField<uint64_t>(VT_M_FILEID, 0);
  }
  uint32_t m_line() const {
    return GetField<uint32_t>(VT_M_LINE, 0);
  }
  uint16_t m_timeUnit() const {
    return GetField<uint16_t>(VT_M_TIMEUNIT, 0);
  }
  double m_timeUnitValue() const {
    return GetField<double>(VT_M_TIMEUNITVALUE, 0.0);
  }
  uint16_t m_timePrecision() const {
    return GetField<uint16_t>(VT_M_TIMEPRECISION, 0);
  }
  double m_timePrecisionValue() const {
    return GetField<double>(VT_M_TIMEPRECISIONVALUE, 0.0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_M_TYPE) &&
           VerifyField<uint64_t>(verifier, VT_M_FILEID) &&
           VerifyField<uint32_t>(verifier, VT_M_LINE) &&
           VerifyField<uint16_t>(verifier, VT_M_TIMEUNIT) &&
           VerifyField<double>(verifier, VT_M_TIMEUNITVALUE) &&
           VerifyField<uint16_t>(verifier, VT_M_TIMEPRECISION) &&
           VerifyField<double>(verifier, VT_M_TIMEPRECISIONVALUE) &&
           verifier.EndTable();
  }
};

struct TimeInfoBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_m_type(uint16_t m_type) {
    fbb_.AddElement<uint16_t>(TimeInfo::VT_M_TYPE, m_type, 0);
  }
  void add_m_fileId(uint64_t m_fileId) {
    fbb_.AddElement<uint64_t>(TimeInfo::VT_M_FILEID, m_fileId, 0);
  }
  void add_m_line(uint32_t m_line) {
    fbb_.AddElement<uint32_t>(TimeInfo::VT_M_LINE, m_line, 0);
  }
  void add_m_timeUnit(uint16_t m_timeUnit) {
    fbb_.AddElement<uint16_t>(TimeInfo::VT_M_TIMEUNIT, m_timeUnit, 0);
  }
  void add_m_timeUnitValue(double m_timeUnitValue) {
    fbb_.AddElement<double>(TimeInfo::VT_M_TIMEUNITVALUE, m_timeUnitValue, 0.0);
  }
  void add_m_timePrecision(uint16_t m_timePrecision) {
    fbb_.AddElement<uint16_t>(TimeInfo::VT_M_TIMEPRECISION, m_timePrecision, 0);
  }
  void add_m_timePrecisionValue(double m_timePrecisionValue) {
    fbb_.AddElement<double>(TimeInfo::VT_M_TIMEPRECISIONVALUE, m_timePrecisionValue, 0.0);
  }
  TimeInfoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  TimeInfoBuilder &operator=(const TimeInfoBuilder &);
  flatbuffers::Offset<TimeInfo> Finish() {
    const auto end = fbb_.EndTable(start_, 7);
    auto o = flatbuffers::Offset<TimeInfo>(end);
    return o;
  }
};

inline flatbuffers::Offset<TimeInfo> CreateTimeInfo(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t m_type = 0,
    uint64_t m_fileId = 0,
    uint32_t m_line = 0,
    uint16_t m_timeUnit = 0,
    double m_timeUnitValue = 0.0,
    uint16_t m_timePrecision = 0,
    double m_timePrecisionValue = 0.0) {
  TimeInfoBuilder builder_(_fbb);
  builder_.add_m_timePrecisionValue(m_timePrecisionValue);
  builder_.add_m_timeUnitValue(m_timeUnitValue);
  builder_.add_m_fileId(m_fileId);
  builder_.add_m_line(m_line);
  builder_.add_m_timePrecision(m_timePrecision);
  builder_.add_m_timeUnit(m_timeUnit);
  builder_.add_m_type(m_type);
  return builder_.Finish();
}

}  // namespace CACHE
}  // namespace SURELOG

#endif  // FLATBUFFERS_GENERATED_HEADER_SURELOG_CACHE_H_




