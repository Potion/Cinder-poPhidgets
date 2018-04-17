#pragma once
#include "poPhidgets/PhidgetBaseInput.h"

namespace po
{
	namespace phidget
	{
		class DistanceSensor;
		typedef std::shared_ptr<DistanceSensor> DistanceSensorRef;

		class DistanceSensor
			: public po::phidget::BaseInput
		{
			public:
				static DistanceSensorRef create();

				virtual PhidgetHandle getHandle() override { return PhidgetHandle( mHandle ); };
				~DistanceSensor();

			protected:
                int createSpecificInput() override;
                static int createDistanceSensor(PhidgetDistanceSensorHandle* dsh);
                void setDataInterval( uint32_t interval ) override;
                void setChangeTriggerUInt32( uint32_t trigger ) override;

			private:
				PhidgetDistanceSensorHandle mHandle = NULL;

				void setChangeHandlers() override;

				//	event handlers
				static void CCONV onDistanceChangeHandler( PhidgetDistanceSensorHandle ch, void* ctx, int distance );
				static void CCONV onSonarReflectionsUpdateHandler( PhidgetDistanceSensorHandle ch, void* ctx, const uint32_t distances[8], const uint32_t amplitudes[8], uint32_t count );

		};
	}
}
