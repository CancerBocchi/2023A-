#include "power_meas_sine_analyzer.h"

void POWER_MEAS_SINE_ANALYZER_run(POWER_MEAS_SINE_ANALYZER *v)
{
    v->vNorm = fabsf(v->v);
    v->iNorm = fabsf(v->i);
    v->currSign = ( v->v > v->threshold) ? 1 : 0;
    v->nSamples++;
    v->vSum = v->vSum+v->vNorm;
    v->vSqrSum = v->vSqrSum+(v->vNorm*v->vNorm);
    v->vEma = v->vEma+(v->emaFilterMultiplier*(v->vNorm - v->vEma));
    v->iSqrSum = v->iSqrSum+(v->iNorm*v->iNorm);
    v->pSum = v->pSum+(v->i*v->v);
    v->zcd=0;

    if((v->prevSign != v->currSign) && (v->currSign == 1))
    {
        
        // check if the nSamples are in the ball park of a real frequency
        // that can be on the grid, this is done by comparing the nSamples
        // with the max value and min value it can be for the 
        // AC Grid Connection these Max and Min are initialized by the 
        // user in the code
        
        if(v->nSamplesMin < v->nSamples )
        {
            v->zcd=1;
            v->inverse_nSamples = (1.0f)/(v->nSamples);
            v->sqrt_inverse_nSamples = sqrtf(v->inverse_nSamples);
            v->vAvg = (v->vSum*v->inverse_nSamples);
            v->vRms = sqrtf(v->vSqrSum)*v->sqrt_inverse_nSamples;
            v->iRms = sqrtf(v->iSqrSum)*v->sqrt_inverse_nSamples;
            v->pRmsSumMul = v->pRmsSumMul + (v->pSum*v->inverse_nSamples);
            v->vaSumMul = v->vaSumMul + v->vRms*v->iRms;
            v->acFreq = (v->sampleFreq*v->inverse_nSamples);
            v->acFreqSum = v->acFreqSum + v->acFreq;

            v->slewPowerUpdate++;

            if(v->slewPowerUpdate >= 100)
            {
                v->slewPowerUpdate=0;
                v->pRms = (v->pRmsSumMul*(0.01f));
                v->pRmsSumMul = 0;
                v->vaRms = v->vaSumMul * (0.01f);
                v->vaSumMul = 0;
                v->powerFactor=v->pRms/v->vaRms;
                v->acFreqAvg=v->acFreqSum*0.01f;
                v->acFreqSum=0;
            }

            v->jitterCount=0;

            v->nSamples=0;
            v->vSum=0;
            v->vSqrSum=0;
            v->iSqrSum=0;
            v->pSum =0;
        }
        else
        {
            //
            // otherwise it may be jitter ignore this reading
            // but count the number of jitters you are getting
            // but do not count to infinity as then when the grid comes back
            // it will take too much time to wind down the jitter count
            //
            if(v->jitterCount<30)
            {
                v->jitterCount++;
            }
            v->nSamples=0;
            
        }
    }

    if(v->nSamples>v->nSamplesMax || v->jitterCount>20)
    {
        //
        // most certainly the AC voltage is not present
        //
        v->vRms = 0;
        v->vAvg = 0;
        v->vEma = 0;
        v->acFreq=0;
        v->iRms = 0;
        v->pRms = 0;
        v->vaRms =0;
        v->powerFactor=0;

        v->zcd=0;
        v->vSum=0;
        v->vSqrSum=0;
        v->iSqrSum=0;
        v->pSum=0;
        v->vaSumMul=0;
        v->pRmsSumMul = 0;
        v->acFreqAvg = 0;
        v->acFreqSum =0 ;
        v->nSamples=0;
        v->jitterCount=0;
    }

    v->prevSign = v->currSign;
}