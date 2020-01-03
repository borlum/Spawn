#ifndef NREL_SPAWN_UTILITY_HPP
#define NREL_SPAWN_UTILITY_HPP

#include <cstdint>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>

namespace spawn {
// creates an RAII managed temporary directory
struct Temp_Directory
{
  explicit Temp_Directory(const std::string_view t_prefix = "spawn");
  ~Temp_Directory();

  const boost::filesystem::path &dir()
  {
    return m_dir;
  }

  Temp_Directory(Temp_Directory &&) = delete;
  Temp_Directory(const Temp_Directory &) = delete;
  Temp_Directory &operator=(const Temp_Directory &) = delete;
  Temp_Directory &operator=(Temp_Directory &&) = delete;

private:
  boost::filesystem::path m_dir;
};
} // namespace spawn

#endif
