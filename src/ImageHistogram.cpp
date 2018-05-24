#include "ImageHistogram.h"
#include "qcustomplot.h"


/*******************************************************************************************************************************/
/* Simple contrast improvement */
/*******************************************************************************************************************************/
void simpleContrastImprovement (const uchar input[], int xSize, int ySize, uchar output[])
{
	/* TODO */
	uchar min = input[0];
	uchar max = input[0];

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			if (input[j*xSize + i] <= min) {
				min = input[j*xSize + i];
			}
			if (input[j*xSize + i] >= max) {
				max = input[j*xSize + i];
			}
		}
	}
	
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			output[j*xSize + i] = ((input[j*xSize + i] - min) * 255) / (max - min);
		}
	}
}


/*******************************************************************************************************************************/
/* Calculate image histogram */
/*******************************************************************************************************************************/
void calculateHistogram(const uchar input[], int xSize, int ySize, int histogram[])
{
	/* TODO */
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			histogram[input[j * xSize + i]]++;

}


/*******************************************************************************************************************************/
/* Equalize image histogram */
/*******************************************************************************************************************************/
void equalizeHistogram(const uchar input[], int xSize, int ySize, uchar output[])
{
	/* TODO */				   
	int histogram[256], cdf[256];
	int sum = 0, min = 0;

	for (int i = 0; i < 256; i++) {
		histogram[i] = 0;
	}
	
	calculateHistogram(input, xSize, ySize, histogram);

	for (int i = 0; i < 256; i++) {
		sum += histogram[i];
		cdf[i] = sum;
	}

	for (int i = 0; i < 256; i++) {
		if (cdf[i] != 0) {
			min = cdf[i];
			break;
		}
	}
	
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			output[j*xSize + i] = ((cdf[input[j*xSize + i]] - min) * 255 / (xSize*ySize - min));
		}
	}
}

/*******************************************************************************************************************************/
/* Modify colour saturation */
/*******************************************************************************************************************************/
void modifySaturation(const uchar inputRGB[], const uchar inputY[], int xSize, int ySize, uchar outputRGB[], double S)
{
	/* TODO */
	for (int j = 0; j < ySize; j++)
	{
		for (int i = 0; i < xSize; i++)
		{
			outputRGB[j*xSize * 3 + i * 3] = inputRGB[j*xSize * 3 + i * 3] * S + inputY[j*xSize + i] * (1 - S);
			outputRGB[j*xSize * 3 + i * 3 + 1] = inputRGB[j*xSize * 3 + i * 3 + 1] * S + inputY[j*xSize + i] * (1 - S);
			outputRGB[j*xSize * 3 + i * 3 + 2] = inputRGB[j*xSize * 3 + i * 3 + 2] * S + inputY[j*xSize + i] * (1 - S);
		}
	}

}

/*******************************************************************************************************************************/
/* Plot image histogram */
/* Returns QImage containing graph with histogram data. */
/* start and end parameters define first and last value for X axis */
/*******************************************************************************************************************************/
QImage createHistogramPlot(int histogram[], int start, int end)
{
	 QCustomPlot& customPlot = QCustomPlot();
	 
	 int vectorSize = end-start+1;
	 int maxValue = 0;
	 QVector<double> x(vectorSize), y(vectorSize);
	 for (int i=0; i<vectorSize; i++)
	 {
	   x[i] = i + start;  
	   y[i] = histogram[i];
	   maxValue = maxValue>=histogram[i]?maxValue:histogram[i];
	 }
	 
	 // create graph and assign data to it:
	 customPlot.addGraph();
	 customPlot.graph(0)->setData(x, y);
	 // give the axes some labels:
	 customPlot.xAxis->setLabel("x");
	 customPlot.yAxis->setLabel("y");
	 // set axes ranges, so we see all data:
	 customPlot.xAxis->setRange(start, end);
	 customPlot.yAxis->setRange(0, maxValue);
	 return customPlot.toPixmap().toImage();
}


