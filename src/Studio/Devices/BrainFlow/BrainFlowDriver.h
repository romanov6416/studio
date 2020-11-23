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

#ifndef __NEUROMORE_BRAINFLOW_H
#define __NEUROMORE_BRAINFLOW_H

#include <Config.h>
#include <DeviceDriver.h>
#include <Devices/BrainFlow/BrainFlowDevices.h>
#include <EngineManager.h>
#include <Core/EventHandler.h>
#include <Core/Array.h>

#include <QObject>

#include "brainflow/board_shim.h"

#ifdef INCLUDE_DEVICE_BRAINFLOW

class BrainFlowDriver : public QObject, public DeviceDriver, public Core::EventHandler
{
public:
	BrainFlowDriver() : DeviceDriver(false) {}
	virtual ~BrainFlowDriver() = default;

	BrainFlowDevice* deviceConnect(int boardId, const BrainFlowInputParams& params);
	void deviceDisconnect(BrainFlowDevice& device);
	void DetectDevices() override {
		if (auto* device = GetDeviceManager()->FindDeviceByType(BrainFlowDevice::TYPE_ID, 0)) 
		{
			deviceDisconnect(*dynamic_cast<BrainFlowDevice*>(device));
		}
		else 
		{
			device = deviceConnect(-1, BrainFlowInputParams());
			GetDeviceManager()->AddDeviceAsync(device);
		}
	}


	const char* GetName() const override { return "BrainFlow Devices"; }
	uint32 GetType() const override { return DeviceTypeIDs::DEVICE_TYPEID_BRAINFLOW_BASE; }
	bool HasAutoDetectionSupport() const override { return false; }
	Device* CreateDevice(uint32 deviceTypeID) override { return deviceConnect(deviceTypeID, BrainFlowInputParams()); };
	bool Init() override { return true; };
	void Update(const Core::Time& delta, const Core::Time& elapsed) override {};

private:
	Core::Array<BrainFlowDevice*> mDevices;
};

class BrainFlowDriverBase : public QObject, public DeviceDriver, public Core::EventHandler
{
	Q_OBJECT
	public:
		enum { TYPE_ID = DeviceTypeIDs::DEVICE_TYPEID_BRAINFLOW_BASE};

		// constructor & destructor
		BrainFlowDriverBase() : DeviceDriver(false), EventHandler()			{}
		virtual ~BrainFlowDriverBase();

		const char* GetName() const override								{ return "BrainFlow Devices"; }

		uint32 GetType() const override										{ return TYPE_ID; }

		bool HasAutoDetectionSupport() const override						{ return false; }

		// add device
		void AddDevice(BrainFlowDeviceBase* device);

		// event handler (removes serial threads)
		void OnRemoveDevice(Device* device) override;

		// must be implemented in subclasses
		virtual BrainFlowInputParams GetParams() = 0;

	private:
		// list to keep track of connected devices (index-matched with theh serial thread array below)
		Core::Array<BrainFlowDeviceBase*> mDevices;

};

class BrainFlowDriverCyton : public BrainFlowDriverBase
{
	Q_OBJECT

	public:
		enum { TYPE_ID = DeviceTypeIDs::DEVICE_TYPEID_BRAINFLOW_CYTON };

		// constructor & destructor
		BrainFlowDriverCyton();

		Device* CreateDevice(uint32 deviceTypeID) override;
		bool Init() override;
		void Update(const Core::Time& delta, const Core::Time& elapsed) override;
		BrainFlowInputParams GetParams() override;

	private:
		double		mTimeSinceDeviceCheck;
};

#endif

#endif
