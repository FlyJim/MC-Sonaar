//
//  main.h
//  essentiaRT~
//
//  Created by Cárthach Ó Nuanáin on 15/05/2014.
//
//

#ifndef essentiaRT__main_h
#define essentiaRT__main_h


/*
 flext tutorial - simple 2
 
 Copyright (c) 2002,2003 Thomas Grill (xovo@gmx.net)
 For information on usage and redistribution, and for a DISCLAIMER OF ALL
 WARRANTIES, see the file, "license.txt," in this distribution.
 
 -------------------------------------------------------------------------
 
 This is an example of a simple object doing a float addition
 */

//#define FLEXT_ATTRIBUTES 1

// include flext header
#include <flext.h>
#include "math.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using std::vector;

#include <essentia/algorithmfactory.h>
#include <essentia/essentiamath.h>
#include <essentia/pool.h>
#include <essentia/debugging.h>

using namespace std;
using namespace essentia;
using namespace essentia::standard;


#include "EssentiaOnset.h"
#include "EssentiaSFX.h"

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif

namespace Helper {

    flext::AtomList floatVectorToList(const std::vector<float>& floatVector) {
        flext::AtomList listOut(floatVector.size());
        
        for(int i=0; i<floatVector.size(); i++) {
            t_atom value;
            flext::SetFloat(value, floatVector[i]);
            
            listOut[i] = value;
        }
        return listOut;
    }

    flext::AtomList stringVectorToList(const std::vector<std::string>& stringVector) {
        flext::AtomList listOut(stringVector.size());
        
        for(int i=0; i<stringVector.size(); i++) {
            t_atom value;
            flext::SetString(value, stringVector[i].c_str());
            
            listOut[i] = value;
        }
        return listOut;
    }
    
    Real getReal(t_atom a){
        if(flext::CanbeInt(a)) return flext::GetInt(a);
        else flext::GetFloat(a);
    }

};


class essentiaRT : public flext_dsp
{
public:
    FLEXT_HEADER_S(essentiaRT, flext_dsp, setup)

    essentiaRT(int argc,const t_atom *argv);
    ~essentiaRT();
//// Flext
    void m_signal(int n, t_sample *const *insigs, t_sample *const *outsigs);    
    void my_bang();

    
    ///// Essentia Config
    //void outputListOfFeatures(const std::map<string, vector<Real> >& features);
    int sampleRate ;
    int frameSize ;
    int hopSize;
    
    
    

    
protected:

    
    //Essentia core
    EssentiaOnset onsetDetection;
    Real onset_thresh;
    
    EssentiaSFX SFX;
    Real SFXLength;
    
    
    ///essentia Environement
    int essentiaBufferCounter;
    vector<Real> audioBuffer,audioBufferOut;
    std::map<string, bool> currentAlgorithms;
    std::map<string, vector<Real> > getFeatures(Pool p);
    
    void outputListOfFeatures(const std::map<string, vector<Real> >& features,int outlet = 1);
    
    
    Pool pool;
    
    
    
    // getfeatures (why virtual?)
    virtual void m_features(int argc, const t_atom *argv);
    
    void m_settings(int argc, const t_atom *argv);
    void m_sfxAggr(void *);
    
    
    flext::Timer SFXTimer;
    bool isSFX;

private:
    static void setup(t_classid c);
    void onsetCB();
    
    FLEXT_CALLBACK_V(m_settings)
    FLEXT_CALLBACK_V(m_features)
    FLEXT_CALLBACK(my_bang)
    FLEXT_CALLBACK_T(m_sfxAggr);
    
    
    
    int framecount = 0;
    

};
FLEXT_NEW_DSP_V("essentiaRT~", essentiaRT)
#endif