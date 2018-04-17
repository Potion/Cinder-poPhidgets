/*
Adapted from example available here: https://www.phidgets.com/?view=code_samples&lang=C
*/
#pragma once

#define __stdcall
#define __cdecl
#define __attribute__ ((dllimport))
#define __declspec(dllimport)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"

#include <phidget22.h>
#include <memory>

#include "poPhidgets/PhidgetBaseInput.h"

#pragma clang diagnostic pop

namespace po
{
	namespace phidget
	{
		class VoltageRatioInput;
		typedef std::shared_ptr<VoltageRatioInput> VoltageRatioInputRef;

		class VoltageRatioInputDelegate;
		typedef std::weak_ptr<VoltageRatioInputDelegate> VoltageRatioInputDelegateRef;


		class VoltageRatioInputDelegate
		{
			public:
				virtual void voltageRatioValueChanged( double ratio ) = 0;
		};

		class VoltageRatioInput
			: public po::phidget::BaseInput
		{
			public:
				static VoltageRatioInputRef create();
				void setDelegate( VoltageRatioInputDelegateRef delegate ) { mDelegate = delegate; };
				double getVoltageRatio();
                double getSensorValue();

				PhidgetHandle getHandle() override { return ( PhidgetHandle ) mHandle; };
				~VoltageRatioInput();

			protected:
				VoltageRatioInput();
                void setDataIntervals( uint32_t interval ) override;

			private:
				PhidgetVoltageRatioInputHandle mHandle = NULL;
				int createSpecificInput() override;

				//	called during setup
				static int createVoltageRatioInput( PhidgetVoltageRatioInputHandle* pvrih );

				//	properties specific to this type of input
				uint32_t mDataInterval;
				double mChangeTrigger;

				int setAttachDetachErrorHandlers( PhidgetHandle ch ) override;
				void setChangeHandlers( PhidgetHandle ch ) override;
				int setVoltageRatioHandler( PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr );

				//	event handlers
				static void CCONV onAttachHandler( PhidgetHandle ph, void* ctx );
				static void CCONV onDetachHandler( PhidgetHandle ph, void* ctx );
				static void CCONV onVoltageRatioChangeHandler( PhidgetVoltageRatioInputHandle pvrih, void* ctx, double ratio );

				//	error display
				//static void displayError( PhidgetReturnCode code );

				//  communication
				VoltageRatioInputDelegateRef mDelegate;
				void testCallbackFunction( double test );
		};
	}
}
