# cloudInfo
[![OpenFOAM version](https://img.shields.io/badge/OpenFOAM-7-brightgreen)](https://github.com/OpenFOAM/OpenFOAM-7)
This is a functionObject for OpenFOAM.

used to get cloud information, e.g., number of parcels, total mass, penetration, spray angle.
It can be used in run time or as postprocessing, see setting below

# Run time setting
In system/controlDict, add the following cloudInfo1 function, revise the type, clouds, penetration, position, and direction according to the constant/XXXCloudProperties.
```C++
functions
{
    cloudInfo1
    {
        libs            ("liblagrangianCloudInfoFunctionObjects.so");
        type            basicSprayCloudInfo;//basicReactingMultiphaseCloudInfo;
        //basicReactingMultiphaseCloudInfo;//for reactingParcelFoam and coalChemistryFoam
        //basicSprayCloudInfo;//for sprayFoam
        //basicKinematicMPPICCloudInfo;//
        //basicThermoCloudInfo;// for I dont know which Foam, you can see the name in createClouds.H
        //basicKinematicCloudInfo;//
        //basicKinematicCollidingCloudInfo;//
        //basicReactingCloudInfo;//
        clouds          sprayCloud;//(reactingCloud1); // cloud name, i.e., the folder name in 0/lagrangian

        enabled         true;
        writeControl    timeStep;
        writeInterval   1;

        penetration     0.95;    // fraction of the total mass, in [0, 1]
        position        (0 0 0); // initial position
        direction       (1 0 0); // initial spray direction
    }
}
```


# Postprocessing
Create a file, e.g., cloudInfo, in system/ folder, and then run `sprayFoam -postProcess -func cloudInfo`.
```
/*--------------------------------*- C++ -*----------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Version:  7
     \\/     M anipulation  |
\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    location    "system";
    object      cloudInfo;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

libs            ("liblagrangianCloudInfoFunctionObjects.so");
type            basicSprayCloudInfo;//basicReactingMultiphaseCloudInfo;
//basicReactingMultiphaseCloudInfo;//for reactingParcelFoam and coalChemistryFoam
//basicSprayCloudInfo;//for sprayFoam
//basicKinematicMPPICCloudInfo;//
//basicThermoCloudInfo;// for I dont know which Foam, you can see the name in createClouds.H
//basicKinematicCloudInfo;//
//basicKinematicCollidingCloudInfo;//
//basicReactingCloudInfo;//

enabled         true;
writeControl    timeStep;
writeInterval   1;

penetration     0.95;    // fraction of the total mass, in [0, 1]
position        (0 0.0995 0); // initial position
direction       (0 -1 0); // initial spray direction
clouds          (sprayCloud);//(reactingCloud1); // cloud name, i.e., the folder name in 0/lagrangian

// ************************************************************************* //
```