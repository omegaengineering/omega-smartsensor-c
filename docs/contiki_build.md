### Build basic_sensor without CCS


```
cd examples/contiki
make
```

To clean build artifacts:
```
make clean
```

### Creating a build project with CCS

1. In CCS, File > New > CCS Project
    Enter Project name
    Select MCU
    Select Debug Connection
    Select Compiler GNU (Linaro)
    Select Empty Project.
2. In example/contiki directory,
    Set CONTIKI to path of Contiki source code relative to your project

3. Right click Project, goto Properties,
    In Linked resources, create varible OMEGA_SDK with path points to omega-smartsensor-c SDK
    In Build, Builder, uncheck "Generate makefiles automatically"
    In Build Directory, change it to "${OMEGA_SDK}/examples/contiki"
    Click Apply, OK.
4. Right click Project, Build Project.
    Project should build successfully


### Creating debug configuration with CCS

1. Right click Project, Debug, Debug Configurations
2.  Create new configuration with CCS Device Debugging template
    Set basic_sensor for name of the debug configuration
    Ensure, Main, Target configuration is ${target_config_active_default:demo}
    Program, Project, click Workspace, select your project name
    Program, Program, give path to basic_sensor.elf (drill down in build subdirectory)
    Apply, OK.
3. Project should now debug. Breakpoint stopped at main().