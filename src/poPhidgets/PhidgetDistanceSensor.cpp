#include "PhidgetDistanceSensor.h"
namespace po
{
	namespace phidget
	{
		DistanceSensorRef DistanceSensor::create()
		{
			return DistanceSensorRef( new DistanceSensor() );
		}


		//
		//	close channel on destroy
		//
		DistanceSensor::~DistanceSensor()
		{
			if( mHandle != NULL ) {
				CI_LOG_V( "Closing distance sensor input handle" );
				closePhidgetChannel( ( PhidgetHandle )mHandle );
				PhidgetDistanceSensor_delete( &mHandle ); // this sets handle value to NULL

			}
			else {
				CI_LOG_V( "Handle was null; methods in destructor skipped" );
			}
		}

		//
		//	Called by base function;
		//	creates distance sensor input
		//
		int DistanceSensor::createSpecificInput()
		{
			PhidgetReturnCode prc;
			CI_LOG_V( "Creating DistanceSensor channel" );
			prc = PhidgetDistanceSensor_create( &mHandle );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Creating DistanceSensorInput:" );
				displayError( prc );
				return 1;
			}

			return 0;
		}



		void DistanceSensor::setChangeHandlers( PhidgetHandle ch )
		{
			CI_LOG_V( "setChangeHandlers" );
		}

		void CCONV DistanceSensor::onDistanceChangeHandler( PhidgetDistanceSensorHandle ch, void* ctx, int distance )
		{
			CI_LOG_V( "onDistanceChangeHandler" );
		}

		void CCONV DistanceSensor::onSonarReflectionsUpdateHandler( PhidgetDistanceSensorHandle ch, void* ctx, const uint32_t distances[8], const uint32_t amplitudes[8], uint32_t count )
		{
			CI_LOG_V( "onSonarReflectionsUpdateHandler" );
		}

	}
}
