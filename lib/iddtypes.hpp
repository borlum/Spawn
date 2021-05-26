#ifndef IDDTYPES_HH_INCLUDED
#define IDDTYPES_HH_INCLUDED

#include <array>

namespace spawn {
constexpr std::array<const char *,4> supportedScheduleTypes = {
	"Schedule:Year",
	"Schedule:Compact",
	"Schedule:Constant",
	"Schedule:File"
};

constexpr std::array<const char *,315> supportedIDDTypes = {
  "Version",
  "SimulationControl",
  "PerformancePrecisionTradeoffs",
  "Building",
  "ShadowCalculation",
  "SurfaceConvectionAlgorithm:Inside",
  "SurfaceConvectionAlgorithm:Outside",
  "HeatBalanceAlgorithm",
  "HeatBalanceSettings:ConductionFiniteDifference",
  "ZoneAirHeatBalanceAlgorithm",
  "Timestep",
  "ConvergenceLimits",
  "Compliance:Building",
  "Site:Location",
  "Site:VariableLocation",
  "RunPeriod",
  "RunPeriodControl:SpecialDays",
  "RunPeriodControl:DaylightSavingTime",
  "WeatherProperty:SkyTemperature",
  "Site:WeatherStation",
  "Site:HeightVariation",
  "Site:GroundTemperature:BuildingSurface",
  "Site:GroundTemperature:FCfactorMethod",
  "Site:GroundTemperature:Shallow",
  "Site:GroundTemperature:Deep",
  "Site:GroundTemperature:Undisturbed:FiniteDifference",
  "Site:GroundTemperature:Undisturbed:KusudaAchenbach",
  "Site:GroundTemperature:Undisturbed:Xing",
  "Site:GroundDomain:Slab",
  "Site:GroundDomain:Basement",
  "Site:GroundReflectance",
  "Site:GroundReflectance:SnowModifier",
  "Site:WaterMainsTemperature",
  "Site:Precipitation",
  "Site:SolarAndVisibleSpectrum",
  "Site:SpectrumData",
  "ScheduleTypeLimits",
  "Schedule:Day:Hourly",
  "Schedule:Day:Interval",
  "Schedule:Day:List",
  "Schedule:Week:Daily",
  "Schedule:Week:Compact",
  "Schedule:Year",
  "Schedule:Compact",
  "Schedule:Constant",
  "Schedule:File:Shading",
  "Schedule:File",
  "Material",
  "Material:NoMass",
  "Material:InfraredTransparent",
  "Material:AirGap",
  "Material:RoofVegetation",
  "WindowMaterial:SimpleGlazingSystem",
  "WindowMaterial:Glazing",
  "WindowMaterial:GlazingGroup:Thermochromic",
  "WindowMaterial:Glazing:RefractionExtinctionMethod",
  "WindowMaterial:Gas",
  "WindowGap:SupportPillar",
  "WindowGap:DeflectionState",
  "WindowMaterial:GasMixture",
  "WindowMaterial:Gap",
  "WindowMaterial:Shade",
  "WindowMaterial:ComplexShade",
  "WindowMaterial:Blind",
  "WindowMaterial:Screen",
  "WindowMaterial:Shade:EquivalentLayer",
  "WindowMaterial:Drape:EquivalentLayer",
  "WindowMaterial:Blind:EquivalentLayer",
  "WindowMaterial:Screen:EquivalentLayer",
  "WindowMaterial:Glazing:EquivalentLayer",
  "WindowMaterial:Gap:EquivalentLayer",
  "MaterialProperty:MoisturePenetrationDepth:Settings",
  "MaterialProperty:PhaseChange",
  "MaterialProperty:PhaseChangeHysteresis",
  "MaterialProperty:VariableThermalConductivity",
  "MaterialProperty:HeatAndMoistureTransfer:Settings",
  "MaterialProperty:HeatAndMoistureTransfer:SorptionIsotherm",
  "MaterialProperty:HeatAndMoistureTransfer:Suction",
  "MaterialProperty:HeatAndMoistureTransfer:Redistribution",
  "MaterialProperty:HeatAndMoistureTransfer:Diffusion",
  "MaterialProperty:HeatAndMoistureTransfer:ThermalConductivity",
  "MaterialProperty:GlazingSpectralData",
  "Construction",
  "Construction:CfactorUndergroundWall",
  "Construction:FfactorGroundFloor",
  "Construction:InternalSource",
  "Construction:AirBoundary",
  "WindowThermalModel:Params",
  "WindowsCalculationEngine",
  "Construction:ComplexFenestrationState",
  "Construction:WindowEquivalentLayer",
  "Construction:WindowDataFile",
  "GlobalGeometryRules",
  "GeometryTransform",
  "Zone",
  "ZoneList",
  "ZoneGroup",
  "BuildingSurface:Detailed",
  "Wall:Detailed",
  "RoofCeiling:Detailed",
  "Floor:Detailed",
  "Wall:Exterior",
  "Wall:Adiabatic",
  "Wall:Underground",
  "Wall:Interzone",
  "Roof",
  "Ceiling:Adiabatic",
  "Ceiling:Interzone",
  "Floor:GroundContact",
  "Floor:Adiabatic",
  "Floor:Interzone",
  "FenestrationSurface:Detailed",
  "Window",
  "Door",
  "GlazedDoor",
  "Window:Interzone",
  "Door:Interzone",
  "GlazedDoor:Interzone",
  "WindowShadingControl",
  "WindowProperty:FrameAndDivider",
  "WindowProperty:AirflowControl",
  "WindowProperty:StormWindow",
  "InternalMass",
  "Shading:Site",
  "Shading:Building",
  "Shading:Site:Detailed",
  "Shading:Building:Detailed",
  "Shading:Overhang",
  "Shading:Overhang:Projection",
  "Shading:Fin",
  "Shading:Fin:Projection",
  "Shading:Zone:Detailed",
  "ShadingProperty:Reflectance",
  "SurfaceProperty:HeatTransferAlgorithm",
  "SurfaceProperty:HeatTransferAlgorithm:MultipleSurface",
  "SurfaceProperty:HeatTransferAlgorithm:SurfaceList",
  "SurfaceProperty:HeatTransferAlgorithm:Construction",
  "SurfaceProperty:HeatBalanceSourceTerm",
  "SurfaceControl:MovableInsulation",
  "SurfaceProperty:OtherSideCoefficients",
  "SurfaceProperty:OtherSideConditionsModel",
  "SurfaceProperty:Underwater",
  "Foundation:Kiva",
  "Foundation:Kiva:Settings",
  "SurfaceProperty:ExposedFoundationPerimeter",
  "SurfaceConvectionAlgorithm:Inside:AdaptiveModelSelections",
  "SurfaceConvectionAlgorithm:Outside:AdaptiveModelSelections",
  "SurfaceConvectionAlgorithm:Inside:UserCurve",
  "SurfaceConvectionAlgorithm:Outside:UserCurve",
  "SurfaceProperty:ConvectionCoefficients",
  "SurfaceProperty:ConvectionCoefficients:MultipleSurface",
  "SurfaceProperties:VaporCoefficients",
  "SurfaceProperty:ExteriorNaturalVentedCavity",
  "SurfaceProperty:SolarIncidentInside",
  "SurfaceProperty:LocalEnvironment",
  "ZoneProperty:LocalEnvironment",
  "SurfaceProperty:SurroundingSurfaces",
  "ComplexFenestrationProperty:SolarAbsorbedLayers",
  "ZoneProperty:UserViewFactors:bySurfaceName",
  "GroundHeatTransfer:Control",
  "GroundHeatTransfer:Slab:Materials",
  "GroundHeatTransfer:Slab:MatlProps",
  "GroundHeatTransfer:Slab:BoundConds",
  "GroundHeatTransfer:Slab:BldgProps",
  "GroundHeatTransfer:Slab:Insulation",
  "GroundHeatTransfer:Slab:EquivalentSlab",
  "GroundHeatTransfer:Slab:AutoGrid",
  "GroundHeatTransfer:Slab:ManualGrid",
  "GroundHeatTransfer:Slab:XFACE",
  "GroundHeatTransfer:Slab:YFACE",
  "GroundHeatTransfer:Slab:ZFACE",
  "GroundHeatTransfer:Basement:SimParameters",
  "GroundHeatTransfer:Basement:MatlProps",
  "GroundHeatTransfer:Basement:Insulation",
  "GroundHeatTransfer:Basement:SurfaceProps",
  "GroundHeatTransfer:Basement:BldgData",
  "GroundHeatTransfer:Basement:Interior",
  "GroundHeatTransfer:Basement:ComBldg",
  "GroundHeatTransfer:Basement:EquivSlab",
  "GroundHeatTransfer:Basement:EquivAutoGrid",
  "GroundHeatTransfer:Basement:AutoGrid",
  "GroundHeatTransfer:Basement:ManualGrid",
  "GroundHeatTransfer:Basement:XFACE",
  "GroundHeatTransfer:Basement:YFACE",
  "GroundHeatTransfer:Basement:ZFACE",
  "People",
  "ComfortViewFactorAngles",
  "Lights",
  "ElectricEquipment",
  "GasEquipment",
  "HotWaterEquipment",
  "SteamEquipment",
  "OtherEquipment",
  "Daylighting:Controls",
  "Daylighting:ReferencePoint",
  "Daylighting:DELight:ComplexFenestration",
  "DaylightingDevice:Tubular",
  "DaylightingDevice:Shelf",
  "DaylightingDevice:LightWell",
  "Output:DaylightFactors",
  "Output:IlluminanceMap",
  "OutputControl:IlluminanceMap:Style",
  "Exterior:Lights",
  "Exterior:FuelEquipment",
  "Exterior:WaterEquipment",
  "EnergyManagementSystem:Sensor",
  "EnergyManagementSystem:Actuator",
  "EnergyManagementSystem:ProgramCallingManager",
  "EnergyManagementSystem:Program",
  "EnergyManagementSystem:Subroutine",
  "EnergyManagementSystem:GlobalVariable",
  "EnergyManagementSystem:OutputVariable",
  "EnergyManagementSystem:MeteredOutputVariable",
  "EnergyManagementSystem:TrendVariable",
  "EnergyManagementSystem:InternalVariable",
  "EnergyManagementSystem:CurveOrTableIndexVariable",
  "EnergyManagementSystem:ConstructionIndexVariable",
  "Refrigeration:Case",
  "Refrigeration:CompressorRack",
  "Refrigeration:CaseAndWalkInList",
  "Refrigeration:Condenser:AirCooled",
  "Refrigeration:Condenser:EvaporativeCooled",
  "Refrigeration:Condenser:WaterCooled",
  "Refrigeration:Condenser:Cascade",
  "Refrigeration:GasCooler:AirCooled",
  "Refrigeration:TransferLoadList",
  "Refrigeration:Subcooler",
  "Refrigeration:Compressor",
  "Refrigeration:CompressorList",
  "Refrigeration:System",
  "Refrigeration:TranscriticalSystem",
  "Refrigeration:SecondarySystem",
  "Refrigeration:WalkIn",
  "Refrigeration:AirChiller",
  "Matrix:TwoDimension",
  "Curve:Linear",
  "Curve:QuadLinear",
  "Curve:Quadratic",
  "Curve:Cubic",
  "Curve:Quartic",
  "Curve:Exponent",
  "Curve:Bicubic",
  "Curve:Biquadratic",
  "Curve:QuadraticLinear",
  "Curve:CubicLinear",
  "Curve:Triquadratic",
  "Curve:Functional:PressureDrop",
  "Curve:FanPressureRise",
  "Curve:ExponentialSkewNormal",
  "Curve:Sigmoid",
  "Curve:RectangularHyperbola1",
  "Curve:RectangularHyperbola2",
  "Curve:ExponentialDecay",
  "Curve:DoubleExponentialDecay",
  "Curve:ChillerPartLoadWithLift",
  "Table:IndependentVariable",
  "Table:IndependentVariableList",
  "Table:Lookup",
  "FluidProperties:Name",
  "FluidProperties:GlycolConcentration",
  "FluidProperties:Temperatures",
  "FluidProperties:Saturated",
  "FluidProperties:Superheated",
  "FluidProperties:Concentration",
  "CurrencyType",
  "ComponentCost:Adjustments",
  "ComponentCost:Reference",
  "ComponentCost:LineItem",
  "UtilityCost:Tariff",
  "UtilityCost:Qualify",
  "UtilityCost:Charge:Simple",
  "UtilityCost:Charge:Block",
  "UtilityCost:Ratchet",
  "UtilityCost:Variable",
  "UtilityCost:Computation",
  "LifeCycleCost:Parameters",
  "LifeCycleCost:RecurringCosts",
  "LifeCycleCost:NonrecurringCost",
  "LifeCycleCost:UsePriceEscalation",
  "LifeCycleCost:UseAdjustment",
  "Parametric:SetValueForRun",
  "Parametric:Logic",
  "Parametric:RunControl",
  "Parametric:FileNameSuffix",
  "Output:VariableDictionary",
  "Output:Surfaces:List",
  "Output:Surfaces:Drawing",
  "Output:Schedules",
  "Output:Constructions",
  "Output:EnergyManagementSystem",
  "OutputControl:SurfaceColorScheme",
  "Output:Table:SummaryReports",
  "Output:Table:TimeBins",
  "Output:Table:Monthly",
  "Output:Table:Annual",
  "OutputControl:Table:Style",
  "OutputControl:ReportingTolerances",
  "Output:Variable",
  "Output:Meter",
  "Output:Meter:MeterFileOnly",
  "Output:Meter:Cumulative",
  "Output:Meter:Cumulative:MeterFileOnly",
  "Meter:Custom",
  "Meter:CustomDecrement",
  "Output:JSON",
  "Output:SQLite",
  "Output:EnvironmentalImpactFactors",
  "EnvironmentalImpactFactors",
  "FuelFactors",
  "Output:Diagnostics",
  "Output:DebuggingData",
  "Output:PreprocessorMessage",
  "ZoneInfiltration:DesignFlowRate",
  "ZoneInfiltration:EffectiveLeakageArea",
  "ZoneInfiltration:FlowCoefficient"
};
}

#endif // IDDTYPES_HH_INCLUDED

