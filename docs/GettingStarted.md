---
layout: docs
title: Getting Started
permalink: /docs/gettingstarted/
published: true
---


### User interface overview 

The application has a main area and several docking windows. The different docking 
windows can be managed from the **Windows** menu or directly using the local menu bar of the docking window.

![ResInsight User Interface]({{ site.baseurl }}/images/ResInsightUIMediumSize.png)


#### Docking Windows

- **Project Tree** - contains all application objects in a tree structure.
- **Property Editor** - displays all properties for the selected object in the **Project Tree**
- **Process Monitor** - displays output from Octave when executing Octave scripts
- **Result Info** - displays info for the object being hit in the 3D scene when clicking with left mouse button

<div class="note">
<h5>Add additional docking windows</h5>
A new project tree and property editor can be added from <b>Windows->New Project and Property View</b>.
</div>


### Toolbars 

A selected subset of actions are presented as controls in the toolbar. The different sections in the toolbar can be dragged and positioned anywhere as small floating toolbars. Management of the toolbar is done by right-clicking on the toolbar and then manipulating the displayed menu.

#### 3D Views 

In the main area of the application, several 3D views can be open at the same time. One of them will be active and the active view can be either maximized to use the whole main area, or normalized so that you can see all the open 3D views.

Management of several views is available from the **Windows** menu

- **Tile Windows** - distribute all open view windows to fill available view widget space
- **Cascade Windows** - organize all open view windows sligthly offset on top of each other
- **Close All Windows** - close all open view windows

<div class="note">
<h5>View window management</h5>
Standard window management for applying minimized, normal and maximized state is available in the upper right corner.
</div>
![Restore Down]({{ site.baseurl }}/images/RestoreDown.PNG)

#### Editing the views

Most of the settings and features of ResInsight is accessible through the **Project Tree** and the **Property Editor**. Selecting an item in the **Project Tree** activates the corresponding 3D View, and shows the item properties in the **Property Editor** available for editing. 

Toggling a checkbox next to an item in the **Project Tree** will toggle visibility in the 3D view. Toggling a checkbox for a collection of items will affect the visibility for all items in the collection. ![]({{ site.baseurl }}/images/TreeViewToggle.png)

Context menu commands are also available to do special operations on a selected set of items.

How to interact and manipulate the 3D model is described in [Model Navigation]({{ site.baseurl }}/docs/modelnavigation)


### Cases and their types

A *Case* in ResInsight means a Grid model with a particular set of results or property data. There are three different types of Eclipse cases and one type of Geomechanical cases.

#### Eclipse Cases
There are three different Eclipse Case types: 

##### Result case ![]({{ site.baseurl }}/images/Case24x24.png) 
This is a Case based on the results of an Eclipse simulation, read from a grid file together with static and restart data. Multiple Cases can be selected and read from a folder.

##### Input case ![]({{ site.baseurl }}/images/EclipseInput24x24.png) 
This Case type is based on a `*.GRDECL` file, or a part of an Eclipse *Input* file. This Case type supports loading single ASCII files defining Eclipse Cell Properties, and also to export modified property sets to ASCII files.
Each of the Eclipse properties is listed as separate entities in the **Project Tree**, and can be renamed and exported.

#####  Statistics case ![]({{ site.baseurl }}/images/Histogram24x24.png)
This is a Case type that belongs to a *Grid Case Group* and makes statistical calculations based on the source cases in the Grid Case Group. 

#### Geomechanical cases ![]({{ site.baseurl }}/images/GeoMechCase24x24.png)
There are only one type of geomechanical cases, namely the ABAQUS-odb case type. 

The geomechanical cases will is sorted into its own folder in the project tree named **Geomechanical Models** ![]({{ site.baseurl }}/images/GeoMechCases24x24.png) as opposed to the **Grid Models** folder where the Eclipse cases and **Grid Case Groups** resides.

#### Grid Case Groups ![]({{ site.baseurl }}/images/GridCaseGroup24x24.png) 

A **Grid Case Group** is a group of Eclipse **Result Cases** with identical grids, but generally different active cells, initial values and results. These cases are called *Source Cases*. 

The purpose of a Grid Case group is to make it easy to calculate statistics across the source cases both for static and dynamic Eclipse Properties. 

See [ Multiple realizations and statistics ]({{ site.baseurl }}/docs/casegroupsandstatistics) for more on this.


### Project files and Cache directory

ResInsight stores which cases you have imported and all the settings for each view etc. in a project file with the extension `.rsp`.
This file only contains references to the real data files, and even references to data files generated by ResInsight itself.

Statistics calculations, property sets you generate by using Octave, and well paths are saved to a folder in the same directory as you save the project file, and is named ProjectFileName_cache. So if you need to move your project, make sure you move this folder as well.

<div class="note">
The `.rsp`-file is an XML file, and can be edited by any text editor.  
</div>

### Export options

#### Snapshot images 

##### Single View ![]({{ site.baseurl }}/images/SnapShot.png) ![]({{ site.baseurl }}/images/SnapShotSave.png)
Image export of current 3D view can be launched from **File -> Export -> Snapshot To File**. A snapshot can also be copied to clipboard using **Edit->Copy Snapshot To Clipboard**. 

##### All Views ![]({{ site.baseurl }}/images/SnapShotSaveViews.png)
If a project contains multiple views, all views can be exported using **File -> Export -> Snapshot All Views To File**. 

It is also possible to snapshot all views from the command line. See [ Command Line Arguments ]({{ site.baseurl }}/docs/commandlineparameters)