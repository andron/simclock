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

#include "simclock.hh"

#include <iostream>
#include <iomanip>


SimulationClock::SimulationClock()
    : current_time(0)
{
}

SimulationClock::SimulationClock(SimulationClock const& self)
{
  (*this) = self;
}

SimulationClock&
SimulationClock::operator=(SimulationClock const& self)
{
  (*this).current_time.store(self.current_time.load());
  return (*this);
}

SimulationClock::time_point
SimulationClock::now() const noexcept
{
  return time_point(duration(current_time));
}

void
SimulationClock::next() noexcept
{
  step(1);
}

void
SimulationClock::step(double ts) noexcept
{
  // Make a duration using double as storage out of ts. Then use
  // duration cast to cast to a "normal" duration and step that amount.
  dseconds dduration(ts);
  step(std::chrono::duration_cast<SimulationClock::duration>(dduration).count());
}

void
SimulationClock::step(int32_t ts) noexcept
{
  current_time.fetch_add(ts);
}

void
SimulationClock::step(int64_t ts) noexcept
{
  current_time.fetch_add(ts);
}

SimulationClock::duration
SimulationClock::seconds(int32_t t) const noexcept
{
  return std::chrono::duration_cast<SimulationClock::duration>
      (std::chrono::seconds(t));
}

SimulationClock::duration
SimulationClock::milliseconds(int32_t t) const noexcept
{
  return std::chrono::duration_cast<SimulationClock::duration>
      (std::chrono::milliseconds(t));
}

SimulationClock::duration
SimulationClock::microseconds(int32_t t) const noexcept
{
  return std::chrono::duration_cast<SimulationClock::duration>
      (std::chrono::microseconds(t));
}

std::ostream&
operator<<(std::ostream& os, SimulationClock::time_point t)
{
  int64_t frame = t.time_since_epoch().count();
  char fill = std::cout.fill();
  return os << "Sim(" << std::setfill('0') << std::setw(6)
            << frame << ")" << std::setfill(fill);
}

SimulationClock::time_point
operator+(SimulationClock::time_point tp, double sec)
{
  if (sec <= 0.0) {
    return SimulationClock::time_point();
  } else {
    SimulationClock::dseconds dsec(sec);
    return tp + std::chrono::duration_cast<SimulationClock::duration>(dsec);
  }
}
