// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2020 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Wei Hu, Radu Serban
// =============================================================================
//
// Definition of the SPH granular TERRAIN NODE (using Chrono::FSI).
//
// The global reference frame has Z up, X towards the front of the vehicle, and
// Y pointing to the left.
//
// =============================================================================

#ifndef TESTRIG_TERRAINNODE_GRANULAR_SPH_H
#define TESTRIG_TERRAINNODE_GRANULAR_SPH_H

#include "chrono/physics/ChSystemSMC.h"
#include "chrono_fsi/ChSystemFsi.h"

#include "chrono_vehicle/cosim/ChVehicleCosimTerrainNodeChrono.h"

namespace chrono {
namespace vehicle {

/// @addtogroup vehicle_cosim_chrono
/// @{

/// Definition of the SPH continuum representation of granular terrain node (using Chrono::FSI).
class CH_VEHICLE_API ChVehicleCosimTerrainNodeGranularSPH : public ChVehicleCosimTerrainNodeChrono {
  public:
    /// Create a Chrono::FSI granular SPH terrain node. Note that no SPH parameters are set.
    ChVehicleCosimTerrainNodeGranularSPH(unsigned int num_tires);

    /// Create a Chrono::FSI granular SPH terrain node using parameters from the provided JSON specfile.
    ChVehicleCosimTerrainNodeGranularSPH(const std::string& specfile, unsigned int num_tires);

    ~ChVehicleCosimTerrainNodeGranularSPH();

    virtual ChSystem* GetSystem() override { return m_system; }

    /// Set full terrain specification from JSON specfile.
    void SetFromSpecfile(const std::string& specfile);

    /// Specify the SPH terrain properties.
    void SetPropertiesSPH(const std::string& filename, double depth);

    /// Set properties of granular material.
    void SetGranularMaterial(double radius,    ///< particle radius (default: 0.01)
                             double density);  ///< particle material density (default: 2000)

  private:
    ChSystemSMC* m_system;          ///< containing system
    fsi::ChSystemFsi* m_systemFSI;  ///< containing FSI system

    std::shared_ptr<fsi::SimParams> m_params;  ///< FSI parameters
    double m_depth;                            ///< SPH soil depth

    double m_radius_g;             ///< radius of one particle of granular material
    double m_rho_g;                ///< particle material density

    virtual bool SupportsMeshInterface() const override { return false; }

    virtual void Construct() override;

    virtual void CreateWheelProxy(unsigned int i) override;
    virtual void UpdateWheelProxy(unsigned int i, const WheelState& wheel_state) override;
    virtual void GetForceWheelProxy(unsigned int i, TerrainForce& wheel_contact) override;

    virtual void OnOutputData(int frame) override;
    virtual void OnRender(double time) override;

    /// Advance simulation.
    /// This function is called after a synchronization to allow the node to advance
    /// its state by the specified time step.  A node is allowed to take as many internal
    /// integration steps as required, but no inter-node communication should occur.
    virtual void OnAdvance(double step_size) override;
};

}  // end namespace vehicle
}  // end namespace chrono

/// @} vehicle_cosim_chrono

#endif
