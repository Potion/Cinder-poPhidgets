#pragma once

#include <memory>
#include <phidget22.h>
#include "cinder/Log.h"

//
//	Abstract Phidgets input class
//

namespace po
{
	namespace phidget
	{
		class BaseInput;
		typedef std::shared_ptr<BaseInput> BaseInputRef;

		class BaseInput
			: public std::enable_shared_from_this<BaseInput>
		{
			public:
				void createAndOpenChannel( int timeout = 5000 );
				int closePhidgetChannel( PhidgetHandle ch );
				virtual PhidgetHandle getHandle() = 0;
                virtual void setProperties( int serialNum, int channelNum, uint32_t dataInterval, double changeTrigger, int hubPort = -1 );

                //  methods to be overwritten in specific input classes
                virtual void setDataIntervals( uint32_t interval ){};
                uint32_t getDataInterval(){ return mDataInterval; };
                virtual void setChangeTrigger(double trigger){};
                double getChangeTrigger() { return mChangeTrigger; };
            
			protected:
				//	properties of all inputs
				int mSerialNumber;
				int mChannel;
                uint32_t mDataInterval;
                double mChangeTrigger;
                int mHubPort;

				int setSerialNumber( PhidgetHandle ph, int deviceSerialNumber = -1 );
				int setChannel( PhidgetHandle ph, int channel = 0 );
                void setHubPort( PhidgetHandle ph, int hubPort );

				virtual int createSpecificInput() = 0;

				virtual int setAttachDetachErrorHandlers( PhidgetHandle ph ) = 0;
				virtual void setChangeHandlers( PhidgetHandle ph ) {};
				int openPhidgetChannelWithTimeout( PhidgetHandle ch, int timeout = 5000 );
            

				//	error display
				static void displayError( PhidgetReturnCode code );
            
                //  event handlers
                static void CCONV onAttachHandler( PhidgetHandle ph, void* ctx );

                static void CCONV onErrorHandler( PhidgetHandle ph, void* ctx, Phidget_ErrorEventCode errorCode, const char* errorString );



			private:
		};

	}
}
