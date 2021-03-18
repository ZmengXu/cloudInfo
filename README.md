# cloudInfo
```C++
functions
{
    cloudInfo1
    {
        libs            ("liblagrangianCloudInfoFunctionObjects.so");
        type            basicReactingMultiphaseCloudInfo;
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
        position        (0 0 0); // initial position
        clouds          (reactingCloud1); // cloud name, i.e., the folder name in 0/lagrangian
    }
}
```
