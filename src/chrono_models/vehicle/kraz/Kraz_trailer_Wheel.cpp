// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban, Rainer Gericke
// =============================================================================
//
// Kraz trailer wheel subsystem
//
// =============================================================================

#include <algorithm>

#include "chrono_models/vehicle/kraz/Kraz_trailer_Wheel.h"

#include "chrono_vehicle/ChVehicleModelData.h"
#include "chrono_thirdparty/filesystem/path.h"

namespace chrono {
namespace vehicle {
namespace kraz {

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------

const double Kraz_trailer_Wheel::m_mass = 30.0;
const ChVector<> Kraz_trailer_Wheel::m_inertia(.6, .63, .6);

const double Kraz_trailer_Wheel::m_radius = 0.28575;
const double Kraz_trailer_Wheel::m_width = 0.29845;

const std::string Kraz_trailer_Wheel::m_meshFile = "longhaul/meshes/SemiTrailer_rim.obj";

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Kraz_trailer_Wheel::Kraz_trailer_Wheel(const std::string& name) : ChWheel(name) {}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void Kraz_trailer_Wheel::AddVisualizationAssets(VisualizationType vis) {
    if (vis == chrono::vehicle::VisualizationType::MESH) {
        chrono::ChQuaternion<> rot = (m_side == VehicleSide::LEFT) ? Q_from_AngZ(0) : Q_from_AngZ(chrono::CH_C_PI);
        auto trimesh = geometry::ChTriangleMeshConnected::CreateFromWavefrontFile(GetDataFile(m_meshFile), false, false);
        trimesh->Transform(ChVector<>(0, m_offset, 0), ChMatrix33<>(rot));
        m_trimesh_shape = chrono_types::make_shared<ChTriangleMeshShape>();
        m_trimesh_shape->Pos = ChVector<>(0, m_offset, 0);
        m_trimesh_shape->Rot = ChMatrix33<>(rot);
        m_trimesh_shape->SetMesh(trimesh);
        m_trimesh_shape->SetStatic(true);
        m_trimesh_shape->SetName(filesystem::path(m_meshFile).stem());
        GetSpindle()->AddAsset(m_trimesh_shape);
    } else {
        ChWheel::AddVisualizationAssets(vis);
    }
}

void Kraz_trailer_Wheel::RemoveVisualizationAssets() {
    ChWheel::RemoveVisualizationAssets();

    // Make sure we only remove the assets added by SemiTrailer_wheel::AddVisualizationAssets.
    // This is important for the ChWheel object because a tire may add its own assets
    // to the same body (the spindle).
    auto it = std::find(GetSpindle()->GetAssets().begin(), GetSpindle()->GetAssets().end(), m_trimesh_shape);
    if (it != GetSpindle()->GetAssets().end())
        GetSpindle()->GetAssets().erase(it);
}

}  // end namespace kraz
}  // end namespace vehicle
}  // end namespace chrono
