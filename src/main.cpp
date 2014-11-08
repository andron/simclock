// -*- mode:c++; indent-tabs-mode:nil; -*-

/*
  Copyright (c) 2013 Anders Ro, anders.ronnbrant@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "simclock.h"

#include <iomanip>
#include <iostream>

int
main(int ac, char** av) {
  int frames = 0;
  if (ac > 1)
    frames = std::atoi(av[1]);
  if (frames == 0)
    frames = 64;

  std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  std::cout.precision(4);

  SimulationClock clock;

  for (int i = 0; i < frames; ++i) {
    clock.next();

    auto simtime = clock.now();

    // Convert to other formats using the fetched time.
    auto us      = SimulationClock::as<std::chrono::microseconds>(simtime);
    auto ns      = SimulationClock::as<std::chrono::nanoseconds>(simtime);
    double dsec  = SimulationClock::as<double>(simtime);

    // Or just get the time as anything.
    double dsec2 = SimulationClock::as<double>(clock);

    std::cout << std::setw(10) << simtime << " : "
              << std::setw(10) << us.count() << " : "
              << std::setw(10) << ns.count() << " : "
              << dsec << " : "
              << dsec2 << std::endl;
  }

  std::cout << "step(double) method" << std::endl;
  for (int i = 0; i < 10; ++i) {
    clock.step(0.107);
    auto us = SimulationClock::as<std::chrono::microseconds>(clock.now());
    std::cout << std::setw(10) << clock.now() << " : "
              << std::setw(10) << us.count() << std::endl;
  }

  return 0;
}
