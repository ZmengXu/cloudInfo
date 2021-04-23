/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2012-2019 OpenFOAM Foundation
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

Class
    Foam::functionObjects::cloudInfo

Description
    Refer to $FOAM_SRC/functionObjects/lagrangian/cloudInfo
    Outputs Lagrangian cloud information to a file.

    The current outputs include:
    - total current number of parcels
    - total current mass of parcels
    - penetration length

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_cloudInfos_H
#define functionObjects_cloudInfos_H

//#include "regionFunctionObject.H"
#include "writeFiles.H"
#include "vector.H"

#include "Cloud.H"
#include "KinematicCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class cloudInfo Declaration
\*---------------------------------------------------------------------------*/
template<class CloudType>
class cloudInfo
:
//    public regionFunctionObject,
    public writeFiles
{
protected:

    // Protected Member Functions
        vector  position_;
        scalar fraction_;
        //- File header information
        virtual void writeFileHeader(const label i);


public:

    //- Runtime type information
    TypeName("cloudInfo");


    // Constructors

        //- Construct from Time and dictionary
        cloudInfo
        (
            const word& name,
            const Time& runTime,
            const dictionary&
        );

        //- Disallow default bitwise copy construction
        cloudInfo(const cloudInfo&) = delete;


    //- Destructor
    virtual ~cloudInfo();


    // Member Functions

        //- Read the controls
        virtual bool read(const dictionary&);

        //- Execute, currently does nothing
        virtual bool execute();

        //- Write
        virtual bool write();

        // Check
        //- Total mass in the system
        scalar massInSystem(const CloudType& cloud) const;
        //- Penetration for fraction [0-1] of the current total mass
        scalar penetration(const CloudType& cloud, const vector position0, const scalar fraction) const;

    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const cloudInfo&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
} // End namespace functionObjects
} // End namespace Foam

#ifdef NoRepository
    #include "cloudInfo.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
