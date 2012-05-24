#ifndef DEVICE_VARIABLE_H
#define DEVICE_VARIABLE_H


/**
 * Class defining the names of the hardware variables/commands.
 *
 * This header should be imported by any class which need to identify a
 * command to be sent or that has been sent to the device/hardware.
 */
class Command
{
public:
	/**
	 * Enumerated list of type CommandID.
	 *
	 * This list contains all the custom command identifiers used by this
	 * program to differentiate the different communication channels of the
	 * device.
	 */
	enum CommandID {
		DEV_INIT,	//!< Initialize the hardware device.
		DEV_START,	//!< Start polling the device for data.
		DEV_STOP,	//!< Stop polling the device for data.

		PROFILE,	//!< Contains profile array, commandValue is its length.
		SAVE_TYPE,

		PA_VOLT_M,		//!<  Change the Voltage Minus value.
		PA_VOLT_P,		//!<  Change the Voltage Plus value.
		PA_VOLT_REF,		//!<  Change the Voltage Ref value.
		PA_VOLT_PUP,		//!<  Change the Voltage Pup value.
		PA_CAPACITANCE,		//!<  Change capacitance setting.
		PA_NUM_POINTS,		//!<  Change the number of data points.
		PA_MISSED_CYCLES,	//!<  Cycles missed waiting for buffer read out.
		PA_FIRST_MEAS,		//!<  Indicates the first measurement.
		PA_SAVE_TYPE,		//!<  Change the way data is saved.
		//PA_TIME_OUT,	//!<  Change the "Collection Time" time out.
		//PA_FREQUENCY	//!<  Change "Clock" frequency settings.

		SAVE_DATA,			//!<  write the data to a file.
		CLEAR,				//!<  discard the recorded data

		// ControlProtocol
		LCMS_SELECTPROTOCOL,
		PC5_PEAK_1_HEIGHT,	//!<  Change the peak 1 height.
		PC5_PEAK_2_HEIGHT,	//!<  Change the peak 2 height.
		PC5_LEAD_TIME,		//!<  Change the lead time.
		PC5_INTERVAL,		//!<  Change the measurement interval.
		PC5_PEAK_1_DUR,		//!<  Change the peak 1 duration.
		PC5_PEAK_2_DUR,		//!<  Change the peak 2 duration.

		// ControlBiases
		LCMS_INT_GBT,
		LCMS_INT_VBN,
		LCMS_INT_VBP,
		LCMS_POST_GBT,
		LCMS_POST_VBN,
		LCMS_POST_VBP,
		LCMS_OBUFF_GBT,
		LCMS_OBUFF_VBN,
		LCMS_OBUFF_VBP,
		LCMS_VREF,
		PC5_V_CMD_OFFSET,

		// ControlConfiguration
		LCMS_CAPSELECT,
		LCMS_RES_SELECT,
		LCMS_POSTGAINSELECT,
		LCMS_BYPASSPOST,
		LCMS_CHANNEL_SEL,
		LCMS_BYPASS_CDS,
		LCMS_INFILTER_SELN,
		LCMS_GAIN,

		// ControlSyncConfig
		LCMS_BYPASS_LPF,
		LCMS_VOLTAGESAMPLINGRATE,
		LCMS_RESETPERIOD,
		LCMS_INT_RESET_DURATION,
		LCMS_POST_RESET_DURATION,
		LCMS_CDS_TIME1,
		LCMS_CDS_TIME2,
		LCMS_CDS_PULSE_WIDTH,
		LCMS_MODE
	};

	struct packet {
		CommandID	commandID;
		int 		commandValue;
		wxString	filename;
		unsigned char*	profile;
	};
};

#endif /* DEVICE_VARIABLE_H */
