/* 
 * File:   mcDataSeriesAnalyser.h
 * Author: chris
 *
 * Created on August 6, 2011, 11:47 AM
 */

#ifndef MCDATASERIESANALYZER_H
#define	MCDATASERIESANALYZER_H

class mcDataSeriesAnalyzer {
private:
    int nSeries;
    double** dataSeries;
    int length;
public:

    mcDataSeriesAnalyzer(int nseries, int seriesLength) {
        nSeries = nseries;
        dataSeries = new double*[nSeries];
        length=seriesLength;

    }

//    setSeriesLength(int l) {
//        length = l;
//    }

    void setDataSeries(int i, double* a) {
        dataSeries[i] = a;
    }

    
    
    void setMeasurement(int i,mcAbstractMeasurement* m){
        dataSeries[i]=m->data;
       
    }
    
    double getMean() {
        double sum = 0;
        double norm = 1.0 / nSeries / length;
        for (int n = 0; n < nSeries; n++) {
            for (int i = 0; i < length; i++) {
                sum += dataSeries[n][i];
            }
        }
        return sum*norm;
    }

    double getBinError(int nbins) {
        double sum = 0;
        int nDataPoints = nSeries * length / nbins;
        double mean=getMean();
        double err;
        double tmp;
        for (int n = 0; n < nSeries; n++) {
            for (int i = 0; i < length; i++) {
                sum += dataSeries[n][i];
                if (((n * length + i) % nDataPoints) == nDataPoints-1) {
                    tmp=(sum/nDataPoints-mean);
                    err+=tmp*tmp;
                    sum=0;
                }
            }
        }
        return sqrt(err)/(nbins-1);
    }

};


#endif	/* MCDATASERIESANALYZER_H */

