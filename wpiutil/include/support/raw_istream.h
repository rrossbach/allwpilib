/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2015. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef WPIUTIL_SUPPORT_RAW_ISTREAM_H_
#define WPIUTIL_SUPPORT_RAW_ISTREAM_H_

#include <cstddef>

namespace wpi {

class raw_istream {
 public:
  raw_istream() = default;
  virtual ~raw_istream() = default;

  raw_istream& read(char& c) {
    read_impl(&c, 1);
    return *this;
  }

  raw_istream& read(unsigned char& c) {
    read_impl(&c, 1);
    return *this;
  }

  raw_istream& read(signed char& c) {
    read_impl(&c, 1);
    return *this;
  }

  raw_istream& read(void* data, std::size_t len) {
    read_impl(data, len);
    return *this;
  }

  virtual void close() = 0;

  bool has_error() const { return m_error; }
  void clear_error() { m_error = false; }

  raw_istream(const raw_istream&) = delete;
  raw_istream& operator=(const raw_istream&) = delete;

 protected:
  void error_detected() { m_error = true; }

 private:
  virtual void read_impl(void* data, std::size_t len) = 0;

  bool m_error = false;
};

class raw_mem_istream : public raw_istream {
 public:
  raw_mem_istream(const char* mem, std::size_t len) : m_cur(mem), m_left(len) {}
  void close() override;

 private:
  void read_impl(void* data, std::size_t len) override;

  const char* m_cur;
  std::size_t m_left;
};

}  // namespace wpi

#endif  // WPIUTIL_SUPPORT_RAW_ISTREAM_H_
