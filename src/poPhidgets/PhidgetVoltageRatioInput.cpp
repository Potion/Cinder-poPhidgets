#include "PhidgetVoltageRatioInput.h"
#include "cinder/Log.h"
#include "cinder/app/App.h"

namespace po
{
	namespace phidget
	{
		//
		//	Create without parameters uses default values in setup
		//
		VoltageRatioInputRef VoltageRatioInput::create()
		{
			return VoltageRatioInputRef( new VoltageRatioInput() );
		}

		VoltageRatioInput::VoltageRatioInput()
		{}


		//
		//	close channel on destroy
		//
		VoltageRatioInput::~VoltageRatioInput()
		{
			if( mHandle != NULL ) {
				CI_LOG_V( "Closing voltage ratio input handle" );
				closePhidgetChannel( ( PhidgetHandle )mHandle );
				PhidgetVoltageRatioInput_delete( &mHandle ); // this sets handle value to NULL

			}
			else {
				CI_LOG_V( "Handle was null; methods in destructor skipped" );
			}
		}

        
        //
        //    Public method;
        //    Returns last reported voltage ratio value from channel
        //
        double VoltageRatioInput::getVoltageRatio()
        {
            if( mHandle == NULL ) {
                CI_LOG_W( "VoltageRatioInput handle does not exist" );
                return 0.0;
            }
            
            PhidgetReturnCode prc;
            double voltageRatio;
            prc = PhidgetVoltageRatioInput_getVoltageRatio( mHandle, &voltageRatio );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Creating VoltageRatioInput:" );
                displayError( prc );
                return 1;
            }
            
            return voltageRatio;
        }
        
        //
        //  Public method;
        //  Returns last reported sensor value appropriate for the sensor
        //
        double VoltageRatioInput::getSensorValue() {
            CI_LOG_V("placeholder");
            if( mHandle == NULL ) {
                CI_LOG_W( "VoltageRatioInput handle does not exist" );
                return 0.0;
            }
            
            PhidgetReturnCode prc;
            double sensorValue;
            prc = PhidgetVoltageRatioInput_getSensorValue( mHandle, &sensorValue );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting sensor value:" );
                displayError( prc );
                return 1;
            }
            
            return sensorValue;
        }
        
        
        /**
         * Creates a new instance of a VoltageRatioInput channel.
         *
         * @param pvrih Pointer to the PhidgetVoltageRatioInputHandle channel to create
         * @return 0 if the operation succeeds, 1 if it fails
         */
		int VoltageRatioInput::createSpecificInput()
		{
            PhidgetReturnCode prc;
            prc = PhidgetVoltageRatioInput_create( &mHandle );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Creating VoltageRatioInput:" );
                displayError( prc );
                return 1;
            }
            
            return 0;
		}

        /*
         *    Set the DataInterval inside of the attach handler to initialize the device with this value.
         *    DataInterval defines the minimum time between VoltageRatioChange events.
         *    DataInterval can be set to any value from MinDataInterval to MaxDataInterval.
         */
        void VoltageRatioInput::setDataInterval(uint32_t interval) {
            uint32_t minDataInterval;
            uint32_t maxDataInterval;
            PhidgetReturnCode prc;
            
            prc = PhidgetVoltageRatioInput_getMinDataInterval( mHandle, &minDataInterval );
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting min data interval" );
                displayError( prc );
                return;
            }
            
            prc = PhidgetVoltageRatioInput_getMaxDataInterval( mHandle, &maxDataInterval );
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
            prc = PhidgetVoltageRatioInput_setDataInterval( mHandle, mDataInterval );
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Set DataInterval" );
                displayError( prc );
                return;
            }
        }
        
        /*
         *    Set the VoltageRatioChangeTrigger inside of the attach handler to initialize the device with this value.
         *    VoltageRatioChangeTrigger will affect the frequency of VoltageRatioChange events, by limiting them to only occur when
         *    the ratio changes by at least the value set.
         */
        void VoltageRatioInput::setChangeTrigger(double trigger)
        {
            //  find min and max change trigger
            double minChangeTrigger;
            double maxChangeTrigger;
            PhidgetReturnCode prc;
            
            prc = PhidgetVoltageRatioInput_getMinVoltageRatioChangeTrigger( mHandle, &minChangeTrigger );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting min change trigger" );
                displayError( prc );
                return;
            }
            
            prc = PhidgetVoltageRatioInput_getMaxVoltageRatioChangeTrigger( mHandle, &maxChangeTrigger );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting max change trigger" );
                displayError( prc );
                return;
            }
            
            CI_LOG_D( "Max and min voltage ratio change triggers\t Max: " << maxChangeTrigger << ", min: " << minChangeTrigger );
            CI_LOG_V( "Setting change trigger to " << mChangeTrigger );
            
            if( mChangeTrigger > maxChangeTrigger || mChangeTrigger < minChangeTrigger ) {
                CI_LOG_W( "Setting change trigger to value outside the max and min limits." );
            }
            
            prc = PhidgetVoltageRatioInput_setVoltageRatioChangeTrigger( mHandle, mChangeTrigger );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Set VoltageRatioChangeTrigger: \n\t" );
                displayError( prc );
                return;
            }
        }


		//
		//	Called by base function;
		//	adds handler for voltage ratio change
		//
		void VoltageRatioInput::setChangeHandlers()
		{
			if( setVoltageRatioHandler( mHandle, onVoltageRatioChangeHandler ) ) {
                CI_LOG_E("Unable to set Voltage Ratio Change Handler");
				return;
			}
		}


		/**
		* Sets the event handler for VoltageRatioInput's VoltageRatioChange event
		*
		* @param pvrih The PhidgetVoltageRatioInputHandle channel to add the event to
		* @param fptr The callback function to be called when a VoltageRatioChange event is fired
		* @return 0 if the operation succeeds, 1 if it fails
		*/
		int VoltageRatioInput::setVoltageRatioHandler( PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr )
		{
			PhidgetReturnCode prc;
			prc = PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler( pvrih, fptr, this );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Setting VoltageRatioChangeHandler: \n\t" );
				displayError( prc );
				return 1;
			}

			return 0;
		}


		/**
		* Outputs the VoltageRatioInput's most recently reported ratio.
		* Fired when a VoltageRatioInput channel with onVoltageRatioChangeHandler registered meets DataInterval and ChangeTrigger criteria
		*
		* @param pvrih The VoltageRatioInput channel that fired the VoltageRatioChange event
		* @param *ctx Context pointer
		* @param ratio The reported voltage ratio from the VoltageRatioInput channel
		*/
		void CCONV VoltageRatioInput::onVoltageRatioChangeHandler( PhidgetVoltageRatioInputHandle pvrih, void* ctx, double ratio )
		{
			VoltageRatioInput* voltageRatioInstance = ( VoltageRatioInput* )ctx;

			if( !voltageRatioInstance->mDelegate.expired() ) {
				voltageRatioInstance->mDelegate.lock()->voltageRatioValueChanged( ratio );
			}

			voltageRatioInstance->testCallbackFunction( ratio );
		}
        
		//
		//  test
		//
		void VoltageRatioInput::testCallbackFunction( double test )
		{
            CI_LOG_D( "Non-static callback function value: " << test );
		}
	}
}




