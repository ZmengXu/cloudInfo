/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2012-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/
#ifndef cloudInfoFunctionObjects_H
#define cloudInfoFunctionObjects_H

#include "cloudInfo.H"
#include "basicKinematicParcel.H"
#include "basicKinematicCollidingParcel.H"
#include "basicKinematicMPPICParcel.H"
#include "basicReactingParcel.H"
#include "basicReactingMultiphaseParcel.H"
#include "basicThermoParcel.H"
#include "basicSprayParcel.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    typedef
        functionObjects::cloudInfo
        <
            Cloud
            <
                basicKinematicParcel
            >
        >
        basicKinematicCloudInfo;

    typedef
        functionObjects::cloudInfo
        <
            Cloud
            <
                basicKinematicCollidingParcel
            >
        >
        basicKinematicCollidingCloudInfo;

    typedef
        functionObjects::cloudInfo
        <
            Cloud
            <
                basicKinematicMPPICParcel
            >
        >
        basicKinematicMPPICCloudInfo;

    typedef
        functionObjects::cloudInfo
        <
            Cloud
            <
                basicReactingParcel
            >
        >
        basicReactingCloudInfo;

    typedef
        functionObjects::cloudInfo
        <
            Cloud
            <
                basicReactingMultiphaseParcel
            >
        >
        basicReactingMultiphaseCloudInfo;
        // coalParcel is another name of basicReactingMultiphaseParcel

    typedef
        functionObjects::cloudInfo
        <
            Cloud
            <
                basicThermoParcel
            >
        >
        basicThermoCloudInfo;

    typedef
        functionObjects::cloudInfo
        <
            Cloud
            <
                basicSprayParcel
            >
        >
        basicSprayCloudInfo;

}

#endif
// ************************************************************************* //
