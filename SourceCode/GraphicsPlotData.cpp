/**
 * @file
 * The Graphics Data Store.
 *
 * This file contains a class which holds the data used when plotting the data
 * which was generated by the DeviceInterface class.
 */
#include "GraphicsPlotData.h"
#include "DeviceEvent.h"

GraphicsPlotData::GraphicsPlotData() : mpFXY()
{
	signalSize = 1;
	arrayIndex = 0;
	maxY = 0;
	minY = 0;

	array_y = new float[1];
	array_x = new float[1];
	array_y[0] = 0;
	array_x[0] = 0;

}

GraphicsPlotData::~GraphicsPlotData()
{
	delete[] array_x;
	delete[] array_y;
	array_x = NULL;
	array_y = NULL;
}

void GraphicsPlotData::SetData (float* time, float* spectrum, int length)
{
	signalSize = length;
	arrayIndex = 0;

	delete[] array_x;
	delete[] array_y;
	array_x = time;
	array_y = spectrum;

	maxY = array_y[0];
	minY = array_y[0];

	for (int ii = 1; ii < signalSize; ii++) {
		if (array_y[ii] < minY) {
			minY = array_y[ii];
		} else if (array_y[ii] > maxY) {
			maxY = array_y[ii];
		}
	}

}

GraphicsPlotData* GraphicsPlotData::Clone()
{
	float* time = new float[signalSize];
	float* spectrum = new float[signalSize];

	for (int ii = 0; ii < signalSize; ii++) {
		time[ii] = array_x[ii];
		spectrum[ii] = array_y[ii];
	}

	GraphicsPlotData* clone = new GraphicsPlotData();

	clone->SetData (&time[0], &spectrum[0], signalSize);

	wxPen pen  = this->GetPen();
	clone->SetPen (pen);

	bool continuity = this->GetContinuity();
	clone->SetContinuity (continuity);

	return clone;
}

bool GraphicsPlotData::GetNextXY (double& x, double& y)
{
	if (arrayIndex < signalSize) {
		x = (array_x[arrayIndex]);
		y = (array_y[arrayIndex]);
		arrayIndex++;
		return TRUE;
	} else {
		return FALSE;
	}
}

void GraphicsPlotData::Rewind()
{
	arrayIndex = 0;
}

double GraphicsPlotData::GetMinX()
{
	return array_x[0];
}

double GraphicsPlotData::GetMaxX()
{
	return array_x[ (signalSize -1) ];
}

double GraphicsPlotData::GetMinY()
{
	return minY;
}

double GraphicsPlotData::GetMaxY()
{
	return maxY;
}

