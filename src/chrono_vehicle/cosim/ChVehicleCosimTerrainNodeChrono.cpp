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
// Authors: Radu Serban
// =============================================================================
//
// Base class for a TERRAIN NODE using a Chrono deformable soil formulation.
//
// The global reference frame has Z up, X towards the front of the vehicle, and
// Y pointing to the left.
//
// =============================================================================

#include <fstream>
#include <algorithm>
#include <cmath>

#include <mpi.h>

#include "chrono_vehicle/cosim/ChVehicleCosimTerrainNodeChrono.h"

#include "chrono_thirdparty/rapidjson/filereadstream.h"
#include "chrono_thirdparty/rapidjson/istreamwrapper.h"

using std::cout;
using std::endl;

using namespace rapidjson;

namespace chrono {
namespace vehicle {

std::string ChVehicleCosimTerrainNodeChrono::GetTypeAsString(ChVehicleCosimTerrainNodeChrono::Type type) {
    switch (type) {
        case Type::RIGID:
            return "RIGID";
        case Type::SCM:
            return "SCM";
        case Type::GRANULAR_OMP:
            return "GRANULAR_OMP";
        case Type::GRANULAR_GPU:
            return "GRANULAR_GPU";
        case Type::GRANULAR_MPI:
            return "GRANULAR_MPI";
        case Type::GRANULAR_SPH:
            return "GRANULAR_SPH";
        default:
            return "UNKNOWN";
    }
}

ChVehicleCosimTerrainNodeChrono::Type ChVehicleCosimTerrainNodeChrono::GetTypeFromString(const std::string& type) {
    if (type == "RIGID")
        return Type::RIGID;
    if (type == "SCM")
        return Type::SCM;
    if (type == "GRANULAR_OMP")
        return Type::GRANULAR_OMP;
    if (type == "GRANULAR_GPU")
        return Type::GRANULAR_GPU;
    if (type == "GRANULAR_MPI")
        return Type::GRANULAR_MPI;
    if (type == "GRANULAR_SPH")
        return Type::GRANULAR_SPH;

    return Type::UNKNOWN;
}

bool ChVehicleCosimTerrainNodeChrono::ReadSpecfile(const std::string& specfile, Document& d) {
    std::ifstream ifs(specfile);
    if (!ifs.good()) {
        cout << "ERROR: Could not open JSON file: " << specfile << "\n" << endl;
        return false;
    }

    IStreamWrapper isw(ifs);
    d.ParseStream<ParseFlag::kParseCommentsFlag>(isw);
    if (d.IsNull()) {
        cout << "ERROR: Invalid JSON file: " << specfile << "\n" << endl;
        return false;
    }

    return true;
}

ChVehicleCosimTerrainNodeChrono::Type ChVehicleCosimTerrainNodeChrono::GetTypeFromSpecfile(
    const std::string& specfile) {
    Document d;
    if (!ReadSpecfile(specfile, d)) {
        return Type::UNKNOWN;
    }

    if (!d.HasMember("Type")) {
        cout << "ERROR: JSON file " << specfile << " does not specify terrain type!\n" << endl;
        return Type::UNKNOWN;
    }

    return GetTypeFromString(d["Type"].GetString());
}

// -----------------------------------------------------------------------------
// Construction of the base Chrono terrain node.
// -----------------------------------------------------------------------------
ChVehicleCosimTerrainNodeChrono::ChVehicleCosimTerrainNodeChrono(Type type,
                                                                 ChContactMethod method,
                                                                 unsigned int num_tires)
    : ChVehicleCosimTerrainNode(num_tires), m_type(type), m_method(method), m_fixed_proxies(false) {
    m_material_tire.resize(num_tires);
    m_proxies.resize(num_tires);

    // Default terrain contact material
    switch (m_method) {
        case ChContactMethod::SMC:
            m_material_terrain = chrono_types::make_shared<ChMaterialSurfaceSMC>();
            break;
        case ChContactMethod::NSC:
            m_material_terrain = chrono_types::make_shared<ChMaterialSurfaceNSC>();
            break;
    }
}

// -----------------------------------------------------------------------------
// Initialization of the base Chrono terrain node:
// - complete system construction
// - create the "tire" material
// - create the appropriate proxy bodies (state not set yet)
// -----------------------------------------------------------------------------
void ChVehicleCosimTerrainNodeChrono::OnInitialize() {
    // Reset system time
    GetSystem()->SetChTime(0);

    // Construct the terrain
    Construct();

    for (unsigned int i = 0; i < m_num_tires; i++) {
        // Create the "tire" contact material, but defer using it until the proxy bodies are created.
        m_material_tire[i] = m_mat_props[i].CreateMaterial(m_method);

        // Create proxy bodies
        switch (m_interface_type) {
            case InterfaceType::BODY:
                CreateWheelProxy(i);
                break;
            case InterfaceType::MESH:
                CreateMeshProxies(i);
                break;
        }
    }
}

// -----------------------------------------------------------------------------
// Advance simulation of the Chrono terrain node by the specified duration
// -----------------------------------------------------------------------------
void ChVehicleCosimTerrainNodeChrono::OnAdvance(double step_size) {
    double t = 0;
    while (t < step_size) {
        double h = std::min<>(m_step_size, step_size - t);
        GetSystem()->DoStepDynamics(h);
        t += h;
    }
}

}  // end namespace vehicle
}  // end namespace chrono
