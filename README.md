# Parametric Search

This repository contains a framework to implement Megiddo's Parametric
Search technique. This ancient code from 2003 has been brought back to
life in 2018.

The tutorial directory contains two example codes that use the framework:

* median_of_lines: this is a simple example of the application of
  parametric search, similar to the example in Megiddo's paper. It is
  a toy example, intended only for explaining the technique; the
  problem can actually be solved more efficienly with a simple
  alternative approach.
* frechet_distance: this is an implementation of the Frechet Distance
  algorithm by Alt and Godau.

The framework and example codes are documented in the PostScript and
PDF files in the DOC directory.

This code was originally intended to be a so-called "CGAL Extension
Package", where CGAL is the Computational Geometry Algorithms Library;
see http://www.cgal.org. The Frechet Distance code depends on CGAL,
but uses only very few CGAL classes. I'm planning to implement a
stand-alone version of the Frechet Distance code.

## Quick Start for the Frechet Distance code
* Install CGAL on your system (see http://www.cgal.org)
* `cd Tutorial/frechet_distance`
* `make`
