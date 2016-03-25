// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban
// =============================================================================
//
// Cosimulation node responsible for simulating a wheeled vehicle system.
//
// =============================================================================

#ifndef CH_COSIM_VEHICLE_NODE_H
#define CH_COSIM_VEHICLE_NODE_H

#include "mpi.h"

#include "chrono_vehicle/ChApiVehicle.h"
#include "chrono_vehicle/ChPowertrain.h"
#include "chrono_vehicle/ChDriver.h"
#include "chrono_vehicle/wheeled_vehicle/ChWheeledVehicle.h"

namespace chrono {
namespace vehicle {

/// @addtogroup vehicle_wheeled_cosim
/// @{

class CH_VEHICLE_API ChCosimVehicleNode {
  public:
    ChCosimVehicleNode(int rank, ChWheeledVehicle* vehicle, ChPowertrain* powertrain, ChDriver* driver);
    int GetNumberAxles() const { return m_vehicle->GetNumberAxles(); }

    void SetStepsize(double stepsize);
    void Initialize(const ChCoordsys<>& chassisPos);
    void Synchronize(double time);
    void Advance(double step);

  private:
    int m_rank;

    ChWheeledVehicle* m_vehicle;
    ChPowertrain* m_powertrain;
    ChDriver* m_driver;

    int m_num_wheels;
    WheelStates m_wheel_states;
    TireForces m_tire_forces;

    double m_stepsize;
};

/// @} vehicle_wheeled_cosim

}  // end namespace vehicle
}  // end namespace chrono

#endif
