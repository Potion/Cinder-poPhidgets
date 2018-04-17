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
        
        void DistanceSensor::setDataInterval(uint32_t interval)
        {
            uint32_t minDataInterval;
            uint32_t maxDataInterval;
            PhidgetReturnCode prc;
            
            prc = PhidgetDistanceSensor_getMinDataInterval(mHandle, &minDataInterval);
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting min data interval" );
                displayError( prc );
                return;
            }
            
            prc = PhidgetDistanceSensor_getMaxDataInterval( mHandle, &maxDataInterval );
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting max data interval" );
                displayError( prc );
                return;
            }
            
            CI_LOG_D( "Max and min data intervals (in milliseconds)\t Max: " << maxDataInterval << ", min: " << minDataInterval );
            
            if( mDataInterval > maxDataInterval || mDataInterval < minDataInterval ) {
                CI_LOG_W( "Setting data interval to value outside max and min limits." );
            }
            
            //  Set the actual data interval
            prc = PhidgetDistanceSensor_setDataInterval( mHandle, mDataInterval );
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Set DataInterval" );
                displayError( prc );
                return;
            }
        }
        
        void DistanceSensor::setChangeTriggerUInt32(uint32_t trigger)
        {
            //  find min and max change trigger
            uint32_t minChangeTrigger;
            uint32_t maxChangeTrigger;
            PhidgetReturnCode prc;
            
            
            prc = PhidgetDistanceSensor_getMinDistanceChangeTrigger( mHandle, &minChangeTrigger );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting min change trigger" );
                displayError( prc );
                return;
            }
            
            prc = PhidgetDistanceSensor_getMaxDistanceChangeTrigger( mHandle, &maxChangeTrigger );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting max change trigger" );
                displayError( prc );
                return;
            }
            
            CI_LOG_D( "Max and min distance change triggers\t Max: " << maxChangeTrigger << ", min: " << minChangeTrigger );
            CI_LOG_V( "Setting change trigger to " << mChangeTriggerUInt32 );
            
            if( mChangeTriggerUInt32 > maxChangeTrigger || mChangeTriggerUInt32 < minChangeTrigger ) {
                CI_LOG_W( "Setting change trigger to value outside the max and min limits." );
            }
            
            PhidgetDistanceSensor_setDistanceChangeTrigger(mHandle, mChangeTriggerUInt32);
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Set DistanceChangeTrigger: \n\t" );
                displayError( prc );
                return;
            }
        }
        
		void DistanceSensor::setChangeHandlers()
		{
			CI_LOG_V( "setChangeHandlers" );
		}

		void CCONV DistanceSensor::onDistanceChangeHandler( PhidgetDistanceSensorHandle ch, void* ctx, int distance )
		{
            CI_LOG_V( "onDistanceChangeHandler: distance: " << distance );
		}

		void CCONV DistanceSensor::onSonarReflectionsUpdateHandler( PhidgetDistanceSensorHandle ch, void* ctx, const uint32_t distances[8], const uint32_t amplitudes[8], uint32_t count )
		{
			CI_LOG_V( "onSonarReflectionsUpdateHandler" );
            for (int i = 0; i < count; i++) {
                CI_LOG_V("\t"<< i << "\tDistance: " << distances[i] << "\tAmplitude: " << amplitudes[i]);
            }
 		}

	}
}
