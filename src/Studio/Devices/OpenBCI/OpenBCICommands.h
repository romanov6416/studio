/****************************************************************************
**
** Copyright 2019 neuromore co
** Contact: https://neuromore.com/contact
**
** Commercial License Usage
** Licensees holding valid commercial neuromore licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and neuromore. For licensing terms
** and conditions see https://neuromore.com/licensing. For further
** information use the contact form at https://neuromore.com/contact.
**
** neuromore Public License Usage
** Alternatively, this file may be used under the terms of the neuromore
** Public License version 1 as published by neuromore co with exceptions as 
** appearing in the file neuromore-class-exception.md included in the 
** packaging of this file. Please review the following information to 
** ensure the neuromore Public License requirements will be met: 
** https://neuromore.com/npl
**
****************************************************************************/

#ifndef __NEUROMORE_OPENBCICOMMANDS_H
#define __NEUROMORE_OPENBCICOMMANDS_H

#include <Core/String.h>


// stream packet data (one sensor)
struct OpenBCIStreamEEGData
{
	char mEEGChannel[3];

	// Convert 24 bit integer to 32 bit integer
	int32 GetValue() const
	{
		int32 newInt = (
			((0xFF & mEEGChannel[0]) << 16) |
			((0xFF & mEEGChannel[1]) << 8) |
			(0xFF & mEEGChannel[2])
		);

		if ((newInt & 0x00800000) > 0)
		    newInt |= 0xFF000000;
		else
		    newInt &= 0x00FFFFFF;

	    return newInt;
	}
};

// accelerometer stream packet data
struct OpenBCIStreamAccData
{
	char mAccelerationChannel[2];

	// Convert 16 bit integer to 32 bit integer
	int32 GetValue() const
	{
	    int newInt = (
            ((0xFF & mAccelerationChannel[0]) << 8) |
            (0xFF & mAccelerationChannel[1])
        );

        if ((newInt & 0x00008000) > 0)
            newInt |= 0xFFFF0000;
		else
            newInt &= 0x0000FFFF;

        return newInt;
	}
};

// the complete stream packet
struct OpenBCIStreamPacket
{
	static char GetProtocolHeader()		{ return (unsigned char)0xA0; }
	static char GetProtocolFooter()		{ return (unsigned char)0xC0; }

	// The packet data:

	char mHeader;			// header byte
	char mSampleNumber;		// increasing sample number

	// 8X EEG Data
	OpenBCIStreamEEGData mSensors[8]; // int24 data

	// Accelerometer Data
	OpenBCIStreamAccData mAcceleration[3]; // int16 data

	char mFooter;			// Footer byte

	bool Verify() { return (mHeader == GetProtocolHeader() && mFooter == GetProtocolFooter()); }

	char GetSampleNumber() const { return mSampleNumber; }
};


#endif
