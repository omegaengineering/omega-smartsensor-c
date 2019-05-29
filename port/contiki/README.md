Creating a build project with CCS

1. In CCS, File > New > CCS Project
    Enter Project name
    Select MCU
    Select Debug Connection
    Select Compiler GNU (Linaro)
    Select Empty Project.
2. Copy Makefile and Makefile.configurations in this sample to root direction of the newly created project.
    In Makefile, 
        Set CONTIKI_PROJECT to your project name (eg: demo)
        Set CONTIKI to path of Contiki source code relative to your project
        Create demo.c in your project
        
3. Right click Project, goto Properties,
    In Build, Builder, uncheck "Generate makefiles automatically"
    In Build Directory, change it to "${BuildDirectory}/../"
    Click Apply, OK.
4. Right click Project, Build Project.
    Project should build successfully


Creating debug configuration with CCS

1. Right click Project, Debug, Debug Configurations
    Create new configuration with CCS Device Debugging template
    Set Name for debug configuration
    Ensure, Main, Target configration is ${target_config_active_default:demo}
    Program, Project, click Workspace, select your project name
    Program, Program, give path to demo.elf (in build subdirectory)
    Apply, OK. Project should now debug.