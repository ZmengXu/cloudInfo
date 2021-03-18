# cloudInfo
```C++
functions
{
    cloudInfo1
    {
        type            cloudInfo;
        libs            ("liblagrangianFunctionObjects.so");

        enabled         true;
        writeControl    timeStep;
        writeInterval   1;

        penetration     0.95;    // fraction of the total mass, in [0, 1]
        position        (0 0 0); // initial position
        clouds          (reactingCloud1); // cloud name
    }
}
```
