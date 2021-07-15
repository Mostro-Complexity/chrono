// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2021 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban
// =============================================================================
//
// Wrapper for a vehicle joint (kinematic or bushing)
//
// =============================================================================

#ifndef CH_VEHICLE_JOINT_H
#define CH_VEHICLE_JOINT_H

#include "chrono_vehicle/ChApiVehicle.h"
#include "chrono/physics/ChBody.h"
#include "chrono/physics/ChLink.h"
#include "chrono/physics/ChLoadsBody.h"

namespace chrono {
namespace vehicle {

/// @addtogroup vehicle
/// @{

/// Stiffness and damping data for a vehicle bushing specification.
/// Attention! The Chrono bushing formulation is valid only for small relative rotations. As such, define a non-zero
/// rotational stiffness in a "DOF" direction only if the kinematics of the modeled mechanism prevent any large relative
/// rotations. For rotational stiffness in the presence of large rotations, use a ChLinkRSDA element.
struct ChVehicleBushingData {
    ChVehicleBushingData()
        : K_lin(0), K_rot(0), D_lin(0), D_rot(0), K_lin_dof(0), K_rot_dof(0), D_lin_dof(0), D_rot_dof(0) {}

    double K_lin;  ///< translational stiffness in "constrained" directions
    double K_rot;  ///< rotational stiffness in "constrained" directions
    double D_lin;  ///< translational damping in "constrained" directions
    double D_rot;  ///< rotational damping in "constraint" directions

    double K_lin_dof;  ///< translational stiffness in "DOF" directions
    double K_rot_dof;  ///< rotational stiffness in "DOF" directions
    double D_lin_dof;  ///< translational damping in "DOF" directions
    double D_rot_dof;  ///< rotational damping in "DOF" directions
};

/// Wrapper class for a joint in a vehicle system which can be either a kinematic joint or a bushing.
class CH_VEHICLE_API ChVehicleJoint {
  public:
    /// Supported types of vehicle joint.
    /// For a kinematic joint, the wrapped element will be a ChLink of the appropriate type. For a bushing, the
    /// stiffness and damping in the appropriate directions (as defined by the DOFs of the joint type) are set to a
    /// different value (typically 0).
    enum class Type { LOCK, SPHERICAL, REVOLUTE, UNIVERSAL };

    /// Construct a vehicle joint wrapper indicating if this is a kinematic joint or a bushing.
    /// Note that the caller is responsible for initializing the joint and adding it to the appropriate container
    /// (system or load container). Prefer using the utility static function ChVehicleJoint::Create which completely
    /// constructs the joint.
    ChVehicleJoint(bool kinematic);

    ~ChVehicleJoint();

    /// Utility function for creating a kinematic joint or a bushing of the specified type, with the given name,
    /// connecting the specified bodies at the given position (expressed in the absolute reference frame).  If no
    /// bushing data is provided (default), a kinematic joint is created; otherwise, this function creates a bushing.
    /// Note that the constructed element is not included in the simulation; for that, pass the joint to the
    /// ChChassis::AddJoint function which adds the joint to its appropriate container (the containing Chrono system for
    /// a kinematic joint or a load container managed by the chassis subsystem for a bushing).
    static std::shared_ptr<ChVehicleJoint> Create(Type type,
                                                  const std::string& name,
                                                  std::shared_ptr<ChBody> body1,
                                                  std::shared_ptr<ChBody> body2,
                                                  ChCoordsys<> pos,
                                                  std::shared_ptr<ChVehicleBushingData> bushing_data = nullptr);

    /// Get the current absolute position of the joint. This is the current absolute location of the underling marker on
    /// the 2nd connected body.
    ChVector<> GetPos() const;

    /// Get the current constraint violation. This will return an empty vector for a bushing element.
    ChVectorDynamic<> GetConstraintViolation() const;

    /// Return true if wrapping a kinematic joint and false if wrapping a bushing.
    bool IsKinematic() const { return m_kinematic; }

    /// Get the underlying kinematic joint. 
    /// A null pointer is returned if the vehicle joint is in fact a bushing!
    std::shared_ptr<ChLink> GetAsLink() const;

    /// Get the underlying bushing.
    /// A null pointer is returned if the vehicle joint is in fact a kinematic joint!
    std::shared_ptr<ChLoadBodyBody> GetAsBushing() const;

  private:
    void CreateLink(Type type, std::shared_ptr<ChBody> body1, std::shared_ptr<ChBody> body2, ChCoordsys<> pos);
    void CreateBushing(Type type,
                       std::shared_ptr<ChBody> body1,
                       std::shared_ptr<ChBody> body2,
                       ChCoordsys<> pos,
                       std::shared_ptr<ChVehicleBushingData> bd);

    bool m_kinematic;  ///< true if kinematic joint
    union {
        std::shared_ptr<ChLink> m_link;                           ///< kinematic joint variant
        std::shared_ptr<ChLoadBodyBodyBushingGeneric> m_bushing;  ///< bushing variant
    };

    friend class ChChassis;
};

/// @} vehicle

}  // namespace vehicle
}  // namespace chrono

#endif
