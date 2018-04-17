#include "PhidgetBaseInput.h"
#include "cinder/Log.h"

namespace po
{
	namespace phidget
	{
        
        //
        //    set the desired properties of the input
        //  Serial number: set to -1 to read from device
        //  Channel number
        //  Data interval: The frequency, in milliseconds, the device checks the value
        //  Change trigger: amount of change that will trigger an event; set to 0 to trigger event every interval
        //
        void BaseInput::setProperties(int serialNum, int channelNum, uint32_t dataInterval, double changeTrigger, int hubPort) {
            mSerialNumber = serialNum;
            mChannel = channelNum;
            mDataInterval = dataInterval;
            mChangeTrigger = changeTrigger;
            mHubPort = hubPort;
        }
        
		//
		//	Once input's properties are set, can set up handlers and open channel
		//
		void BaseInput::createAndOpenChannel( int timeout )
		{
			createSpecificInput();

			if( setSerialNumber( getHandle(), mSerialNumber ) ) {
				return;
			}

			if( setChannel( getHandle(), mChannel ) ) {
				return;
			}
            
            //  see if this is a hub port device
            int isHubPort = 0;
            PhidgetReturnCode prc = Phidget_getIsHubPortDevice(getHandle(), &isHubPort);
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting isHubPortDevice" );
                displayError( prc );
                return;
            }
            
            CI_LOG_V("IS this a hub port device? " << isHubPort);

            if (isHubPort && mHubPort != -1) {
                CI_LOG_V("We have an item here that's a hub port device");
            } else {
                CI_LOG_V("Problem with the hub port device");
            }

			if( setAttachDetachErrorHandlers( getHandle() ) ) {
				return;
			}

			setChangeHandlers( getHandle() );


			if( openPhidgetChannelWithTimeout( getHandle(), timeout ) ) {
				return;
			}

		}


		int BaseInput::setSerialNumber( PhidgetHandle ph, int deviceSerialNumber )
		{
			CI_LOG_V( "Setting Phidget serial number to " << deviceSerialNumber );
			PhidgetReturnCode prc;
			prc = Phidget_setDeviceSerialNumber( ph, deviceSerialNumber );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Setting DeviceSerialNumber" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

		int BaseInput::setChannel( PhidgetHandle ph, int channel )
		{
			CI_LOG_V( "Setting channel to " << channel );
			PhidgetReturnCode prc;
			prc = Phidget_setChannel( ph, channel );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Setting Channel: \n\t" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

        void BaseInput::setHubPort(PhidgetHandle ph, int hubPort)
        {
            CI_LOG_V( "Setting Phidget hub port " << hubPort );
            PhidgetReturnCode prc;
            prc = Phidget_setHubPort(ph, hubPort);
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Setting HubPort" );
                displayError( prc );
            }
        }

		int BaseInput::openPhidgetChannelWithTimeout( PhidgetHandle ch, int timeout )
		{
			PhidgetReturnCode prc;

			CI_LOG_V( "Opening Channel..." );
			prc = Phidget_openWaitForAttachment( ch, timeout );

			if( prc != EPHIDGET_OK ) {
				CI_LOG_E( "Runtime Error -> Opening Phidget Channel" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

		int BaseInput::closePhidgetChannel( PhidgetHandle ch )
		{
			PhidgetReturnCode prc;

			CI_LOG_V( "Closing Channel..." );
			prc = Phidget_close( ch );

			if( prc != EPHIDGET_OK ) {
				CI_LOG_E( "Runtime Error -> Closing Phidget Channel" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

		void BaseInput::displayError( PhidgetReturnCode code )
		{
			PhidgetReturnCode prc;
			const char* error;

			prc = Phidget_getErrorDescription( code, &error );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Getting ErrorDescription: \n\t" );
				//displayError( prc );
				return;
			}

			CI_LOG_E( "Error desc: " << error );

		}
        
        /**
         * Writes phidget error info to stderr.
         * Fired when a Phidget channel with onErrorHandler registered encounters an error in the library
         *
         * @param ph The Phidget channel that fired the error event
         * @param *ctx Context pointer
         * @param errorCode the code associated with the error of enum type Phidget_ErrorEventCode
         * @param *errorString string containing the description of the error fired
         */
        void BaseInput::onErrorHandler(PhidgetHandle ph, void *ctx, Phidget_ErrorEventCode errorCode, const char *errorString)
        {
            CI_LOG_E( "[Phidget Error Event] -> " << errorString << ", code:" << errorCode );
        }
	}
}
