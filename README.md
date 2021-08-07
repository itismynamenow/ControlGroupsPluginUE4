# ControlGroupsPluginUE4

C++ plugin for Unreal Engine 4.25 that allows to create control groups

## What is included

* C++ code for 
  * UControlGroupsManagerComponent that should be attached to AGameMode to keep track of all control groups
  
## Install

* Install [SelectionPluginUE4](https://github.com/itismynamenow/SelectionPluginUE4)
* Clone this repository into Plugins folder in your UE4 project
* Right click your *.uproject file to generate Visual Studio project files
* Build solution

## Use

* Setup everything needed by [SelectionPluginUE4](https://github.com/itismynamenow/SelectionPluginUE4) to be able to select units
* Add UControlGroupsManagerComponent to AGameMode instance
* To form control group select units and press `UControlGroupsManagerComponent::m_ModifierKey`(Left Ctrl) and number key
* To select control group press number key

## Don't forget 

* This "plugin" depends on [SelectionPluginUE4](https://github.com/itismynamenow/SelectionPluginUE4)
* Check logs if something doesn't work
