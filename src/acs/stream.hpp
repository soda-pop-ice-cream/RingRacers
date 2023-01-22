// DR. ROBOTNIK'S RING RACERS
//-----------------------------------------------------------------------------
// Copyright (C) 2016 by James Haley, David Hill, et al. (Team Eternity)
// Copyright (C) 2022 by Sally "TehRealSalt" Cochenour
// Copyright (C) 2022 by Kart Krew
//
// This program is free software distributed under the
// terms of the GNU General Public License, version 2.
// See the 'LICENSE' file for more details.
//-----------------------------------------------------------------------------
/// \file  stream.hpp
/// \brief Action Code Script: Dummy stream buffer to
///        interact with P_Save/LoadNetGame

// TODO? Maybe untie this file from ACS?

#ifndef __SRB2_ACS_STREAM_HPP__
#define __SRB2_ACS_STREAM_HPP__

extern "C" {
#include "../doomtype.h"
#include "../doomdef.h"
#include "../doomstat.h"
#include "../p_saveg.h"
}

#include <ACSVM/Code.hpp>
#include <ACSVM/CodeData.hpp>
#include <ACSVM/Environment.hpp>
#include <ACSVM/Error.hpp>
#include <ACSVM/Module.hpp>
#include <ACSVM/Scope.hpp>
#include <ACSVM/Script.hpp>
#include <ACSVM/Serial.hpp>
#include <ACSVM/Thread.hpp>
#include <Util/Floats.hpp>

#include <istream>
#include <ostream>
#include <streambuf>

namespace srb2::acs {

class SaveBuffer : public std::streambuf
{
public:
	savebuffer_t *save;
	UINT8 buf[1];

	explicit SaveBuffer(savebuffer_t *save_);

private:
	virtual int_type overflow(int_type ch);
	virtual int_type underflow();
};

}

#endif // __SRB2_ACS_STREAM_HPP__