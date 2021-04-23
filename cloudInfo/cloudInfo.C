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

#include "cloudInfo.H"
#include "dictionary.H"
#include "PstreamReduceOps.H"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //
template<class CloudType>
void Foam::functionObjects::cloudInfo<CloudType>::writeFileHeader(const label i)
{
    writeHeader(file(), "Cloud information");
    writeCommented(file(), "Time");
    writeTabbed(file(), "nParcels");
    writeTabbed(file(), "massInSystem");
    writeTabbed(file(), "penetration");
    file() << endl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
template<class CloudType>
Foam::functionObjects::cloudInfo<CloudType>::cloudInfo
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
//    regionFunctionObject(name, runTime, dict),
    writeFiles(name, runTime, dict, name),
    position_(dict.lookup("position")),
    fraction_(readScalar(dict.lookup("penetration")))
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //
template<class CloudType>
Foam::functionObjects::cloudInfo<CloudType>::~cloudInfo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
template<class CloudType>
bool Foam::functionObjects::cloudInfo<CloudType>::read(const dictionary& dict)
{
//    regionFunctionObject::read(dict);

    writeFiles::resetNames(dict.lookup("clouds"));

    Info<< type() << " " << name() << ": ";
    if (names().size())
    {
        Info<< "applying to clouds:" << nl;
        forAll(names(), i)
        {
            Info<< "    " << names()[i] << nl;
        }
        Info<< endl;
    }
    else
    {
        Info<< "no clouds to be processed" << nl << endl;
    }

    return true;
}

template<class CloudType>
bool Foam::functionObjects::cloudInfo<CloudType>::execute()
{
    return true;
}

template<class CloudType>
bool Foam::functionObjects::cloudInfo<CloudType>::write()
{
    writeFiles::write();

    forAll(names(), i)
    {
        const word& cloudName = names()[i];

        const CloudType& cloud =
            obr_.lookupObject<CloudType>(cloudName);
        label nParcels = returnReduce(cloud.size(), sumOp<label>());
        scalar mass = massInSystem(cloud);

        scalar pen = penetration(cloud, position_, fraction_);

        if (Pstream::master())
        {
            writeTime(file(i));
            file(i)
                << tab
                << nParcels << tab
                << mass << tab
                << pen << endl;
        }
    }

    return true;
}

template<class CloudType>
Foam::scalar Foam::functionObjects::cloudInfo<CloudType>::massInSystem
(
    const CloudType& cloud
) const
{

    label i = 0;
    scalar mSum = 0.0;

    forAllConstIter(typename CloudType, cloud, iter)
    {
        const typename CloudType::particleType& p = iter();
        scalar m = p.nParticle()*p.mass();
        mSum += m;
        i++;
    }

    // calculate total mass across all processors
    reduce(mSum, sumOp<scalar>());

    return mSum;
}

template<class CloudType>
Foam::scalar Foam::functionObjects::cloudInfo<CloudType>::penetration
(
    const CloudType& cloud,
    const vector position0,
    const scalar fraction
) const
{

    if ((fraction < 0) || (fraction > 1))
    {
        FatalErrorInFunction
            << "fraction should be in the range 0 < fraction < 1"
            << exit(FatalError);
    }

    scalar distance = 0.0;

    const label nParcel = cloud.size();
    globalIndex globalParcels(nParcel);
    const label nParcelSum = globalParcels.size();

    if (nParcelSum == 0)
    {
        return distance;
    }

    // lists of parcels mass and distance from initial injection point
    List<List<scalar>> procMass(Pstream::nProcs());
    List<List<scalar>> procDist(Pstream::nProcs());

    List<scalar>& mass = procMass[Pstream::myProcNo()];
    List<scalar>& dist = procDist[Pstream::myProcNo()];

    mass.setSize(nParcel);
    dist.setSize(nParcel);

    label i = 0;
    scalar mSum = 0.0;

    forAllConstIter(typename CloudType, cloud, iter)
    {
        const typename CloudType::particleType& p = iter();
        scalar m = p.nParticle()*p.mass();
        scalar d = mag(p.position() - position0);
        mSum += m;

        mass[i] = m;
        dist[i] = d;

        i++;
    }

    // calculate total mass across all processors
    reduce(mSum, sumOp<scalar>());
    Pstream::gatherList(procMass);
    Pstream::gatherList(procDist);

    if (Pstream::master())
    {
        // flatten the mass lists
        List<scalar> allMass(nParcelSum, 0.0);
        SortableList<scalar> allDist(nParcelSum, 0.0);
        for (label proci = 0; proci < Pstream::nProcs(); proci++)
        {
            SubList<scalar>
            (
                allMass,
                globalParcels.localSize(proci),
                globalParcels.offset(proci)
            ) = procMass[proci];

            // flatten the distance list
            SubList<scalar>
            (
                allDist,
                globalParcels.localSize(proci),
                globalParcels.offset(proci)
            ) = procDist[proci];
        }

        // sort allDist distances into ascending order
        // note: allMass masses are left unsorted
        allDist.sort();

        if (nParcelSum > 1)
        {
            const scalar mLimit = fraction*mSum;
            const labelList& indices = allDist.indices();

            if (mLimit > (mSum - allMass[indices.last()]))
            {
                distance = allDist.last();
            }
            else
            {
                // assuming that 'fraction' is generally closer to 1 than 0,
                // loop through in reverse distance order
                const scalar mThreshold = (1.0 - fraction)*mSum;
                scalar mCurrent = 0.0;
                label i0 = 0;

                forAllReverse(indices, i)
                {
                    label indI = indices[i];

                    mCurrent += allMass[indI];

                    if (mCurrent > mThreshold)
                    {
                        i0 = i;
                        break;
                    }
                }

                if (i0 == indices.size() - 1)
                {
                    distance = allDist.last();
                }
                else
                {
                    // linearly interpolate to determine distance
                    scalar alpha = (mCurrent - mThreshold)/allMass[indices[i0]];
                    distance =
                        allDist[i0] + alpha*(allDist[i0+1] - allDist[i0]);
                }
            }
        }
        else
        {
            distance = allDist.first();
        }
    }

    Pstream::scatter(distance);

    return distance;

}

// ************************************************************************* //
