
/*
  This file is part of Luppp.
  Copyright 2012, Harry van Haaren

  Luppp is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Luppp is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Luppp.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef LUPPP_EQUALIZERSTATE
#define LUPPP_EQUALIZERSTATE

enum FrequencyGraphType {
  FREQUENCY_GRAPH_TYPE_LOWPASS,
  FREQUENCY_GRAPH_TYPE_HIGHPASS,
  FREQUENCY_GRAPH_TYPE_PARAMETRIC,
};

struct EqualizerState {
  bool active;
  FrequencyGraphType type;
  float Q[4];
  float gain[4];
  float cutoffFreq[4];
};

#endif
