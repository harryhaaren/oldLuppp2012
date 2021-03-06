
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

#ifndef LUPPP_AUTOMOVE_TYPES
#define LUPPP_AUTOMOVE_TYPES

enum AutoMoveType
{
  AUTOMOVE_TYPE_NONE = 0,
  AUTOMOVE_TYPE_UP,
  AUTOMOVE_TYPE_DOWN,
  AUTOMOVE_TYPE_UP_DOWN,
  AUTOMOVE_TYPE_DOWN_UP,
};

enum AutoMoveDuration
{
  AUTOMOVE_DURATION_2   =  2,
  AUTOMOVE_DURATION_4   =  4,
  AUTOMOVE_DURATION_8   =  8,
  AUTOMOVE_DURATION_16  = 16,
  AUTOMOVE_DURATION_32  = 32,
};

#endif

