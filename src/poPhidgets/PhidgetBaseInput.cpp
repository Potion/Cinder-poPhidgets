#include "PhidgetBaseInput.h"
#include "cinder/Log.h"

namespace po
{
	namespace phidget
	{
        
        //
        //  set the desired properties of the input
        //  Do this before creating and opening the channel
        //  Serial number: set to -1 to read from device
        //  Channel number
        //  Data interval: The frequency, in milliseconds, the device checks the value
        //  Change trigger: amount of change that will trigger an event; set to 0 to trigger event every interval
        //
        
        BaseInput::BaseInput()
        : mSerialNumber(-1)
        , mChannel(0)
        , mDataInterval(0)
        , mChangeTrigger(0)
        , mChangeTriggerUInt32(0)
        , mHubPort(-1)
        {}
        
		//
		//	Once input's properties are set, can set up handlers and open channel
		//
		void BaseInput::createAndOpenChannel( int timeout )
		{
			createSpecificInput();

			if( setSerialNumber( getHandle(), mSerialNumber ) ) {
                CI_LOG_E("Error setting serial number");
			}

			if( setChannel( getHandle(), mChannel ) ) {
                CI_LOG_E("Error setting channel");
			}
            
            if (mHubPort != -1) {
                if ( setHubPort(getHandle(), mHubPort) ) {
                    CI_LOG_E("Error setting hub port");
                }
            }
            
			if( setAttachDetachErrorHandlers( getHandle() ) ) {
                CI_LOG_E("Unable to set the attach, detach, and/or error handlers");
			}

			setChangeHandlers();

            if( openPhidgetChannelWithTimeout( getHandle(), timeout ) ) {
                CI_LOG_E("Unable to create channel.");
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

        int BaseInput::setHubPort(PhidgetHandle ph, int hubPort)
        {
            CI_LOG_V( "Setting Phidget hub port " << hubPort );
            PhidgetReturnCode prc;
            prc = Phidget_setHubPort(ph, hubPort);
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Setting HubPort" );
                displayError( prc );
                return 1;
            }
            
            return 0;
        }
        
        /**
         * Sets the event handlers for Phidget Attach, Phidget Detach, Phidget Error events
         *
         * @param ph The Phidget channel to add event handlers to
         * @return 0 if the operation succeeds, 1 if it fails
         */
        int BaseInput::setAttachDetachErrorHandlers( PhidgetHandle ph )
        {
            PhidgetReturnCode prc;
            prc = Phidget_setOnAttachHandler( ph, onAttachHandler, this );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Set Attach Handler" );
                displayError( prc );
                return 1;
            }
            
            prc = Phidget_setOnDetachHandler( ph, onDetachHandler, NULL );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Set Detach Handler" );
                displayError( prc );
                return 1;
            }
            
            prc = Phidget_setOnErrorHandler( ph, onErrorHandler, NULL );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Set Error Handler" );
                displayError( prc );
                return 1;
            }
            
            return 0;
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
         * Configures the device's DataInterval and ChangeTrigger.
         * Displays info about the attached phidget channel.
         * Fired when a Phidget channel with onAttachHandler registered attaches
         *
         * @param ph The Phidget channel that fired the attach event
         * @param *ctx Context pointer
         */
        void CCONV BaseInput::onAttachHandler( PhidgetHandle ph, void* ctx )
        {
            BaseInput* baseInputInstance = ( BaseInput* ) ctx;
            
            CI_LOG_V( "onAttachHandler" );
            PhidgetReturnCode prc;
            int32_t serialNumber;
            PhidgetHandle hub;
            int32_t hubPort;
            int32_t channel;
            
            //  Set data intervals
            baseInputInstance->setDataInterval( baseInputInstance->getDataInterval() );
            //  Set change trigger (type will depend on type of input)
            baseInputInstance->setChangeTrigger( baseInputInstance->getChangeTrigger() );
            baseInputInstance->setChangeTriggerUInt32( baseInputInstance->getChangeTriggerUInt32() );
            prc = Phidget_getDeviceSerialNumber( ph, &serialNumber );
            CI_LOG_V( "Getting serial number: " << serialNumber );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Get DeviceSerialNumber: \n\t" );
                displayError( prc );
                return;
            }
            
            prc = Phidget_getChannel( ph, &channel );
            CI_LOG_V( "Getting channel: " << channel );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Get Channel: \n\t" );
                displayError( prc );
                return;
            }
            
            //Check if this is a VINT device
            prc = Phidget_getHub( ph, &hub );
            CI_LOG_V( "Checking hub: " << hub );
            
            if( EPHIDGET_WRONGDEVICE != prc ) {
                prc = Phidget_getHubPort( ph, &hubPort );
                
                if( EPHIDGET_OK != prc ) {
                    CI_LOG_E( "Runtime Error -> Get HubPort: \n\t" );
                    displayError( prc );
                    return;
                }
                
                CI_LOG_V( "Attach Event: Serial Number: " << serialNumber << "\n\t-> Hub Port: " << hubPort << "\n\t-> Channel: " << channel );
            }
            else {
                CI_LOG_V( "Attach Event:Serial Number: " << serialNumber << "\n\t-> Channel: " << channel );
            }
        }

        
        /**
         * Displays info about the detached phidget channel.
         * Fired when a Phidget channel with onDetachHandler registered detaches
         *
         * @param ph The Phidget channel that fired the detach event
         * @param *ctx Context pointer
         */
        void CCONV BaseInput::onDetachHandler( PhidgetHandle ph, void* ctx )
        {
            CI_LOG_V( "onDetachHandler" );
            PhidgetReturnCode prc;
            PhidgetHandle hub;
            int serialNumber;
            int hubPort;
            int channel;
            
            prc = Phidget_getDeviceSerialNumber( ph, &serialNumber );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Get DeviceSerialNumber: \n\t" );
                displayError( prc );
                return;
            }
            
            prc = Phidget_getChannel( ph, &channel );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Get Channel: \n\t" );
                displayError( prc );
                return;
            }
            
            //Check if this is a VINT device
            prc = Phidget_getHub( ph, &hub );
            
            if( EPHIDGET_WRONGDEVICE != prc ) {
                prc = Phidget_getHubPort( ph, &hubPort );
                
                if( EPHIDGET_OK != prc ) {
                    CI_LOG_E( "Runtime Error -> Get HubPort: \n\t" );
                    displayError( prc );
                    return;
                }
                
                CI_LOG_V( "Detach Event:-> Serial Number: " << serialNumber << "\n\t-> Hub Port: " << hubPort << "\n\t-> Channel " << channel );
            }
            else {
                CI_LOG_V( "Detach Event:-> Serial Number: " << serialNumber << "\n\t-> Channel " << channel );
            }
        }

        /**
         * Writes phidget error info using Cinder Log.
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
