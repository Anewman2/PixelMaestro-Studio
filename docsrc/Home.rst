
Home
====

Welcome to the PixelMaestro Studio documentation!

What is PixelMaestro Studio?
----------------------------

PixelMaestro Studio is a desktop application for managing LEDs, based on the `PixelMaestro library <https://github.com/8bitbuddhist/PixelMaestro/>`_. With PixelMaestro Studio, you can create custom animations, schedule dynamic light shows, control devices over USB, and more.

To start using PixelMaestro Studio, see the :doc:`Getting Started <Getting-Started>` guide. If you want to learn more about the PixelMaestro library, check out the :pmdocs:`PixelMaestro documentation <index.html>`.

FAQ
---

**How do I connect my Arduino, Raspberry Pi, Teensy, etc?**

Connect your device to your PC using USB, then follow the instructions under :doc:`Device Tab <Device-Tab>`.

**When I click Connect on the Device Tab, nothing happens/I get an error message!**

Make sure you have read and write access to the serial port that the device is connected on. Also, make sure the port isn't being used by another process.

**My device is connected, but when I upload a Cuefile or enable Live Updates, nothing happens!**

In your device's sketch, make sure you :pmdocs:`enabled the CueController <Cues.html#initializing-the-cuecontroller>` and appropriate :pmdocs:`CueHandlers <Cues.html#enabling-cuehandlers>`. You can specify the size of the CueController's buffer during its initialization; any Cues larger than the buffer will be ignored. Read the input from ``serial::read()`` to :pmdocs:`CueController::read() <Cues.html#running-cues>` *before* running ``Maestro::update()``. You can refer to the :pmarduino:`Arduino sketches <>` included with PixelMaestro.

Also, make sure that the CueController on your device isn't blocking Cues. You can tell when Cues are being blocked if your sketch contains statements similar to:

.. code-block:: c++

   cue_controller.set_blocked_cues(...)

You might also be hitting the memory limits of your device. Try disabling or removing features from your sketch, especially Canvases and Shows as they can use large amounts of RAM.

**The app runs really slow at high resolutions!**

All rendering is done on the CPU, so the amount of processing power required increases as you increase your resolution. You can try:


* Reducing the size of your grids
* Not rendering the Maestro locally by unchecking the options in the *Window* menu
* :pmdocs:`Tweaking Pixel performance options <Pixels.html#performance-options>`

**You should add X animation or Y feature!**

Submit an issue, or better yet, a pull request!