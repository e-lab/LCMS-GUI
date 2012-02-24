#ifndef GRAPHICS_PLOT_DATA_H
#define GRAPHICS_PLOT_DATA_H

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/utils.h>

#include <vector>
#include <math.h>

#include "GraphicsPanel.h"
#include "mathplot.h"


/**
 * Stores spectrum/time data for one measurement.
 *
 * The plot data from one measurement is kept in two arrays, member-functions
 * provide access to the data as well as supplying information about the data.
 */

class GraphicsPlotData : public mpFXY
{
public:
	/**
	 * Constructor.
	 *
	 * Creates an object empty of data.
	 */
	GraphicsPlotData();
	/**
	 * Destructor.
	 */
	~GraphicsPlotData();

	/**
	 * Reset the data arrays for this object.
	 *
	 * The time and spectrum data arrays for this object get replaced with
	 * those passed as arguments.  The ownership of the argument arrays is
	 * also passed to this object which now has the responsibility for
	 * freeing their memory.
	 *
	 * @param time
	 *   Pointer to the first element of the time array.
	 * @param spectrum
	 *   Pointer to the first element of the spectrum array.
	 * @param length
	 *   The size the arrays were before they were passed.
	 */
	void SetData (float*, float*, int);
	/**
	 * Clone the current objects state.
	 *
	 * A new GraphicsPlotData object is created and the data and state
	 * information of the current object is copied into this new object.  A
	 * reference as well as ownership of this new object is then passed to
	 * the caller.
	 *
	 * @retval clone
	 *   Pointer to a GraphicsPlotData object with the same state as the current object.
	 */
	GraphicsPlotData* Clone();

	/**
	 * Request the next pair of x/y coordinates.
	 *
	 * This class acts as an iterator over the data arrays with the first call of
	 * this function accessing the first x/y coordinate pair and each subsequent
	 * call accessing the next pair in order.  The Boolean returned indicates if
	 * the end of the arrays have been reached.
	 *
	 * @param x
	 *   Reference to the variable into which the next X-coordinate is to be written.
	 * @param y
	 *   Reference to the variable into which the next Y-coordinate is to be written.
	 * @retval TRUE
	 *   If there is more data to collect.
	 * @retval FALSE
	 *   When the end of the data array have been reached.
	 */
	bool GetNextXY (double&, double&);
	/**
	 * Reset GetNextXY() to reference the first x/y data pair.
	 */
	void Rewind();

	/**
	 * Get the minimum x value.
	 *
	 * @return
	 *   The smallest element in the x array.
	 */
	double GetMinX();
	/**
	 * Get the maximum x value.
	 *
	 * @return
	 *   The largest element in the x array.
	 */
	double GetMaxX();
	/**
	 * Get the minimum y value.
	 *
	 * @return
	 *   The smallest element in the y array.
	 */
	double GetMinY();
	/**
	 * Get the maximum y value.
	 *
	 * @return
	 *   The largest element in the y array.
	 */
	double GetMaxY();


private:
	int signalSize;
	int arrayIndex;

	float* array_y;
	float* array_x;

	double maxY;
	double minY;
};

#endif /* GRAPHICS_PLOT_DATA_H */
