/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    MSStageDriving.h
/// @author  Michael Behrisch
/// @date    Tue, 21 Apr 2015
///
// The common superclass for modelling transportable objects like persons and containers
/****************************************************************************/
#pragma once

// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <set>
#include <cassert>
#include <utils/common/SUMOTime.h>
#include <utils/common/SUMOVehicleClass.h>
#include <utils/geom/Position.h>
#include <utils/geom/PositionVector.h>
#include <utils/geom/Boundary.h>
#include <utils/router/SUMOAbstractRouter.h>
#include <utils/vehicle/SUMOTrafficObject.h>
#include "MSStage.h"


// ===========================================================================
// class declarations
// ===========================================================================
class MSEdge;
class MSLane;
class MSNet;
class MSStoppingPlace;
class MSVehicleType;
class OutputDevice;
class SUMOVehicleParameter;
class SUMOVehicle;
class MSTransportableDevice;
class MSTransportable;

typedef std::vector<const MSEdge*> ConstMSEdgeVector;

// ===========================================================================
// class definitions
// ===========================================================================
/**
* A "real" stage performing the travelling by a transport system
* The given route will be chosen. The travel time is computed by the simulation
*/
class MSStageDriving : public MSStage {
public:
    /// constructor
    MSStageDriving(const MSEdge* destination, MSStoppingPlace* toStop,
                   const double arrivalPos, const std::vector<std::string>& lines,
                   const std::string& intendedVeh = "", SUMOTime intendedDepart = -1);

    /// destructor
    virtual ~MSStageDriving();

    MSStage* clone() const;

    /// abort this stage (TraCI)
    void abort(MSTransportable* t);

    /// Returns the current edge
    const MSEdge* getEdge() const;
    const MSEdge* getFromEdge() const;
    double getEdgePos(SUMOTime now) const;

    ///
    Position getPosition(SUMOTime now) const;

    double getAngle(SUMOTime now) const;

    /// @brief get travel distance in this stage
    double getDistance() const {
        return myVehicleDistance;
    }

    /// @brief return (brief) string representation of the current stage
    std::string getStageDescription(const bool isPerson) const;

    /// @brief return string summary of the current stage
    std::string getStageSummary(const bool isPerson) const;

    /// proceeds to this stage
    void proceed(MSNet* net, MSTransportable* transportable, SUMOTime now, MSStage* previous);

    /** @brief Called on writing tripinfo output
     * @param[in] os The stream to write the information into
     * @exception IOError not yet implemented
     */
    void tripInfoOutput(OutputDevice& os, const MSTransportable* const transportable) const;

    /** @brief Called on writing vehroute output
     * @param[in] os The stream to write the information into
     * @param[in] withRouteLength whether route length shall be written
     * @exception IOError not yet implemented
     */
    void routeOutput(const bool isPerson, OutputDevice& os, const bool withRouteLength) const;

    /// Whether the person waits for the given vehicle
    bool isWaitingFor(const SUMOVehicle* vehicle) const;

    /// @brief Whether the person waits for a vehicle
    bool isWaiting4Vehicle() const;

    /// @brief Return where the person waits and for what
    std::string getWaitingDescription() const;

    SUMOVehicle* getVehicle() const {
        return myVehicle;
    }

    /// @brief time spent waiting for a ride
    SUMOTime getWaitingTime(SUMOTime now) const;

    double getSpeed() const;

    ConstMSEdgeVector getEdges() const;

    void setVehicle(SUMOVehicle* v);

    /// @brief marks arrival time and records driven distance
    const std::string setArrived(MSNet* net, MSTransportable* transportable, SUMOTime now);

    const std::set<std::string>& getLines() const {
        return myLines;
    }

    std::string getIntendedVehicleID() const {
        return myIntendedVehicleID;
    }

    SUMOTime getIntendedDepart() const {
        return myIntendedDepart;
    }

protected:
    /// the lines  to choose from
    const std::set<std::string> myLines;

    /// @brief The taken vehicle
    SUMOVehicle* myVehicle;
    /// @brief cached vehicle data for output after the vehicle has been removed
    std::string myVehicleID;
    std::string myVehicleLine;

    SUMOVehicleClass myVehicleVClass;
    double myVehicleDistance;

    double myWaitingPos;
    /// @brief The time since which this person is waiting for a ride
    SUMOTime myWaitingSince;
    const MSEdge* myWaitingEdge;
    Position myStopWaitPos;

    std::string myIntendedVehicleID;
    SUMOTime myIntendedDepart;

private:
    /// @brief Invalidated copy constructor.
    MSStageDriving(const MSStageDriving&);

    /// @brief Invalidated assignment operator.
    MSStageDriving& operator=(const MSStageDriving&) = delete;

};


/****************************************************************************/
