# Changelog
All notable changes to PixelMaestro Studio will be documented in this file.

The format is loosely based on [Keep a Changelog](http://keepachangelog.com/).

## [v0.60] - 2020-03-05

### Added
- Added support for connecting network devices. Enter the IP address and port number in the format <IP>:<Port> in place of a serial device URI. Leaving the port number blank defaults to port 8077.
- Added wrap toggle to Section tab.
- Added icons to palette, animation, and orientation comboboxes.

### Changed
- GUI changes:
	- Broke out Section and Layer controls to the top of the screen.
	- Converted Section and Layer combo boxes into list boxes.
	- Sections are now displayed in a grid. You can change the width of the grid in the Preferences.
	- Reorganized the Canvas and Show tabs.
	- Added highlights to play/pause buttons when clicked.
- Fixed crash when closing PixelMaestro Studio when both Maestro views are closed.
- Added Cue size to code output in the Cue Interpreter dialog.

## [v0.51] - 2019-11-26

### Added
- Added window icons.

### Changed
- Changed time widgets to display time spans instead of milliseconds.

## [v0.50.2] - 2019-07-26

### Added
- Added checkbox for toggling "Code" column in Cue Preview dialog.
- Add ability to set an Animation's center.

### Changed
- Fixed cursor positioning in SectionDrawingArea.
- Migrated docs to Sphinx.

## [v0.50.1] - 2019-06-30

### Added
- Added support for generating "comet-style" palettes.

### Changed
- Device tab: readded flag when uploading Cues to EEPROM.

## [v0.50] - 2019-06-19

### Important Changes

#### Cue Changes

Due to changes in how Cues are formatted, Cuefiles created in earlier versions can no longer be opened in this version.

### Added
- Added option to mirror Sections across the x and/or y axes.
- Added notification icon for when a background action (e.g. a Show) modifies the Maestro.
- Added notification icon for when a device is connected.
- Added the ability to drag and drop Events in the Show tab's Event Queue.
- Added a dialog for editing Events. Double-click an Event in the Event Queue to change its settings.
- Added option to allow Show Events to trigger live device updates. This allows you to trigger commands on a connected device without having to run the Show on the device.
- Added a play/pause button and cycle selector for Animations.

### Changed
- Enabling Maestro outputs has been moved from the Preferences dialog to the Window menu item.
- Redesigned Device Tab.
- Fixed `MaestroController::save_section_to_datastream()` not saving the Canvas frame count.
- Fixed Device Section Map not updating the correct Cue.
- Fixed Canvases not drawing frames larger than 255x255 pixels.
- Fixed Delay getting disabled when fading is enabled.
- Fixed Canvas actions performed on a SectionDrawingArea not updating the cursor before performing the action.
- Reverted broken duplicate Palette name check.
- Increased minimum window width.
- Bumped Qt version to 5.12.3.

### Removed
- Wave animation mirror option.

## [v0.40.1] - 2018-12-06
### Changed
- Fixed issue with populating Grid Size when switching Sections/Layers.

## [v0.40] - 2018-12-03
### Added
- Main Window
	- Added `File > Open and Merge...`, which loads a Cuefile on top of the current Maestro instead of replacing it.
	- Added `File > Open to Event History...`, which loads a Cuefile into the Event History without affecting the running Maestro.
	- Added ability to resize sections of the interface.
	- Added shortcut keys for Save, New, Open, etc.
	- Added confirmation prompt when there are unsaved changes.
- Animation Editor
	- Added cursor when using the Canvas brush tool.
	- Added duplicate name check to Palette Editor.
	- Added lock icon to tabs when `Lock Maestro` is checked in the Show tab.
	- Added check to prevent segfaults when editing certain animations while `Lock Maestro` is enabled in the Show tab.
	- Added buttons to clear the Event History and Event Queue in the Show tab.
	- Added ability to click on a Section to make it the Active Section.
	- Added "replace" tool for Canvases, which replaces a single color with another color.
	- Added ability to re-select a Palette from the Palette drop-down. This lets you reapply a Palette after making changes.
	- Added refresh button to Device tab, which scans for connected devices.
	- Added ability to map Sections between the PC and a device to "translate" live updates to different Sections.
	- Added Section brightness slider (brightness moved from Maestro-level to Section-level).
- Preferences
	- Added ability to customize the max number of events stored in the Event History.

### Changed
- Main Window
	- Added tooltips to several menu options.
	- Renamed several file menu options to be more clear about their actions.
	- Removed unnecessary Cues being saved to new Cuefiles.
	- Viewport now reflects changes to Maestro brightness.
- Animation Editor
	- Moved Maestro play/pause and lock controls from Show tab to the main window.
	- Split MaestroControlWidget into subwidgets.
	- Show timer now runs continuously instead of only when the Show is running.
	- Renamed `Cue Interpreter` dialog to `Cue Preview`.
	- Fixed memory leak when loading images into a Canvas.
	- Fixed issue where Cues executed twice while loading.
	- Fixed custom Palettes saving and loading even with session saving disabled.
	- Fixed Palette colors regenerating when changing a Palette's name.
	- `Enable Show` now initializes and deletes the Show instead of just pausing it.
	- Increased Event History to 200 events.
	- Fixed issue where cancelling the Palette Editor dialog regenerated the color scheme anyway.
	- Saved devices will now try to auto-connect even if they weren't auto-detected.
- Other
	- Updated Qt version to 5.11.2.
	- Created static build for Linux. Linux users do not need to preinstall Qt.

### Removed
- Removed donation link.
- Removed Pixel Size option from Preferences dialog.

## [v0.30] - 2018-04-18
WARNING: This update breaks backwards compatability with v0.20.1 and earlier Cuefiles.

### Added
- Animation Editor
	- Added Device tab for controlling USB devices. This replaces the Serial section of the Preferences dialog.
	- Added ability to upload an entire Maestro configuration (a `Cuefile`) to a USB device.
	- Added ability to toggle live updating of USB devices.
	- Added ability to unset Animations.
	- Added ability to edit Palettes.
	- Palettes are now saved between sessions.
	- Added dialog window for previewing Cuefiles.
- Preferences
	- Added setting to toggle session auto save/load when closing/opening the application. This is enabled by default.

### Changed
- Animation Editor
	- Fixed MaestroController not properly saving Section scroll settings.
	- Palette list is now automatically saved and reopened when closing/opening PixelMaestro Studio.
	- Consolidated all three Canvas types into the base Canvas class. This new class works like a PaletteCanvas with support for transparency.
	- Replaced the "Canvas Type" drop-down with a check box.
- Preferences
	- Moved USB device settings to Animation Editor.
- File selector dialog now remembers the last directory used.
- Lowered minimum Qt version to 5.9.1 for compatibility with Ubuntu 18.04 repository Qt version.
- Code cleanup.

## [v0.20.1] - 2018-03-20

### Changed
- Fixed an issue where an extra non-existent Section was listed in the Animation Editor dropdown.
- Fixed active Section identification and highlighting.
- Fixed mirror checkbox not being checked when switching to a Section with a mirrored Wave animation.

## [v0.20] - 2018-03-17
WARNING: This update breaks backwards compatability with v0.12 and earlier Cuefiles.

### Added
- Added ability to set the Canvas cursor by clicking on a pixel.
- Added Canvas drawing tool that lets you draw directly onto a Section.
- Added playback buttons to the Canvas and Show tabs.
- Added fire animation.
- Added `skew` and `mirror` parameters to Wave animation.
- Added ability to reorder Show Events.
- Added a border to Sections. The active Section/Layer has a lighter border, while all other Sections have a dark border.
	
### Changed
- _Many_ UI changes and bugfixes.
- Fixed Maestro-level properties not updating when loading a Maestro from a Cuefile.
- Enabled changing the grid size while a Layer is active.
- Enabled changing per-axis offset values while scrolling is disabled on that axis.
- Merged Lightning animation's two separate drift controls into a single control.
- Changed Preferences dialog layout.
	
### Removed
- Removed the Merge animation. Use `WaveAnimation::set_mirror(true)` to recreate the merge effect.
- Removed standalone demos. To see a demo, open one of the example Cuefiles from the `examples` folder.

## [v0.12] - 2018-01-22
### Changed
- Animation Editor:
	- Loading an image into a PaletteCanvas now adds the palette to the palette drop-down.
- MaestroController: Changed default refresh rate to 50ms.
- SimpleDrawingAreaDialog: Added fullscreen toggle.
- Renamed `Settings` to `Preferences`.

## [v0.11] - 2017-12-23
### Added
- Moved Maestro timing to the MaestroController. This should reduce resource usage and keep screen outputs from falling out of sync with each other.
- Animation Editor
	- Added support for `PaletteCanvases`.
	- Added a pause/resume button to the Show tab. This lets you stop and start the Maestro at any time.
	- Added a relative time box to the Show tab. This shows the time elapsed since the last event that was ran.
- Demos
	- Added sample Cue files for each demo. You can load these files in the Animation Editor.
- Settings
	- Added the ability to pause the Maestro when the Animation Editor first opens.

### Changed
- Moved Cuefile processing and Maestro initialization into MaestroController.
- Animation Editor
	- Show tab: Renamed "Current Time" to "Absolute Time".

### Changed
- Animation Editor: Removed alert when a saved USB device is no longer connected. The device now appears greyed out on the Settings page.

## [v0.10] - 2017-12-05
### Added
- Added donate link to Help menu.
- Added ability to configure multiple output devices in settings.
- Animation Editor
	- Added checkbox to enable repeated Canvas scrolling.
	- Added checkbox for looping Show Events.
	- Added empty space between Sections.

### Changed
- Moved project into Beta state.
- Modified Show Event management UI
	- Added ability to remove Events from Show Event list.
	- Added timing method combo box.
	- Simplified Event History selection.
	- Events that have already run will now appear grayed out.

### Removed
- Removed device simulator.
- Removed Animated Canvas demo.

## [v0.9] - 2017-11-28
### Added
- Added frame controls to Canvas editing tools.
- Added Canvas Edit mode, which stops the Canvas from animating so you can draw on individual frames.
- Added Show Edit mode, which lets you generate Events without changing the Maestro itself.

### Changed
- Redesigned Animation Editor UI. Controls are now grouped into tabs for each component: Section, Animation, Canvas, and Show.
- Show controls are now fully functional.
