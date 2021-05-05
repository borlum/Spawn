#ifndef INPUT_HH_INCLUDED
#define INPUT_HH_INCLUDED

#include "emsactuator.hpp"
#include "outputvariable.hpp"
#include "schedule.hpp"
#include "surface.hpp"
#include "zone.hpp"
#include "../variables.hpp"
#include "../iddtypes.hpp"
#include "../outputtypes.hpp"
#include "../../util/filesystem.hpp"
#include <fstream>

namespace spawn {

class Input {
public:
  explicit Input(const std::string & spawnInputJSON);

  std::vector<Zone> zones;
  std::vector<Schedule> schedules;
  std::vector<OutputVariable> outputVariables;
  std::vector<EMSActuator> emsActuators;
  std::vector<Surface> surfaces;

  [[nodiscard]] fs::path basepath() const;

  [[nodiscard]] std::string fmuname() const;
  void setFMUName(std::string name);
  // fmu name without extension
  [[nodiscard]] std::string fmuBaseName() const;

  [[nodiscard]] fs::path idfInputPath() const;
  void setIdfInputPath(fs::path idfpath);

  [[nodiscard]] fs::path epwInputPath() const;
  void setEPWInputPath(fs::path epwpath);

  double relativeSurfaceTolerance() const;

  void save(const fs::path & savepath) const;

private:

  // Return an expanded absolute path,
  // this will prepend basepath if the given pathstring is not absolute
  [[nodiscard]] fs::path toPath(const std::string & pathstring) const;

  nlohmann::json jsonidf;
  nlohmann::json spawnjson;
  fs::path m_basepath;
};

} // namespace spawn

#endif // INPUT_HH_INCLUDED

