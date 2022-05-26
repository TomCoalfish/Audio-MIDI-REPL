/*
  Bit Grinder - derived from m_delay.cpp

  Copyright (C) 2011 Bill Yerazunis <yerazunis@yahoo.com>

  This file is part of ams.

  ams is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.

  ams is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ams.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef M_BITGRIND_H
#define M_BITGRIND_H

#include "module.h"


#define MODULE_BITGRIND_HEIGHT                95

class M_bitgrind : public Module
{
    Q_OBJECT 

      float sampleRate, sampleRateMod;
    float bits, bitsMod;
    float samplecounter;
    
    Port *port_M_in, *port_M_sampleRate, *port_M_bits; 
    Port *port_out;
    
  public: 
    float **inData, **inSampleRate, **inBits;       
    float *sval;
    
  public:
    M_bitgrind(QWidget* parent=0, int id = 0);
    ~M_bitgrind();

    void generateCycle();
};
  
#endif
