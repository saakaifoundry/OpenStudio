/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef SDD_REVERSETRANSLATOR_HPP
#define SDD_REVERSETRANSLATOR_HPP

#include "SDDAPI.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

#include "../model/Schedule.hpp"
#include "../model/ConstructionBase.hpp"

class QDomDocument;
class QDomElement;
class QDomNodeList;

namespace openstudio {

class ProgressBar;

namespace model {
  class Model;
  class ModelObject;
  class BuildingStory;
  class Space;
  class ShadingSurfaceGroup;
  class Surface;
  class SubSurface;
  class PlantLoop;
}

namespace sdd {

  class SDD_API ReverseTranslator {
  public:

    // If masterAutosize = true the translator will ignore choices in the sdd file and forcibly autosize the entire model.
    ReverseTranslator(bool masterAutosize = false);

    virtual ~ReverseTranslator();
    
    boost::optional<openstudio::model::Model> loadModel(const openstudio::path& path, ProgressBar* progressBar=nullptr);
  
    /** Get warning messages generated by the last translation. */
    std::vector<LogMessage> warnings() const;

    /** Get error messages generated by the last translation. */
    std::vector<LogMessage> errors() const;

  private:

    std::string escapeName(QString name);

    // listed in translation order
    boost::optional<openstudio::model::Model> convert(const QDomDocument& doc);
    boost::optional<openstudio::model::Model> translateSDD(const QDomElement& element, const QDomDocument& doc);
    boost::optional<openstudio::model::ModelObject> translateRunPeriod(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSite(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateWaterMainsTemperature(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    std::vector<openstudio::WorkspaceObject> translateDesignDays(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateWeatherFile(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateMaterial(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateConstructAssembly(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateScheduleDay(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateScheduleWeek(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSchedule(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateHoliday(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateDoorConstruction(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateFenestrationConstruction(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCrvCubic(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCrvDblQuad(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCrvQuad(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateFluidSys(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translatePump(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateBoiler(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateChiller(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateWtrHtr(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateHtRej(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateBuilding(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> createSpace(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> createThermalZone(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateBuildingStory(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSpace(const QDomElement& element, const QDomDocument& doc, openstudio::model::BuildingStory& buildingStory);
    boost::optional<openstudio::model::ModelObject> translateLoads(const QDomElement& element, const QDomDocument& doc, openstudio::model::Space& space);
    boost::optional<openstudio::model::ModelObject> translateSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::Space& space);
    boost::optional<openstudio::model::ModelObject> translateSubSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::Surface& surface);
    boost::optional<openstudio::model::ModelObject> translateShadingSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::ShadingSurfaceGroup& shadingSurfaceGroup);
    boost::optional<openstudio::model::ModelObject> translateAirSystem(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCoilCooling(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCoilHeating(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateFan(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateThermalZone(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateTrmlUnit(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateZnSys(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);

    // Looks for a loop in the SDD instance with a segment named fluidSegmentName
    // If found then looks for a model::Loop with that name and returns it
    // This is useful for hooking water coils up to their plant and maybe other things.
    boost::optional<model::PlantLoop> loopForSupplySegment(const QString & fluidSegmentName, const QDomDocument& doc, openstudio::model::Model& model);

    // Retruns the ServiceHotWater loop in the SDD instance with a segment named fluidSegmentName
    // If the loop is not found in the model, this function will attempt to translate it out of the SDD.
    // If the loop is found in the model it will simply be returned.
    boost::optional<model::PlantLoop> serviceHotWaterLoopForSupplySegment(const QString & fluidSegmentName, const QDomDocument & doc, openstudio::model::Model& model);

    // Return the "ZnSys" element with the name znSysName.
    QDomElement findZnSysElement(const QString & znSysName,const QDomDocument & doc);

    model::Schedule alwaysOnSchedule(openstudio::model::Model& model);
    boost::optional<model::Schedule> m_alwaysOnSchedule;

    model::Schedule defaultDeckTempSchedule(openstudio::model::Model& model);
    boost::optional<model::Schedule> m_defaultDeckTempSchedule;

    model::Schedule hotWaterPlantSetpointSchedule(openstudio::model::Model& model);
    boost::optional<model::Schedule> m_hotWaterPlantSetpointSchedule;

    model::Schedule chilledWaterPlantSetpointSchedule(openstudio::model::Model& model);
    boost::optional<model::Schedule> m_chilledWaterPlantSetpointSchedule;

    model::Schedule serviceHotWaterSetpointSchedule(openstudio::model::Model& model);
    boost::optional<model::Schedule> m_serviceHotWaterSetpointSchedule;

    model::ConstructionBase shadingConstruction(openstudio::model::Model& model, double solRefl, double visRefl);
    std::map<std::pair<double, double>, model::ConstructionBase> m_shadingConstructionMap;

    //helper method to do unit conversions; probably should be in OS proper
    boost::optional<double> unitToUnit(const double& val, const std::string& ipUnitString, const std::string& siUnitString);

    bool autosize() const;

    bool m_autosize;

    bool m_masterAutosize;

    StringStreamLogSink m_logSink;

    openstudio::path m_path;

    ProgressBar* m_progressBar;

    REGISTER_LOGGER("openstudio.sdd.ReverseTranslator");
  };

} // sdd
} // openstudio

#endif // SDD_REVERSETRANSLATOR_HPP
