TAPSMaintain
============

TAPS maintenance software

Installation
------------

### Dependencies
* ROOT 5.34
* TAPSsc

### Installation
* Compile and install the software using `make clean ; make; make install`

Changelog
---------

### 0.4.0
March 2017
* cosmics calibration: improved fitting, extended manual correction
* LED calibration: use histogram derivative, improved manual correction,
  rebin support

### 0.3.0
December 4, 2013
* removed support for old slowcontrol system via libtcpip
* added support for new AcquDAQ-based acquisition and TAPSsc
  slowcontrol

### 0.2.2
November 11, 2011
* support for veto QAC pedestal database setting

### 0.2.1
February 14, 2011
* improved fitting in TDC gain calibration module

### 0.2.0
February 4, 2011
* added TDC gain calibration module

### 0.1.7
December 16, 2010
* code refactoring

### 0.1.6
November 24, 2010
* fixed axis range issue in the check spectra module

### 0.1.5
November 5, 2010
* support for QAC pedestal database setting
* support for hexadecimal value input

### 0.1.4
August 5, 2010
* show block/ring numbers during cosmics calibration
* skip BaF2 modules of ring 1 and 2 in gain match procedure

### 0.1.3
February 24, 2010
* Support for the new taps00 server
* Various small improvements and bugfixes

