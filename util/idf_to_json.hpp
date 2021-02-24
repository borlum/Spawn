#ifndef idf_to_json_hh_INCLUDED
#define idf_to_json_hh_INCLUDED

#include "../submodules/EnergyPlus/src/EnergyPlus/InputProcessing/IdfParser.hh"
#include "../submodules/EnergyPlus/src/EnergyPlus/InputProcessing/EmbeddedEpJSONSchema.hh"
#include "../submodules/EnergyPlus/src/EnergyPlus/DataStringGlobals.hh"
#include "../submodules/EnergyPlus/src/EnergyPlus/UtilityRoutines.hh"
#include "../submodules/EnergyPlus/third_party/nlohmann/json.hpp"
#include "./filesystem.hpp"

namespace spawn {

nlohmann::json idfToJSON(const fs::path & idfpath);

void jsonToIdf(const nlohmann::json & idfjson, const fs::path & idfpath);

} // namespace spawn

#endif // idf_to_json_hh_INCLUDED

