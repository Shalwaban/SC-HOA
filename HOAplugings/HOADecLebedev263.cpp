//----------------------------------------------------------
// name: "HOADecLebedev263"
// version: "1.0"
// author: "Pierre Lecomte"
// license: "GPL"
// copyright: "(c) Pierre Lecomte 2014"
//
// Code generated with Faust 0.9.95 (http://faust.grame.fr)
//----------------------------------------------------------

/* link with  */
#include <math.h>
#ifndef FAUSTPOWER
#define FAUSTPOWER
#include <cmath>
template <int N> inline int faustpower(int x)              { return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline int faustpower<0>(int x)            { return 1; }
template <> 	 inline int faustpower<1>(int x)            { return x; }
template <> 	 inline int faustpower<2>(int x)            { return x*x; }
template <int N> inline double faustpower(double x)          { return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline double faustpower<0>(double x)        { return 1; }
template <> 	 inline double faustpower<1>(double x)        { return x; }
template <> 	 inline double faustpower<2>(double x)        { return x*x; }
#endif
// If other than 'faust2sc --prefix Faust' is used, sed this as well:
#if !defined(SC_FAUST_PREFIX)
# define SC_FAUST_PREFIX "Faust"
#endif

//-------------------------------------------------------------------
// FAUST architecture file for SuperCollider.
// Copyright (C) 2005-2012 Stefan Kersten.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
// 02111-1307 USA
//-------------------------------------------------------------------

#include <map>
#include <string>
#include <string.h>
#include <SC_PlugIn.h>

/************************************************************************
    IMPORTANT NOTE : this file contains two clearly delimited sections :
    the ARCHITECTURE section (in two parts) and the USER section. Each section
    is governed by its own copyright and license. Please check individually
    each section for license and copyright information.
*************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
    FAUST Architecture File
    Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; If not, see <http://www.gnu.org/licenses/>.

    EXCEPTION : As a special exception, you may create a larger work
    that contains this FAUST architecture section and distribute
    that work under terms of your choice, so long as this FAUST
    architecture section is not modified.

 ************************************************************************
 ************************************************************************/
 
/******************************************************************************
*******************************************************************************

								FAUST DSP

*******************************************************************************
*******************************************************************************/

#ifndef __dsp__
#define __dsp__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class UI;
struct Meta;

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the UI* parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the UI* user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /** Global init, calls the following methods :
         * - static class 'classInit' : static table initialisation
         * - 'instanceInit' : constants and instance table initialisation
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void init(int samplingRate) = 0;
    
        /** Init instance state
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void instanceInit(int samplingRate) = 0;
    
        /** Init instance constant state
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void instanceConstants(int samplingRate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
    
        /**  
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value metadata).
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with sucessive in/out audio buffers.
         *
         * @param count - the nomber of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation : alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the nomber of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         *
         */
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = 0):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int samplingRate) { fDSP->init(samplingRate); }
        virtual void instanceInit(int samplingRate) { fDSP->instanceInit(samplingRate); }
        virtual void instanceConstants(int samplingRate) { fDSP->instanceConstants(samplingRate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { return fDSP->metadata(m); }
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
       
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/************************************************************************
    FAUST Architecture File
    Copyright (C) 2003-2016 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; If not, see <http://www.gnu.org/licenses/>.

    EXCEPTION : As a special exception, you may create a larger work
    that contains this FAUST architecture section and distribute
    that work under terms of your choice, so long as this FAUST
    architecture section is not modified.


 ************************************************************************
 ************************************************************************/
 
#ifndef FAUST_UI_H
#define FAUST_UI_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust User Interface
 * This abstract class contains only the method that the faust compiler can
 * generate to describe a DSP interface.
 ******************************************************************************/

class UI
{

    public:

        UI() {}

        virtual ~UI() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
};

//----------------------------------------------------------------
//  Generic decorator
//----------------------------------------------------------------

class DecoratorUI : public UI
{
    protected:
    
        UI* fUI;

    public:
    
        DecoratorUI(UI* ui = 0):fUI(ui)
        {}

        virtual ~DecoratorUI() { delete fUI; }

        // -- widget's layouts
        virtual void openTabBox(const char* label)          { fUI->openTabBox(label); }
        virtual void openHorizontalBox(const char* label)   { fUI->openHorizontalBox(label); }
        virtual void openVerticalBox(const char* label)     { fUI->openVerticalBox(label); }
        virtual void closeBox()                             { fUI->closeBox(); }

        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone)         { fUI->addButton(label, zone); }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)    { fUI->addCheckButton(label, zone); }
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
            { fUI->addVerticalSlider(label, zone, init, min, max, step); }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) 	
            { fUI->addHorizontalSlider(label, zone, init, min, max, step); }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) 			
            { fUI->addNumEntry(label, zone, init, min, max, step); }

        // -- passive widgets	
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) 
            { fUI->addHorizontalBargraph(label, zone, min, max); }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
            { fUI->addVerticalBargraph(label, zone, min, max); }

        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) { fUI->declare(zone, key, val); }

};

#endif
/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3 of
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
	along with this program; If not, see <http://www.gnu.org/licenses/>.

 ************************************************************************
 ************************************************************************/
 
#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <string.h>
#include <stdlib.h>

/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3 of
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
	along with this program; If not, see <http://www.gnu.org/licenses/>.

 ************************************************************************
 ************************************************************************/
 
#ifndef __meta__
#define __meta__

struct Meta
{
    virtual void declare(const char* key, const char* value) = 0;
    virtual ~Meta() {};
};

#endif

using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

inline int lsr(int x, int n)	{ return int(((unsigned int)x) >> n); }

inline int int2pow2(int x)		{ int r = 0; while ((1<<r) < x) r++; return r; }

inline long lopt(char* argv[], const char* name, long def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
	return def;
}

inline bool isopt(char* argv[], const char* name)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
	return false;
}

inline const char* lopts(char* argv[], const char* name, const char* def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
	return def;
}

#endif


using namespace std;

#if defined(__GNUC__) && __GNUC__ >= 4
# define FAUST_EXPORT __attribute__((visibility("default")))
#else
# define FAUST_EXPORT  SC_API_EXPORT
#endif

//----------------------------------------------------------------------------
// Vector intrinsics
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Metadata
//----------------------------------------------------------------------------

class MetaData : public Meta
               , public std::map<std::string, std::string>
{
public:
    void declare(const char* key, const char* value)
    {
        (*this)[key] = value;
    }
};

//----------------------------------------------------------------------------
// Control counter
//----------------------------------------------------------------------------

class ControlCounter : public UI
{
public:
    ControlCounter()
        : mNumControlInputs(0),
          mNumControlOutputs(0)
    { }

    size_t getNumControls() const { return getNumControlInputs(); }
    size_t getNumControlInputs() const { return mNumControlInputs; }
    size_t getNumControlOutputs() const { return mNumControlOutputs; }

    // Layout widgets
    virtual void openTabBox(const char* label) { }
    virtual void openHorizontalBox(const char* label) { }
    virtual void openVerticalBox(const char* label) { }
    virtual void closeBox() { }

    // Active widgets
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    { addControlInput(); }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    { addControlInput(); }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }

    // Passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    { addControlOutput(); }
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    { addControlOutput(); }

protected:
    void addControlInput() { mNumControlInputs++; }
    void addControlOutput() { mNumControlOutputs++; }

private:
    size_t mNumControlInputs;
    size_t mNumControlOutputs;
};

//----------------------------------------------------------------------------
// UI control
//----------------------------------------------------------------------------

struct Control
{
    typedef void (*UpdateFunction)(Control* self, FAUSTFLOAT value);

    UpdateFunction updateFunction;
    FAUSTFLOAT* zone;
    FAUSTFLOAT min, max;

    inline void update(FAUSTFLOAT value)
    {
        (*updateFunction)(this, value);
    }

    static void simpleUpdate(Control* self, FAUSTFLOAT value)
    {
        *self->zone = value;
    }
    static void boundedUpdate(Control* self, FAUSTFLOAT value)
    {
        *self->zone = sc_clip(value, self->min, self->max);
    }
};

//----------------------------------------------------------------------------
// Control allocator
//----------------------------------------------------------------------------

class ControlAllocator : public UI
{
public:
    ControlAllocator(Control* controls)
        : mControls(controls)
    { }

    // Layout widgets
    virtual void openTabBox(const char* label) { }
    virtual void openHorizontalBox(const char* label) { }
    virtual void openVerticalBox(const char* label) { }
    virtual void closeBox() { }

    // Active widgets
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    { addSimpleControl(zone); }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    { addSimpleControl(zone); }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }

    // Passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) { }
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) { }

private:
    void addControl(Control::UpdateFunction updateFunction, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT /* step */)
    {
        Control* ctrl        = mControls++;
        ctrl->updateFunction = updateFunction;
        ctrl->zone           = zone;
        ctrl->min            = min;
        ctrl->max            = max;
    }
    void addSimpleControl(FAUSTFLOAT* zone)
    {
        addControl(Control::simpleUpdate, zone, 0.f, 0.f, 0.f);
    }
    void addBoundedControl(FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        addControl(Control::boundedUpdate, zone, min, max, step);
    }

private:
    Control* mControls;
};

//----------------------------------------------------------------------------
// FAUST generated code
//----------------------------------------------------------------------------

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  


#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

class mydsp : public dsp {
  private:
	double 	fConst0;
	double 	fConst1;
	FAUSTFLOAT 	fslider0;
	double 	fRec1[2];
	FAUSTFLOAT 	fcheckbox0;
	FAUSTFLOAT 	fentry0;
	FAUSTFLOAT 	fslider1;
	double 	fRec3[2];
	double 	fRec2[2];
	FAUSTFLOAT 	fbargraph0;
	double 	fConst2;
	double 	fConst3;
	double 	fConst4;
	double 	fConst5;
	double 	fRec7[2];
	double 	fRec8[2];
	double 	fRec6[2];
	double 	fConst6;
	double 	fRec9[2];
	double 	fRec5[2];
	double 	fRec4[2];
	FAUSTFLOAT 	fbargraph1;
	double 	fRec13[2];
	double 	fRec14[2];
	double 	fRec12[2];
	double 	fRec15[2];
	double 	fRec11[2];
	double 	fRec10[2];
	FAUSTFLOAT 	fbargraph2;
	double 	fRec19[2];
	double 	fRec20[2];
	double 	fRec18[2];
	double 	fRec21[2];
	double 	fRec17[2];
	double 	fRec16[2];
	FAUSTFLOAT 	fbargraph3;
	double 	fRec25[2];
	double 	fRec26[2];
	double 	fRec24[2];
	double 	fRec27[2];
	double 	fRec23[2];
	double 	fRec22[2];
	FAUSTFLOAT 	fbargraph4;
	double 	fConst7;
	double 	fConst8;
	double 	fRec30[2];
	double 	fRec31[2];
	double 	fRec29[2];
	double 	fRec28[2];
	FAUSTFLOAT 	fbargraph5;
	double 	fRec34[2];
	double 	fRec35[2];
	double 	fRec33[2];
	double 	fRec32[2];
	FAUSTFLOAT 	fbargraph6;
	double 	fRec38[2];
	double 	fRec39[2];
	double 	fRec37[2];
	double 	fRec36[2];
	FAUSTFLOAT 	fbargraph7;
	double 	fConst9;
	double 	fRec42[2];
	double 	fRec41[2];
	double 	fRec40[2];
	FAUSTFLOAT 	fbargraph8;
	double 	fRec45[2];
	double 	fRec44[2];
	double 	fRec43[2];
	FAUSTFLOAT 	fbargraph9;
	double 	fRec0[2];
	FAUSTFLOAT 	fbargraph10;
	double 	fRec46[2];
	FAUSTFLOAT 	fbargraph11;
	double 	fRec50[2];
	double 	fRec51[2];
	double 	fRec49[2];
	double 	fRec48[2];
	FAUSTFLOAT 	fbargraph12;
	double 	fRec54[2];
	double 	fRec53[2];
	double 	fRec52[2];
	FAUSTFLOAT 	fbargraph13;
	double 	fRec58[2];
	double 	fRec59[2];
	double 	fRec57[2];
	double 	fRec60[2];
	double 	fRec56[2];
	double 	fRec55[2];
	FAUSTFLOAT 	fbargraph14;
	double 	fRec64[2];
	double 	fRec65[2];
	double 	fRec63[2];
	double 	fRec66[2];
	double 	fRec62[2];
	double 	fRec61[2];
	FAUSTFLOAT 	fbargraph15;
	double 	fRec47[2];
	FAUSTFLOAT 	fbargraph16;
	double 	fRec67[2];
	FAUSTFLOAT 	fbargraph17;
	double 	fRec68[2];
	FAUSTFLOAT 	fbargraph18;
	double 	fRec69[2];
	FAUSTFLOAT 	fbargraph19;
	double 	fRec70[2];
	FAUSTFLOAT 	fbargraph20;
	double 	fRec74[2];
	double 	fRec75[2];
	double 	fRec73[2];
	double 	fRec72[2];
	FAUSTFLOAT 	fbargraph21;
	double 	fRec79[2];
	double 	fRec80[2];
	double 	fRec78[2];
	double 	fRec81[2];
	double 	fRec77[2];
	double 	fRec76[2];
	FAUSTFLOAT 	fbargraph22;
	double 	fRec71[2];
	FAUSTFLOAT 	fbargraph23;
	double 	fRec82[2];
	FAUSTFLOAT 	fbargraph24;
	double 	fRec83[2];
	FAUSTFLOAT 	fbargraph25;
	double 	fRec84[2];
	FAUSTFLOAT 	fbargraph26;
	double 	fRec85[2];
	FAUSTFLOAT 	fbargraph27;
	double 	fRec86[2];
	FAUSTFLOAT 	fbargraph28;
	double 	fRec87[2];
	FAUSTFLOAT 	fbargraph29;
	double 	fRec88[2];
	FAUSTFLOAT 	fbargraph30;
	double 	fRec89[2];
	FAUSTFLOAT 	fbargraph31;
	double 	fRec90[2];
	FAUSTFLOAT 	fbargraph32;
	double 	fRec91[2];
	FAUSTFLOAT 	fbargraph33;
	double 	fRec92[2];
	FAUSTFLOAT 	fbargraph34;
	double 	fRec93[2];
	FAUSTFLOAT 	fbargraph35;
	double 	fRec94[2];
	FAUSTFLOAT 	fbargraph36;
	double 	fRec95[2];
	FAUSTFLOAT 	fbargraph37;
	double 	fRec96[2];
	FAUSTFLOAT 	fbargraph38;
	double 	fRec97[2];
	FAUSTFLOAT 	fbargraph39;
	double 	fRec98[2];
	FAUSTFLOAT 	fbargraph40;
	double 	fRec99[2];
	FAUSTFLOAT 	fbargraph41;
	int fSamplingFreq;

  public:
	virtual void metadata(Meta* m) { 
		m->declare("name", "HOADecLebedev263");
		m->declare("version", "1.0");
		m->declare("author", "Pierre Lecomte");
		m->declare("license", "GPL");
		m->declare("copyright", "(c) Pierre Lecomte 2014");
		m->declare("lib/ymn.lib/name", "Spherical Harmonics library");
		m->declare("lib/ymn.lib/version", "1.0");
		m->declare("lib/ymn.lib/author", "Pierre Lecomte");
		m->declare("lib/ymn.lib/license", "GPL");
		m->declare("lib/ymn.lib/copyright", "(c) Pierre Lecomte 2016");
		m->declare("lib/nfc.lib/name", "NF And NFC Filters Library");
		m->declare("lib/nfc.lib/version", "1.0");
		m->declare("lib/nfc.lib/author", "Pierre Lecomte");
		m->declare("lib/nfc.lib/license", "GPL");
		m->declare("lib/nfc.lib/copyright", "(c) Pierre Lecomte 2014");
		m->declare("lib/lebedev.lib/name", "Lebdev grids and weights");
		m->declare("lib/lebedev.lib/version", "1.0");
		m->declare("lib/lebedev.lib/author", "Pierre Lecomte");
		m->declare("lib/lebedev.lib/license", "GPL");
		m->declare("lib/lebedev.lib/copyright", "(c) Pierre Lecomte 2014");
		m->declare("lib/gui.lib/name", "GUI Library");
		m->declare("lib/gui.lib/version", "1.0");
		m->declare("lib/gui.lib/author", "Pierre Lecomte");
		m->declare("lib/gui.lib/license", "GPL");
		m->declare("lib/gui.lib/copyright", "(c) Pierre Lecomte 2016");
		m->declare("signal.lib/name", "Faust Signal Routing Library");
		m->declare("signal.lib/version", "0.0");
		m->declare("basic.lib/name", "Faust Basic Element Library");
		m->declare("basic.lib/version", "0.0");
		m->declare("math.lib/name", "Faust Math Library");
		m->declare("math.lib/version", "2.0");
		m->declare("math.lib/author", "GRAME");
		m->declare("math.lib/copyright", "GRAME");
		m->declare("math.lib/license", "LGPL with exception");
	}

	virtual int getNumInputs() { return 16; }
	virtual int getNumOutputs() { return 26; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = min(1.92e+05, max(1.0, (double)fSamplingFreq));
		fConst1 = (8e+01 / fConst0);
		fConst2 = (394.7715102864353 / fConst0);
		fConst3 = (625.2284897135664 / fConst0);
		fConst4 = (1.7e+02 / fConst0);
		fConst5 = (1250.4569794271329 / fConst0);
		fConst6 = (789.5430205728705 / fConst0);
		fConst7 = (5.1e+02 / fConst0);
		fConst8 = (1.02e+03 / fConst0);
		fConst9 = (3.4e+02 / fConst0);
	}
	virtual void instanceResetUserInterface() {
		fslider0 = 0.0;
		fcheckbox0 = 0.0;
		fentry0 = 1.07;
		fslider1 = 0.0;
	}
	virtual void instanceClear() {
		for (int i=0; i<2; i++) fRec1[i] = 0;
		for (int i=0; i<2; i++) fRec3[i] = 0;
		for (int i=0; i<2; i++) fRec2[i] = 0;
		for (int i=0; i<2; i++) fRec7[i] = 0;
		for (int i=0; i<2; i++) fRec8[i] = 0;
		for (int i=0; i<2; i++) fRec6[i] = 0;
		for (int i=0; i<2; i++) fRec9[i] = 0;
		for (int i=0; i<2; i++) fRec5[i] = 0;
		for (int i=0; i<2; i++) fRec4[i] = 0;
		for (int i=0; i<2; i++) fRec13[i] = 0;
		for (int i=0; i<2; i++) fRec14[i] = 0;
		for (int i=0; i<2; i++) fRec12[i] = 0;
		for (int i=0; i<2; i++) fRec15[i] = 0;
		for (int i=0; i<2; i++) fRec11[i] = 0;
		for (int i=0; i<2; i++) fRec10[i] = 0;
		for (int i=0; i<2; i++) fRec19[i] = 0;
		for (int i=0; i<2; i++) fRec20[i] = 0;
		for (int i=0; i<2; i++) fRec18[i] = 0;
		for (int i=0; i<2; i++) fRec21[i] = 0;
		for (int i=0; i<2; i++) fRec17[i] = 0;
		for (int i=0; i<2; i++) fRec16[i] = 0;
		for (int i=0; i<2; i++) fRec25[i] = 0;
		for (int i=0; i<2; i++) fRec26[i] = 0;
		for (int i=0; i<2; i++) fRec24[i] = 0;
		for (int i=0; i<2; i++) fRec27[i] = 0;
		for (int i=0; i<2; i++) fRec23[i] = 0;
		for (int i=0; i<2; i++) fRec22[i] = 0;
		for (int i=0; i<2; i++) fRec30[i] = 0;
		for (int i=0; i<2; i++) fRec31[i] = 0;
		for (int i=0; i<2; i++) fRec29[i] = 0;
		for (int i=0; i<2; i++) fRec28[i] = 0;
		for (int i=0; i<2; i++) fRec34[i] = 0;
		for (int i=0; i<2; i++) fRec35[i] = 0;
		for (int i=0; i<2; i++) fRec33[i] = 0;
		for (int i=0; i<2; i++) fRec32[i] = 0;
		for (int i=0; i<2; i++) fRec38[i] = 0;
		for (int i=0; i<2; i++) fRec39[i] = 0;
		for (int i=0; i<2; i++) fRec37[i] = 0;
		for (int i=0; i<2; i++) fRec36[i] = 0;
		for (int i=0; i<2; i++) fRec42[i] = 0;
		for (int i=0; i<2; i++) fRec41[i] = 0;
		for (int i=0; i<2; i++) fRec40[i] = 0;
		for (int i=0; i<2; i++) fRec45[i] = 0;
		for (int i=0; i<2; i++) fRec44[i] = 0;
		for (int i=0; i<2; i++) fRec43[i] = 0;
		for (int i=0; i<2; i++) fRec0[i] = 0;
		for (int i=0; i<2; i++) fRec46[i] = 0;
		for (int i=0; i<2; i++) fRec50[i] = 0;
		for (int i=0; i<2; i++) fRec51[i] = 0;
		for (int i=0; i<2; i++) fRec49[i] = 0;
		for (int i=0; i<2; i++) fRec48[i] = 0;
		for (int i=0; i<2; i++) fRec54[i] = 0;
		for (int i=0; i<2; i++) fRec53[i] = 0;
		for (int i=0; i<2; i++) fRec52[i] = 0;
		for (int i=0; i<2; i++) fRec58[i] = 0;
		for (int i=0; i<2; i++) fRec59[i] = 0;
		for (int i=0; i<2; i++) fRec57[i] = 0;
		for (int i=0; i<2; i++) fRec60[i] = 0;
		for (int i=0; i<2; i++) fRec56[i] = 0;
		for (int i=0; i<2; i++) fRec55[i] = 0;
		for (int i=0; i<2; i++) fRec64[i] = 0;
		for (int i=0; i<2; i++) fRec65[i] = 0;
		for (int i=0; i<2; i++) fRec63[i] = 0;
		for (int i=0; i<2; i++) fRec66[i] = 0;
		for (int i=0; i<2; i++) fRec62[i] = 0;
		for (int i=0; i<2; i++) fRec61[i] = 0;
		for (int i=0; i<2; i++) fRec47[i] = 0;
		for (int i=0; i<2; i++) fRec67[i] = 0;
		for (int i=0; i<2; i++) fRec68[i] = 0;
		for (int i=0; i<2; i++) fRec69[i] = 0;
		for (int i=0; i<2; i++) fRec70[i] = 0;
		for (int i=0; i<2; i++) fRec74[i] = 0;
		for (int i=0; i<2; i++) fRec75[i] = 0;
		for (int i=0; i<2; i++) fRec73[i] = 0;
		for (int i=0; i<2; i++) fRec72[i] = 0;
		for (int i=0; i<2; i++) fRec79[i] = 0;
		for (int i=0; i<2; i++) fRec80[i] = 0;
		for (int i=0; i<2; i++) fRec78[i] = 0;
		for (int i=0; i<2; i++) fRec81[i] = 0;
		for (int i=0; i<2; i++) fRec77[i] = 0;
		for (int i=0; i<2; i++) fRec76[i] = 0;
		for (int i=0; i<2; i++) fRec71[i] = 0;
		for (int i=0; i<2; i++) fRec82[i] = 0;
		for (int i=0; i<2; i++) fRec83[i] = 0;
		for (int i=0; i<2; i++) fRec84[i] = 0;
		for (int i=0; i<2; i++) fRec85[i] = 0;
		for (int i=0; i<2; i++) fRec86[i] = 0;
		for (int i=0; i<2; i++) fRec87[i] = 0;
		for (int i=0; i<2; i++) fRec88[i] = 0;
		for (int i=0; i<2; i++) fRec89[i] = 0;
		for (int i=0; i<2; i++) fRec90[i] = 0;
		for (int i=0; i<2; i++) fRec91[i] = 0;
		for (int i=0; i<2; i++) fRec92[i] = 0;
		for (int i=0; i<2; i++) fRec93[i] = 0;
		for (int i=0; i<2; i++) fRec94[i] = 0;
		for (int i=0; i<2; i++) fRec95[i] = 0;
		for (int i=0; i<2; i++) fRec96[i] = 0;
		for (int i=0; i<2; i++) fRec97[i] = 0;
		for (int i=0; i<2; i++) fRec98[i] = 0;
		for (int i=0; i<2; i++) fRec99[i] = 0;
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void instanceInit(int samplingFreq) {
		instanceConstants(samplingFreq);
		instanceResetUserInterface();
		instanceClear();
	}
	virtual mydsp* clone() {
		return new mydsp();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
	}
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("0x00");
		ui_interface->openHorizontalBox("Inputs");
		ui_interface->openHorizontalBox("B-Format");
		ui_interface->openHorizontalBox("0");
		ui_interface->openVerticalBox("0");
		ui_interface->declare(&fbargraph0, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422810a0", &fbargraph0, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("1");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fbargraph13, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422aa2a0", &fbargraph13, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fbargraph8, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd54229e2a0", &fbargraph8, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fbargraph9, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422a0b60", &fbargraph9, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2");
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fbargraph12, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422a7a90", &fbargraph12, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fbargraph21, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422c1550", &fbargraph21, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fbargraph7, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd54229b5b0", &fbargraph7, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fbargraph6, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422986b0", &fbargraph6, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fbargraph5, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd542295670", &fbargraph5, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3");
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fbargraph15, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422b1b10", &fbargraph15, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fbargraph22, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422c52e0", &fbargraph22, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fbargraph14, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422adfc0", &fbargraph14, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fbargraph4, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd542291980", &fbargraph4, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd54228dfa0", &fbargraph3, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd54228a4c0", &fbargraph2, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd542286940", &fbargraph1, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->declare(&fslider1, "1", "");
		ui_interface->declare(&fslider1, "osc", "/levelin -10 60");
		ui_interface->declare(&fslider1, "unit", "dB");
		ui_interface->addVerticalSlider("Inputs Gain", &fslider1, 0.0, -1e+01, 6e+01, 0.1);
		ui_interface->declare(&fslider0, "2", "");
		ui_interface->declare(&fslider0, "osc", "/levelout -10 60");
		ui_interface->declare(&fslider0, "unit", "dB");
		ui_interface->addVerticalSlider("Outputs Gain", &fslider0, 0.0, -1e+01, 6e+01, 0.1);
		ui_interface->declare(0, "3", "");
		ui_interface->openVerticalBox("NFC");
		ui_interface->addCheckButton("Yes", &fcheckbox0);
		ui_interface->closeBox();
		ui_interface->declare(&fentry0, "4", "");
		ui_interface->addNumEntry("Speakers Radius", &fentry0, 1.07, 0.5, 1e+01, 0.01);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Outputs 1-outs/2");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fbargraph10, "osc", "/output1");
		ui_interface->declare(&fbargraph10, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422a2a30", &fbargraph10, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fbargraph11, "osc", "/output2");
		ui_interface->declare(&fbargraph11, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422a4ab0", &fbargraph11, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fbargraph16, "osc", "/output3");
		ui_interface->declare(&fbargraph16, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422b3760", &fbargraph16, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fbargraph17, "osc", "/output4");
		ui_interface->declare(&fbargraph17, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422b5d80", &fbargraph17, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fbargraph18, "osc", "/output5");
		ui_interface->declare(&fbargraph18, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422b8580", &fbargraph18, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fbargraph19, "osc", "/output6");
		ui_interface->declare(&fbargraph19, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422ba240", &fbargraph19, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fbargraph20, "osc", "/output7");
		ui_interface->declare(&fbargraph20, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422bd3b0", &fbargraph20, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fbargraph23, "osc", "/output8");
		ui_interface->declare(&fbargraph23, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422c7150", &fbargraph23, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fbargraph24, "osc", "/output9");
		ui_interface->declare(&fbargraph24, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422ca000", &fbargraph24, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fbargraph25, "osc", "/output10");
		ui_interface->declare(&fbargraph25, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422cd090", &fbargraph25, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fbargraph26, "osc", "/output11");
		ui_interface->declare(&fbargraph26, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422cffe0", &fbargraph26, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fbargraph27, "osc", "/output12");
		ui_interface->declare(&fbargraph27, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422d2820", &fbargraph27, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fbargraph28, "osc", "/output13");
		ui_interface->declare(&fbargraph28, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422d4d70", &fbargraph28, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Outputs 14-26");
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fbargraph29, "osc", "/output14");
		ui_interface->declare(&fbargraph29, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422d7400", &fbargraph29, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fbargraph30, "osc", "/output15");
		ui_interface->declare(&fbargraph30, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422d9160", &fbargraph30, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fbargraph31, "osc", "/output16");
		ui_interface->declare(&fbargraph31, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422db6b0", &fbargraph31, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fbargraph32, "osc", "/output17");
		ui_interface->declare(&fbargraph32, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422dd940", &fbargraph32, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fbargraph33, "osc", "/output18");
		ui_interface->declare(&fbargraph33, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422dfd50", &fbargraph33, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fbargraph34, "osc", "/output19");
		ui_interface->declare(&fbargraph34, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422e4030", &fbargraph34, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fbargraph35, "osc", "/output20");
		ui_interface->declare(&fbargraph35, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422e7680", &fbargraph35, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fbargraph36, "osc", "/output21");
		ui_interface->declare(&fbargraph36, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422ea5c0", &fbargraph36, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fbargraph37, "osc", "/output22");
		ui_interface->declare(&fbargraph37, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422ed4b0", &fbargraph37, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fbargraph38, "osc", "/output23");
		ui_interface->declare(&fbargraph38, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422efd90", &fbargraph38, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fbargraph39, "osc", "/output24");
		ui_interface->declare(&fbargraph39, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422f23f0", &fbargraph39, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("25");
		ui_interface->declare(&fbargraph40, "osc", "/output25");
		ui_interface->declare(&fbargraph40, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422f4c70", &fbargraph40, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("26");
		ui_interface->declare(&fbargraph41, "osc", "/output26");
		ui_interface->declare(&fbargraph41, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7fd5422f7960", &fbargraph41, -7e+01, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		double 	fSlow0 = (0.0010000000000000009 * pow(10,(0.05 * double(fslider0))));
		double 	fSlow1 = double(fcheckbox0);
		double 	fSlow2 = double(fentry0);
		double 	fSlow3 = (1 - (fSlow1 * (1 - fSlow2)));
		double 	fSlow4 = (0.0010000000000000009 * pow(10,(0.05 * double(fslider1))));
		double 	fSlow5 = (1.0 / ((fConst2 / fSlow2) + 1));
		double 	fSlow6 = (fConst4 / fSlow2);
		double 	fSlow7 = faustpower<2>(fSlow6);
		double 	fSlow8 = (1.0 / (((fConst3 / fSlow2) + (6.45943269348338 * fSlow7)) + 1));
		double 	fSlow9 = (25.83773077393352 * fSlow7);
		double 	fSlow10 = (fSlow9 + (fConst5 / fSlow2));
		double 	fSlow11 = (fSlow1 * fSlow2);
		double 	fSlow12 = (fConst6 / fSlow2);
		double 	fSlow13 = (1 - fSlow1);
		double 	fSlow14 = (1.0 / (((3 * fSlow7) + (fConst7 / fSlow2)) + 1));
		double 	fSlow15 = (12 * fSlow7);
		double 	fSlow16 = (fSlow15 + (fConst8 / fSlow2));
		double 	fSlow17 = (1.0 / (fSlow6 + 1));
		double 	fSlow18 = (fConst9 / fSlow2);
		FAUSTFLOAT* input0 = input[0];
		FAUSTFLOAT* input1 = input[1];
		FAUSTFLOAT* input2 = input[2];
		FAUSTFLOAT* input3 = input[3];
		FAUSTFLOAT* input4 = input[4];
		FAUSTFLOAT* input5 = input[5];
		FAUSTFLOAT* input6 = input[6];
		FAUSTFLOAT* input7 = input[7];
		FAUSTFLOAT* input8 = input[8];
		FAUSTFLOAT* input9 = input[9];
		FAUSTFLOAT* input10 = input[10];
		FAUSTFLOAT* input11 = input[11];
		FAUSTFLOAT* input12 = input[12];
		FAUSTFLOAT* input13 = input[13];
		FAUSTFLOAT* input14 = input[14];
		FAUSTFLOAT* input15 = input[15];
		FAUSTFLOAT* output0 = output[0];
		FAUSTFLOAT* output1 = output[1];
		FAUSTFLOAT* output2 = output[2];
		FAUSTFLOAT* output3 = output[3];
		FAUSTFLOAT* output4 = output[4];
		FAUSTFLOAT* output5 = output[5];
		FAUSTFLOAT* output6 = output[6];
		FAUSTFLOAT* output7 = output[7];
		FAUSTFLOAT* output8 = output[8];
		FAUSTFLOAT* output9 = output[9];
		FAUSTFLOAT* output10 = output[10];
		FAUSTFLOAT* output11 = output[11];
		FAUSTFLOAT* output12 = output[12];
		FAUSTFLOAT* output13 = output[13];
		FAUSTFLOAT* output14 = output[14];
		FAUSTFLOAT* output15 = output[15];
		FAUSTFLOAT* output16 = output[16];
		FAUSTFLOAT* output17 = output[17];
		FAUSTFLOAT* output18 = output[18];
		FAUSTFLOAT* output19 = output[19];
		FAUSTFLOAT* output20 = output[20];
		FAUSTFLOAT* output21 = output[21];
		FAUSTFLOAT* output22 = output[22];
		FAUSTFLOAT* output23 = output[23];
		FAUSTFLOAT* output24 = output[24];
		FAUSTFLOAT* output25 = output[25];
		for (int i=0; i<count; i++) {
			fRec1[0] = (fSlow0 + (0.999 * fRec1[1]));
			fRec3[0] = (fSlow4 + (0.999 * fRec3[1]));
			double fTemp0 = (fSlow3 * ((double)input0[i] * fRec3[0]));
			fRec2[0] = max((fRec2[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp0))))));
			fbargraph0 = fRec2[0];
			double fTemp1 = fTemp0;
			double fTemp2 = (0.04761904761904762 * fTemp1);
			fRec7[0] = (fRec6[1] + fRec7[1]);
			fRec8[0] = (fRec7[1] + fRec8[1]);
			double fTemp3 = ((double)input15[i] * fRec3[0]);
			fRec6[0] = (0 - (fSlow8 * (((fSlow10 * fRec7[0]) + (fSlow9 * fRec8[0])) - (fSlow11 * fTemp3))));
			fRec9[0] = (fRec5[1] + fRec9[1]);
			fRec5[0] = (fSlow5 * (fRec6[0] - (fSlow12 * fRec9[0])));
			double fTemp4 = (fRec5[0] + (fSlow13 * fTemp3));
			fRec4[0] = max((fRec4[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp4))))));
			fbargraph1 = fRec4[0];
			double fTemp5 = fTemp4;
			double fTemp6 = (2.2867163770701174e-50 * fTemp5);
			fRec13[0] = (fRec12[1] + fRec13[1]);
			fRec14[0] = (fRec13[1] + fRec14[1]);
			double fTemp7 = ((double)input14[i] * fRec3[0]);
			fRec12[0] = (0 - (fSlow8 * (((fSlow10 * fRec13[0]) + (fSlow9 * fRec14[0])) - (fSlow11 * fTemp7))));
			fRec15[0] = (fRec11[1] + fRec15[1]);
			fRec11[0] = (fSlow5 * (fRec12[0] - (fSlow12 * fRec15[0])));
			double fTemp8 = (fRec11[0] + (fSlow13 * fTemp7));
			fRec10[0] = max((fRec10[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp8))))));
			fbargraph2 = fRec10[0];
			double fTemp9 = fTemp8;
			double fTemp10 = (9.147598008156145e-34 * fTemp9);
			fRec19[0] = (fRec18[1] + fRec19[1]);
			fRec20[0] = (fRec19[1] + fRec20[1]);
			double fTemp11 = ((double)input13[i] * fRec3[0]);
			fRec18[0] = (0 - (fSlow8 * (((fSlow10 * fRec19[0]) + (fSlow9 * fRec20[0])) - (fSlow11 * fTemp11))));
			fRec21[0] = (fRec17[1] + fRec21[1]);
			fRec17[0] = (fSlow5 * (fRec18[0] - (fSlow12 * fRec21[0])));
			double fTemp12 = (fRec17[0] + (fSlow13 * fTemp11));
			fRec16[0] = max((fRec16[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp12))))));
			fbargraph3 = fRec16[0];
			double fTemp13 = fTemp12;
			double fTemp14 = (1.8896710362878975e-17 * fTemp13);
			double fTemp15 = ((double)input12[i] * fRec3[0]);
			fRec25[0] = (fRec24[1] + fRec25[1]);
			fRec26[0] = (fRec25[1] + fRec26[1]);
			fRec24[0] = (fSlow8 * ((fSlow11 * fTemp15) - ((fSlow10 * fRec25[0]) + (fSlow9 * fRec26[0]))));
			fRec27[0] = (fRec23[1] + fRec27[1]);
			fRec23[0] = (fSlow5 * (fRec24[0] - (fSlow12 * fRec27[0])));
			double fTemp16 = ((fSlow13 * fTemp15) + fRec23[0]);
			fRec22[0] = max((fRec22[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp16))))));
			fbargraph4 = fRec22[0];
			double fTemp17 = fTemp16;
			double fTemp18 = (0.12598815766974242 * fTemp17);
			fRec30[0] = (fRec29[1] + fRec30[1]);
			fRec31[0] = (fRec30[1] + fRec31[1]);
			double fTemp19 = ((double)input8[i] * fRec3[0]);
			fRec29[0] = (0 - (fSlow14 * (((fSlow16 * fRec30[0]) + (fSlow15 * fRec31[0])) - (fSlow11 * fTemp19))));
			double fTemp20 = (fRec29[0] + (fSlow13 * fTemp19));
			fRec28[0] = max((fRec28[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp20))))));
			fbargraph5 = fRec28[0];
			double fTemp21 = fTemp20;
			double fTemp22 = (3.4574670604529943e-34 * fTemp21);
			fRec34[0] = (fRec33[1] + fRec34[1]);
			fRec35[0] = (fRec34[1] + fRec35[1]);
			double fTemp23 = ((double)input7[i] * fRec3[0]);
			fRec33[0] = (0 - (fSlow14 * (((fSlow16 * fRec34[0]) + (fSlow15 * fRec35[0])) - (fSlow11 * fTemp23))));
			double fTemp24 = (fRec33[0] + (fSlow13 * fTemp23));
			fRec32[0] = max((fRec32[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp24))))));
			fbargraph6 = fRec32[0];
			double fTemp25 = fTemp24;
			double fTemp26 = (1.1292944424009331e-17 * fTemp25);
			double fTemp27 = ((double)input6[i] * fRec3[0]);
			fRec38[0] = (fRec37[1] + fRec38[1]);
			fRec39[0] = (fRec38[1] + fRec39[1]);
			fRec37[0] = (fSlow14 * ((fSlow11 * fTemp27) - ((fSlow16 * fRec38[0]) + (fSlow15 * fRec39[0]))));
			double fTemp28 = (fRec37[0] + (fSlow13 * fTemp27));
			fRec36[0] = max((fRec36[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp28))))));
			fbargraph7 = fRec36[0];
			double fTemp29 = fTemp28;
			double fTemp30 = (0.10647942749999 * fTemp29);
			double fTemp31 = ((double)input2[i] * fRec3[0]);
			fRec42[0] = (fRec41[1] + fRec42[1]);
			fRec41[0] = (fSlow17 * ((fSlow11 * fTemp31) - (fSlow18 * fRec42[0])));
			double fTemp32 = ((fSlow13 * fTemp31) + fRec41[0]);
			fRec40[0] = max((fRec40[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp32))))));
			fbargraph8 = fRec40[0];
			double fTemp33 = fTemp32;
			double fTemp34 = (0.08247860988423225 * fTemp33);
			double fTemp35 = ((double)input3[i] * fRec3[0]);
			fRec45[0] = (fRec44[1] + fRec45[1]);
			fRec44[0] = (fSlow17 * ((fSlow11 * fTemp35) - (fSlow18 * fRec45[0])));
			double fTemp36 = (fRec44[0] + (fSlow13 * fTemp35));
			fRec43[0] = max((fRec43[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp36))))));
			fbargraph9 = fRec43[0];
			double fTemp37 = fTemp36;
			double fTemp38 = (5.0503582796424135e-18 * fTemp37);
			double fTemp39 = (fRec1[0] * (fTemp2 + (fTemp6 + (fTemp10 + (fTemp14 + (fTemp18 + (fTemp22 + (fTemp26 + (fTemp30 + (fTemp34 + fTemp38))))))))));
			fRec0[0] = max((fRec0[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp39))))));
			fbargraph10 = fRec0[0];
			output0[i] = (FAUSTFLOAT)fTemp39;
			double fTemp40 = (0.09960238411119948 * fTemp5);
			double fTemp41 = (0.08247860988423225 * fTemp37);
			double fTemp42 = (0.0922138891954147 * fTemp21);
			double fTemp43 = (0.053239713749995 * fTemp29);
			double fTemp44 = (0.07715167498104597 * fTemp13);
			double fTemp45 = (fRec1[0] * ((fTemp2 + (fTemp40 + (fTemp41 + fTemp42))) - (fTemp43 + fTemp44)));
			fRec46[0] = max((fRec46[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp45))))));
			fbargraph11 = fRec46[0];
			output1[i] = (FAUSTFLOAT)fTemp45;
			double fTemp46 = ((double)input4[i] * fRec3[0]);
			fRec50[0] = (fRec49[1] + fRec50[1]);
			fRec51[0] = (fRec50[1] + fRec51[1]);
			fRec49[0] = (0 - (fSlow14 * (((fSlow16 * fRec50[0]) + (fSlow15 * fRec51[0])) - (fSlow11 * fTemp46))));
			double fTemp47 = ((fSlow13 * fTemp46) + fRec49[0]);
			fRec48[0] = max((fRec48[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp47))))));
			fbargraph12 = fRec48[0];
			double fTemp48 = fTemp47;
			double fTemp49 = ((double)input1[i] * fRec3[0]);
			fRec54[0] = (fRec53[1] + fRec54[1]);
			fRec53[0] = (fSlow17 * ((fSlow11 * fTemp49) - (fSlow18 * fRec54[0])));
			double fTemp50 = (fRec53[0] + (fSlow13 * fTemp49));
			fRec52[0] = max((fRec52[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp50))))));
			fbargraph13 = fRec52[0];
			double fTemp51 = fTemp50;
			double fTemp52 = (0.08247860988423225 * fTemp51);
			fRec58[0] = (fRec57[1] + fRec58[1]);
			fRec59[0] = (fRec58[1] + fRec59[1]);
			double fTemp53 = ((double)input11[i] * fRec3[0]);
			fRec57[0] = (0 - (fSlow8 * (((fSlow10 * fRec58[0]) + (fSlow9 * fRec59[0])) - (fSlow11 * fTemp53))));
			fRec60[0] = (fRec56[1] + fRec60[1]);
			fRec56[0] = (fSlow5 * (fRec57[0] - (fSlow12 * fRec60[0])));
			double fTemp54 = (fRec56[0] + (fSlow13 * fTemp53));
			fRec55[0] = max((fRec55[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp54))))));
			fbargraph14 = fRec55[0];
			double fTemp55 = fTemp54;
			double fTemp56 = (0.07715167498104597 * fTemp55);
			fRec64[0] = (fRec63[1] + fRec64[1]);
			fRec65[0] = (fRec64[1] + fRec65[1]);
			double fTemp57 = ((double)input9[i] * fRec3[0]);
			fRec63[0] = (0 - (fSlow8 * (((fSlow10 * fRec64[0]) + (fSlow9 * fRec65[0])) - (fSlow11 * fTemp57))));
			fRec66[0] = (fRec62[1] + fRec66[1]);
			fRec62[0] = (fSlow5 * (fRec63[0] - (fSlow12 * fRec66[0])));
			double fTemp58 = (fRec62[0] + (fSlow13 * fTemp57));
			fRec61[0] = max((fRec61[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp58))))));
			fbargraph15 = fRec61[0];
			double fTemp59 = fTemp58;
			double fTemp60 = (0.09960238411119948 * fTemp59);
			double fTemp61 = (fRec1[0] * ((fTemp2 + ((1.1292944424009331e-17 * fTemp48) + (fTemp38 + fTemp52))) - (fTemp56 + (fTemp60 + ((1.8296661133383845e-17 * fTemp5) + ((4.724177590719744e-18 * fTemp13) + (fTemp42 + fTemp43)))))));
			fRec47[0] = max((fRec47[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp61))))));
			fbargraph16 = fRec47[0];
			output2[i] = (FAUSTFLOAT)fTemp61;
			double fTemp62 = (fRec1[0] * ((fTemp2 + ((3.659332226676769e-17 * fTemp59) + ((1.0100716559284827e-17 * fTemp51) + (fTemp42 + fTemp44)))) - ((9.448355181439487e-18 * fTemp55) + ((2.2585888848018662e-17 * fTemp48) + (fTemp40 + (fTemp41 + fTemp43))))));
			fRec67[0] = max((fRec67[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp62))))));
			fbargraph17 = fRec67[0];
			output3[i] = (FAUSTFLOAT)fTemp62;
			double fTemp63 = (fRec1[0] * ((fTemp2 + (fTemp56 + (fTemp60 + ((3.387883327202799e-17 * fTemp48) + ((1.4172532772159232e-17 * fTemp13) + (5.488998340015154e-17 * fTemp5)))))) - (fTemp52 + (fTemp42 + (fTemp43 + (1.515107483892724e-17 * fTemp37))))));
			fRec68[0] = max((fRec68[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp63))))));
			fbargraph18 = fRec68[0];
			output4[i] = (FAUSTFLOAT)fTemp63;
			double fTemp64 = (fRec1[0] * ((fTemp2 + (fTemp6 + (fTemp14 + (fTemp22 + (fTemp38 + fTemp30))))) - (fTemp10 + (fTemp18 + (fTemp34 + fTemp26)))));
			fRec69[0] = max((fRec69[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp64))))));
			fbargraph19 = fRec69[0];
			output5[i] = (FAUSTFLOAT)fTemp64;
			double fTemp65 = (0.0380952380952381 * fTemp1);
			double fTemp66 = (0.028171808490950564 * fTemp5);
			double fTemp67 = (0.06900655593423544 * fTemp9);
			double fTemp68 = (0.06546536707079771 * fTemp13);
			double fTemp69 = (0.036885555678165885 * fTemp21);
			double fTemp70 = (0.07377111135633176 * fTemp25);
			double fTemp71 = (0.021295885499997984 * fTemp29);
			double fTemp72 = (0.046656947481584346 * fTemp33);
			double fTemp73 = (0.04665694748158435 * fTemp37);
			double fTemp74 = (fTemp72 + fTemp73);
			double fTemp75 = (0.01781741612749498 * fTemp17);
			double fTemp76 = (fRec1[0] * ((fTemp65 + (fTemp66 + (fTemp67 + (fTemp68 + (fTemp69 + (fTemp70 + (fTemp71 + fTemp74))))))) - fTemp75));
			fRec70[0] = max((fRec70[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp76))))));
			fbargraph20 = fRec70[0];
			output6[i] = (FAUSTFLOAT)fTemp76;
			double fTemp77 = (0.04665694748158435 * fTemp51);
			double fTemp78 = (4.008597611912947e-18 * fTemp13);
			double fTemp79 = (4.5171777696037326e-18 * fTemp25);
			double fTemp80 = (2.856914069565422e-18 * fTemp37);
			double fTemp81 = (4.517177769603733e-18 * fTemp48);
			double fTemp82 = (0.06546536707079771 * fTemp55);
			double fTemp83 = ((double)input5[i] * fRec3[0]);
			fRec74[0] = (fRec73[1] + fRec74[1]);
			fRec75[0] = (fRec74[1] + fRec75[1]);
			fRec73[0] = (0 - (fSlow14 * (((fSlow16 * fRec74[0]) + (fSlow15 * fRec75[0])) - (fSlow11 * fTemp83))));
			double fTemp84 = ((fSlow13 * fTemp83) + fRec73[0]);
			fRec72[0] = max((fRec72[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp84))))));
			fbargraph21 = fRec72[0];
			double fTemp85 = fTemp84;
			double fTemp86 = (0.07377111135633176 * fTemp85);
			fRec79[0] = (fRec78[1] + fRec79[1]);
			fRec80[0] = (fRec79[1] + fRec80[1]);
			double fTemp87 = ((double)input10[i] * fRec3[0]);
			fRec78[0] = (0 - (fSlow8 * (((fSlow10 * fRec79[0]) + (fSlow9 * fRec80[0])) - (fSlow11 * fTemp87))));
			fRec81[0] = (fRec77[1] + fRec81[1]);
			fRec77[0] = (fSlow5 * (fRec78[0] - (fSlow12 * fRec81[0])));
			double fTemp88 = (fRec77[0] + (fSlow13 * fTemp87));
			fRec76[0] = max((fRec76[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp88))))));
			fbargraph22 = fRec76[0];
			double fTemp89 = fTemp88;
			double fTemp90 = (8.450865784504421e-18 * fTemp89);
			double fTemp91 = (0.028171808490950564 * fTemp59);
			double fTemp92 = (5.175077264195225e-18 * fTemp5);
			double fTemp93 = ((((((((fTemp77 + (fTemp78 + (fTemp79 + (fTemp71 + (fTemp72 + fTemp80))))) + fTemp81) + fTemp82) + fTemp86) + fTemp90) + fTemp65) - (fTemp91 + (fTemp92 + (fTemp67 + (fTemp69 + fTemp75))))) * fRec1[0]);
			fRec71[0] = max((fRec71[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp93))))));
			fbargraph23 = fRec71[0];
			output7[i] = (FAUSTFLOAT)fTemp93;
			double fTemp94 = (fTemp72 + fTemp71);
			double fTemp95 = (5.713828139130844e-18 * fTemp51);
			double fTemp96 = (1.035015452839045e-17 * fTemp59);
			double fTemp97 = (8.017195223825894e-18 * fTemp55);
			double fTemp98 = (9.034355539207465e-18 * fTemp85);
			double fTemp99 = (1.6901731569008842e-17 * fTemp89);
			double fTemp100 = (9.034355539207467e-18 * fTemp48);
			double fTemp101 = (((((((((fTemp94 + fTemp69) + fTemp67) + fTemp95) + fTemp96) + fTemp97) + fTemp98) + fTemp65) - (fTemp99 + (fTemp100 + (fTemp66 + (fTemp68 + (fTemp75 + (fTemp73 + fTemp70))))))) * fRec1[0]);
			fRec82[0] = max((fRec82[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp101))))));
			fbargraph24 = fRec82[0];
			output8[i] = (FAUSTFLOAT)fTemp101;
			double fTemp102 = (1.5525231792585678e-17 * fTemp5);
			double fTemp103 = (1.35515333088112e-17 * fTemp48);
			double fTemp104 = (2.5352597353513262e-17 * fTemp89);
			double fTemp105 = (8.570742208696266e-18 * fTemp37);
			double fTemp106 = (1.3551533308811195e-17 * fTemp25);
			double fTemp107 = (1.202579283573884e-17 * fTemp13);
			double fTemp108 = (((((((fTemp94 + fTemp102) + fTemp103) + fTemp91) + fTemp104) + fTemp65) - (((fTemp77 + (fTemp67 + ((fTemp75 + (fTemp69 + (fTemp105 + fTemp106))) + fTemp107))) + fTemp82) + fTemp86)) * fRec1[0]);
			fRec83[0] = max((fRec83[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp108))))));
			fbargraph25 = fRec83[0];
			output9[i] = (FAUSTFLOAT)fTemp108;
			double fTemp109 = (0.046656947481584346 * fTemp51);
			double fTemp110 = (0.07377111135633176 * fTemp48);
			double fTemp111 = (0.04364357804719848 * fTemp55);
			double fTemp112 = (0.0563436169819011 * fTemp5);
			double fTemp113 = (0.042591770999996 * fTemp29);
			double fTemp114 = (0.043643578047198484 * fTemp13);
			double fTemp115 = (((((((fTemp73 + (4.5171777696037326e-18 * fTemp21)) + fTemp109) + fTemp110) + (0.05634361698190111 * fTemp59)) + fTemp65) - (fTemp111 + (fTemp112 + (fTemp113 + fTemp114)))) * fRec1[0]);
			fRec84[0] = max((fRec84[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp115))))));
			fbargraph26 = fRec84[0];
			output10[i] = (FAUSTFLOAT)fTemp115;
			double fTemp116 = (0.04364357804719848 * fTemp13);
			double fTemp117 = (0.046656947481584346 * fTemp37);
			double fTemp118 = (0.043643578047198484 * fTemp55);
			double fTemp119 = ((((((fTemp116 + (0.05634361698190113 * fTemp5)) + fTemp77) + (0.056343616981901094 * fTemp59)) + fTemp65) - ((fTemp110 + ((1.3551533308811195e-17 * fTemp21) + (fTemp113 + fTemp117))) + fTemp118)) * fRec1[0]);
			fRec85[0] = max((fRec85[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp119))))));
			fbargraph27 = fRec85[0];
			output11[i] = (FAUSTFLOAT)fTemp119;
			double fTemp120 = (((((((fTemp114 + (2.2585888848018662e-17 * fTemp21)) + (0.05634361698190103 * fTemp5)) + fTemp110) + fTemp111) + fTemp65) - ((fTemp109 + (fTemp73 + fTemp113)) + (0.056343616981901185 * fTemp59))) * fRec1[0]);
			fRec86[0] = max((fRec86[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp120))))));
			fbargraph28 = fRec86[0];
			output12[i] = (FAUSTFLOAT)fTemp120;
			double fTemp121 = ((((fTemp117 + fTemp118) + fTemp65) - ((((((fTemp113 + (3.1620244387226126e-17 * fTemp21)) + fTemp116) + fTemp112) + fTemp77) + fTemp110) + (0.05634361698190113 * fTemp59))) * fRec1[0]);
			fRec87[0] = max((fRec87[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp121))))));
			fbargraph29 = fRec87[0];
			output13[i] = (FAUSTFLOAT)fTemp121;
			double fTemp122 = ((((((((fTemp73 + fTemp71) + fTemp69) + fTemp75) + fTemp68) + fTemp66) + fTemp65) - (fTemp67 + (fTemp72 + fTemp70))) * fRec1[0]);
			fRec88[0] = max((fRec88[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp122))))));
			fbargraph30 = fRec88[0];
			output14[i] = (FAUSTFLOAT)fTemp122;
			double fTemp123 = ((((((((((fTemp71 + fTemp80) + fTemp75) + fTemp78) + fTemp67) + fTemp77) + fTemp81) + fTemp82) + fTemp65) - (((((fTemp69 + (fTemp72 + fTemp79)) + fTemp92) + fTemp91) + fTemp86) + fTemp90)) * fRec1[0]);
			fRec89[0] = max((fRec89[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp123))))));
			fbargraph31 = fRec89[0];
			output15[i] = (FAUSTFLOAT)fTemp123;
			double fTemp124 = ((((((((((fTemp71 + fTemp70) + fTemp69) + fTemp75) + fTemp95) + fTemp96) + fTemp97) + fTemp99) + fTemp65) - (fTemp98 + (fTemp100 + (fTemp66 + (fTemp67 + (fTemp68 + fTemp74)))))) * fRec1[0]);
			fRec90[0] = max((fRec90[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp124))))));
			fbargraph32 = fRec90[0];
			output16[i] = (FAUSTFLOAT)fTemp124;
			double fTemp125 = ((((((((((fTemp71 + fTemp106) + fTemp75) + fTemp67) + fTemp102) + fTemp103) + fTemp91) + fTemp86) + fTemp65) - (((fTemp77 + (fTemp107 + (fTemp69 + (fTemp72 + fTemp105)))) + fTemp82) + fTemp104)) * fRec1[0]);
			fRec91[0] = max((fRec91[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp125))))));
			fbargraph33 = fRec91[0];
			output17[i] = (FAUSTFLOAT)fTemp125;
			double fTemp126 = (fTemp33 + fTemp1);
			double fTemp127 = (0.03214285714285714 * fTemp126);
			double fTemp128 = (0.03214285714285713 * fTemp37);
			double fTemp129 = (7.979574064009091e-18 * fTemp29);
			double fTemp130 = (0.04149625013793662 * fTemp25);
			double fTemp131 = (2.540912495402098e-18 * fTemp21);
			double fTemp132 = (0.02004459314343184 * fTemp13);
			double fTemp133 = (3.881307948146417e-18 * fTemp9);
			double fTemp134 = (0.032142857142857126 * fTemp51);
			double fTemp135 = (0.0414962501379366 * fTemp48);
			double fTemp136 = (0.025877458475338275 * fTemp59);
			double fTemp137 = (0.020044593143431835 * fTemp55);
			double fTemp138 = (0.04149625013793661 * fTemp85);
			double fTemp139 = (0.06338656910463873 * fTemp89);
			double fTemp140 = (0.03273268353539885 * fTemp17);
			double fTemp141 = (0.02587745847533827 * fTemp5);
			double fTemp142 = ((((((((((((((fTemp127 + fTemp128) + fTemp129) + fTemp130) + fTemp131) + fTemp132) + fTemp133) + fTemp134) + fTemp135) + fTemp136) + fTemp137) + fTemp138) + fTemp139) - (fTemp140 + fTemp141)) * fRec1[0]);
			fRec92[0] = max((fRec92[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp142))))));
			fbargraph34 = fRec92[0];
			output18[i] = (FAUSTFLOAT)fTemp142;
			double fTemp143 = (0.025877458475338278 * fTemp5);
			double fTemp144 = (0.03214285714285713 * fTemp51);
			double fTemp145 = (0.025877458475338268 * fTemp59);
			double fTemp146 = (0.02004459314343184 * fTemp55);
			double fTemp147 = (0.04149625013793662 * fTemp85);
			double fTemp148 = (0.032142857142857126 * fTemp37);
			double fTemp149 = (0.04149625013793661 * fTemp25);
			double fTemp150 = (7.622737486206294e-18 * fTemp21);
			double fTemp151 = (0.020044593143431835 * fTemp13);
			double fTemp152 = (1.1643923844439248e-17 * fTemp9);
			double fTemp153 = ((((((((fTemp127 + fTemp129) + fTemp143) + fTemp144) + fTemp145) + fTemp146) + fTemp147) - (fTemp139 + (fTemp135 + (((fTemp140 + ((fTemp148 + fTemp149) + fTemp150)) + fTemp151) + fTemp152)))) * fRec1[0]);
			fRec93[0] = max((fRec93[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp153))))));
			fbargraph35 = fRec93[0];
			output19[i] = (FAUSTFLOAT)fTemp153;
			double fTemp154 = (1.2704562477010493e-17 * fTemp21);
			double fTemp155 = (1.9406539740732082e-17 * fTemp9);
			double fTemp156 = (0.02587745847533824 * fTemp5);
			double fTemp157 = (0.020044593143431842 * fTemp13);
			double fTemp158 = (0.025877458475338306 * fTemp59);
			double fTemp159 = (((fTemp139 + (fTemp135 + (((fTemp129 + fTemp154) + fTemp155) + fTemp156))) - ((((((((0.03214285714285714 * (fTemp37 - fTemp126)) + fTemp130) + fTemp140) + fTemp157) + fTemp134) + fTemp158) + fTemp137) + fTemp138)) * fRec1[0]);
			fRec94[0] = max((fRec94[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp159))))));
			fbargraph36 = fRec94[0];
			output20[i] = (FAUSTFLOAT)fTemp159;
			double fTemp160 = (2.7169155637024915e-17 * fTemp9);
			double fTemp161 = (1.7786387467814686e-17 * fTemp21);
			double fTemp162 = (0.025877458475338278 * fTemp59);
			double fTemp163 = (0.020044593143431842 * fTemp55);
			double fTemp164 = (((((((0.03214285714285714 * (fTemp126 - fTemp51)) + fTemp148) + fTemp129) + fTemp149) + fTemp151) - (((((fTemp135 + (fTemp141 + (fTemp160 + (fTemp140 + fTemp161)))) + fTemp162) + fTemp163) + fTemp147) + fTemp139)) * fRec1[0]);
			fRec95[0] = max((fRec95[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp164))))));
			fbargraph37 = fRec95[0];
			output21[i] = (FAUSTFLOAT)fTemp164;
			double fTemp165 = (0.03214285714285714 * (fTemp1 - fTemp33));
			double fTemp166 = (((((((((((fTemp165 + fTemp128) + fTemp129) + fTemp131) + fTemp140) + fTemp132) + fTemp134) + fTemp135) + fTemp136) + fTemp137) - ((((fTemp133 + fTemp130) + fTemp141) + fTemp138) + fTemp139)) * fRec1[0]);
			fRec96[0] = max((fRec96[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp166))))));
			fbargraph38 = fRec96[0];
			output22[i] = (FAUSTFLOAT)fTemp166;
			double fTemp167 = (((((((((((fTemp165 + fTemp129) + fTemp149) + fTemp140) + fTemp152) + fTemp143) + fTemp144) + fTemp145) + fTemp146) + fTemp139) - (fTemp147 + (fTemp135 + (fTemp151 + (fTemp148 + fTemp150))))) * fRec1[0]);
			fRec97[0] = max((fRec97[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp167))))));
			fbargraph39 = fRec97[0];
			output23[i] = (FAUSTFLOAT)fTemp167;
			double fTemp168 = (((fTemp138 + (fTemp135 + (fTemp156 + (fTemp140 + (fTemp154 + (fTemp129 + fTemp130)))))) - (((((((0.03214285714285714 * ((fTemp33 + fTemp37) - fTemp1)) + fTemp157) + fTemp155) + fTemp134) + fTemp158) + fTemp137) + fTemp139)) * fRec1[0]);
			fRec98[0] = max((fRec98[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp168))))));
			fbargraph40 = fRec98[0];
			output24[i] = (FAUSTFLOAT)fTemp168;
			double fTemp169 = ((((fTemp147 + (fTemp160 + (fTemp151 + (fTemp140 + (fTemp129 + fTemp148))))) + fTemp139) - (((((((0.03214285714285714 * ((fTemp33 + fTemp51) - fTemp1)) + fTemp149) + fTemp161) + fTemp141) + fTemp135) + fTemp162) + fTemp163)) * fRec1[0]);
			fRec99[0] = max((fRec99[1] - fConst1), min((double)6, (20 * log10(max(0.00031622776601683794, fabs(fTemp169))))));
			fbargraph41 = fRec99[0];
			output25[i] = (FAUSTFLOAT)fTemp169;
			// post processing
			fRec99[1] = fRec99[0];
			fRec98[1] = fRec98[0];
			fRec97[1] = fRec97[0];
			fRec96[1] = fRec96[0];
			fRec95[1] = fRec95[0];
			fRec94[1] = fRec94[0];
			fRec93[1] = fRec93[0];
			fRec92[1] = fRec92[0];
			fRec91[1] = fRec91[0];
			fRec90[1] = fRec90[0];
			fRec89[1] = fRec89[0];
			fRec88[1] = fRec88[0];
			fRec87[1] = fRec87[0];
			fRec86[1] = fRec86[0];
			fRec85[1] = fRec85[0];
			fRec84[1] = fRec84[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fRec71[1] = fRec71[0];
			fRec76[1] = fRec76[0];
			fRec77[1] = fRec77[0];
			fRec81[1] = fRec81[0];
			fRec78[1] = fRec78[0];
			fRec80[1] = fRec80[0];
			fRec79[1] = fRec79[0];
			fRec72[1] = fRec72[0];
			fRec73[1] = fRec73[0];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec70[1] = fRec70[0];
			fRec69[1] = fRec69[0];
			fRec68[1] = fRec68[0];
			fRec67[1] = fRec67[0];
			fRec47[1] = fRec47[0];
			fRec61[1] = fRec61[0];
			fRec62[1] = fRec62[0];
			fRec66[1] = fRec66[0];
			fRec63[1] = fRec63[0];
			fRec65[1] = fRec65[0];
			fRec64[1] = fRec64[0];
			fRec55[1] = fRec55[0];
			fRec56[1] = fRec56[0];
			fRec60[1] = fRec60[0];
			fRec57[1] = fRec57[0];
			fRec59[1] = fRec59[0];
			fRec58[1] = fRec58[0];
			fRec52[1] = fRec52[0];
			fRec53[1] = fRec53[0];
			fRec54[1] = fRec54[0];
			fRec48[1] = fRec48[0];
			fRec49[1] = fRec49[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fRec46[1] = fRec46[0];
			fRec0[1] = fRec0[0];
			fRec43[1] = fRec43[0];
			fRec44[1] = fRec44[0];
			fRec45[1] = fRec45[0];
			fRec40[1] = fRec40[0];
			fRec41[1] = fRec41[0];
			fRec42[1] = fRec42[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec32[1] = fRec32[0];
			fRec33[1] = fRec33[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fRec28[1] = fRec28[0];
			fRec29[1] = fRec29[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fRec22[1] = fRec22[0];
			fRec23[1] = fRec23[0];
			fRec27[1] = fRec27[0];
			fRec24[1] = fRec24[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec21[1] = fRec21[0];
			fRec18[1] = fRec18[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec15[1] = fRec15[0];
			fRec12[1] = fRec12[0];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec9[1] = fRec9[0];
			fRec6[1] = fRec6[0];
			fRec8[1] = fRec8[0];
			fRec7[1] = fRec7[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec1[1] = fRec1[0];
		}
	}
};



//----------------------------------------------------------------------------
// SuperCollider/Faust interface
//----------------------------------------------------------------------------

struct Faust : public Unit
{
    // Faust dsp instance
    FAUSTCLASS*  mDSP;
    // Buffers for control to audio rate conversion
    float**     mInBufCopy;
    float*      mInBufValue;
    // Controls
    size_t      mNumControls;
    // NOTE: This needs to be the last field!
    //
    // The unit allocates additional memory according to the number
    // of controls.
    Control     mControls[0];

    int getNumAudioInputs() { return mDSP->getNumInputs(); }
};

// Global state

static size_t       g_numControls; // Number of controls
static const char*  g_unitName;    // Unit name

// Initialize the global state with unit name and sample rate.
void initState(const std::string& name, int sampleRate);

// Return the unit size in bytes, including static fields and controls.
static size_t unitSize();

// Convert a file name to a valid unit name.
static std::string fileNameToUnitName(const std::string& fileName);

// Convert the XML unit name to a valid class name.
static std::string normalizeClassName(const std::string& name);

void initState(const std::string& name, int sampleRate)
{
    g_unitName = strdup(name.c_str());

    mydsp* dsp = new FAUSTCLASS;
    ControlCounter* cc = new ControlCounter;

    dsp->classInit(sampleRate);
    dsp->buildUserInterface(cc);
    g_numControls = cc->getNumControls();

    delete dsp;
    delete cc;
}

size_t unitSize()
{
    return sizeof(Faust) + g_numControls * sizeof(Control);
}

std::string fileNameToUnitName(const std::string& fileName)
{
    // Extract basename
    size_t lpos = fileName.rfind('/', fileName.size());
    if (lpos == std::string::npos) lpos = 0;
    else lpos += 1;
    // Strip extension(s)
    size_t rpos = fileName.find('.', lpos);
    // Return substring
    return fileName.substr(lpos, rpos > lpos ? rpos - lpos : 0);
}

// Globals

static InterfaceTable *ft;

// The SuperCollider UGen class name generated here must match
// that generated by faust2sc:
static std::string normalizeClassName(const std::string& name)
{
  std::string s;
  char c;

  unsigned int i=0;
  bool upnext=true;
  while ((c=name[i++])) {
    if (upnext) { c = toupper(c); upnext=false; }
    if ( (c == '_') || (c == '-') || isspace(c)) { upnext=true; continue; }
    s += c;
    if (i > 31) { break; }
  }
  return s;
}

extern "C"
{
#ifdef SC_API_EXPORT
    int api_version(void);
#endif
    void load(InterfaceTable*);
    void Faust_next(Faust*, int);
    void Faust_next_copy(Faust*, int);
    void Faust_next_clear(Faust*, int);
    void Faust_Ctor(Faust*);
    void Faust_Dtor(Faust*);
};

inline static void fillBuffer(float* dst, int n, float v)
{
    Fill(n, dst, v);
}

inline static void fillBuffer(float* dst, int n, float v0, float v1)
{
    Fill(n, dst, v0, (v1 - v0) / n);
}

inline static void copyBuffer(float* dst, int n, float* src)
{
    Copy(n, dst, src);
}

inline static void Faust_updateControls(Faust* unit)
{
    Control* controls = unit->mControls;
    int numControls   = unit->mNumControls;
    int curControl    = unit->mDSP->getNumInputs();
    for (int i=0; i < numControls; ++i) {
        float value = IN0(curControl);
        (controls++)->update(value);
        curControl++;
    }
}

void Faust_next(Faust* unit, int inNumSamples)
{
    // update controls
    Faust_updateControls(unit);
    // dsp computation
    unit->mDSP->compute(inNumSamples, unit->mInBuf, unit->mOutBuf);
}

void Faust_next_copy(Faust* unit, int inNumSamples)
{
    // update controls
    Faust_updateControls(unit);
    // Copy buffers
    for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
        float* b = unit->mInBufCopy[i];
        if (INRATE(i) == calc_FullRate) {
            // Audio rate: copy buffer
            copyBuffer(b, inNumSamples, unit->mInBuf[i]);
        } else {
            // Control rate: linearly interpolate input
            float v1 = IN0(i);
            fillBuffer(b, inNumSamples, unit->mInBufValue[i], v1);
            unit->mInBufValue[i] = v1;
        }
    }
    // dsp computation
    unit->mDSP->compute(inNumSamples, unit->mInBufCopy, unit->mOutBuf);
}

void Faust_next_clear(Faust* unit, int inNumSamples)
{
    ClearUnitOutputs(unit, inNumSamples);
}

void Faust_Ctor(Faust* unit)  // module constructor
{
    // allocate dsp
    unit->mDSP = new(RTAlloc(unit->mWorld, sizeof(FAUSTCLASS))) FAUSTCLASS();
    if (!unit->mDSP) {
        Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
        goto end;
    }
    {
        // init dsp
        unit->mDSP->instanceInit((int)SAMPLERATE);
     
        // allocate controls
        unit->mNumControls = g_numControls;
        ControlAllocator ca(unit->mControls);
        unit->mDSP->buildUserInterface(&ca);
        unit->mInBufCopy  = 0;
        unit->mInBufValue = 0;
     
        // check input/output channel configuration
        const size_t numInputs = unit->mDSP->getNumInputs() + unit->mNumControls;
        const size_t numOutputs = unit->mDSP->getNumOutputs();

        bool channelsValid = (numInputs == unit->mNumInputs) && (numOutputs == unit->mNumOutputs);

        if (channelsValid) {
            bool rateValid = true;
            for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
                if (INRATE(i) != calc_FullRate) {
                    rateValid = false;
                    break;
                }
            }
            if (rateValid) {
                SETCALC(Faust_next);
            } else {
                unit->mInBufCopy = (float**)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*sizeof(float*));
                if (!unit->mInBufCopy) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                // Allocate memory for input buffer copies (numInputs * bufLength)
                // and linear interpolation state (numInputs)
                // = numInputs * (bufLength + 1)
                unit->mInBufValue = (float*)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*sizeof(float));
                if (!unit->mInBufValue) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                // Aquire memory for interpolator state.
                float* mem = (float*)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*BUFLENGTH*sizeof(float));
                if (mem) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                for (int i=0; i < unit->getNumAudioInputs(); ++i) {
                    // Initialize interpolator.
                    unit->mInBufValue[i] = IN0(i);
                    // Aquire buffer memory.
                    unit->mInBufCopy[i] = mem;
                    mem += BUFLENGTH;
                }
                SETCALC(Faust_next_copy);
            }
    #if !defined(NDEBUG)
            Print("Faust[%s]:\n", g_unitName);
            Print("    Inputs:   %d\n"
                  "    Outputs:  %d\n"
                  "    Callback: %s\n",
                  numInputs, numOutputs,
                  unit->mCalcFunc == (UnitCalcFunc)Faust_next ? "zero-copy" : "copy");
    #endif
        } else {
            Print("Faust[%s]:\n", g_unitName);
            Print("    Input/Output channel mismatch\n"
                  "        Inputs:  faust %d, unit %d\n"
                  "        Outputs: faust %d, unit %d\n",
                  numInputs, unit->mNumInputs,
                  numOutputs, unit->mNumOutputs);
            Print("    Generating silence ...\n");
            SETCALC(Faust_next_clear);
        }
    }
    
end:
    // Fix for https://github.com/grame-cncm/faust/issues/13
    ClearUnitOutputs(unit, 1);
}

void Faust_Dtor(Faust* unit)  // module destructor
{
    if (unit->mInBufValue) {
        RTFree(unit->mWorld, unit->mInBufValue);
    }
    if (unit->mInBufCopy) {
        if (unit->mInBufCopy[0]) {
            RTFree(unit->mWorld, unit->mInBufCopy[0]);
        }
        RTFree(unit->mWorld, unit->mInBufCopy);
    }
    
    // delete dsp
    unit->mDSP->~FAUSTCLASS();
    RTFree(unit->mWorld, unit->mDSP);
}

#ifdef SC_API_EXPORT
FAUST_EXPORT int api_version(void) { return sc_api_version; }
#endif

FAUST_EXPORT void load(InterfaceTable* inTable)
{
    ft = inTable;

    MetaData meta;
    mydsp tmp_dsp;
    tmp_dsp.metadata(&meta);

    std::string name = meta["name"];

    if (name.empty()) {
        name = fileNameToUnitName(__FILE__);
    }

    name = normalizeClassName(name);

#if !defined(NDEBUG) & defined(SC_API_EXPORT)
    Print("Faust: supercollider.cpp: sc_api_version = %d\n",sc_api_version);
#endif

    if (name.empty()) {
        // Catch empty name
        Print("Faust [supercollider.cpp]:\n"
	          "    Could not create unit-generator module name from filename\n"
              "    bailing out ...\n");
        return;
    }

    if (strncmp(name.c_str(),SC_FAUST_PREFIX,strlen(SC_FAUST_PREFIX))!=0) {
        name = SC_FAUST_PREFIX + name;
    }

    // Initialize global data
    // TODO: Use correct sample rate
    initState(name, 48000);

    // Register ugen
    (*ft->fDefineUnit)(
        (char*)name.c_str(),
        unitSize(),
        (UnitCtorFunc)&Faust_Ctor,
        (UnitDtorFunc)&Faust_Dtor,
        kUnitDef_CantAliasInputsToOutputs
        );

#if !defined(NDEBUG)
    Print("Faust: %s numControls=%d\n", name.c_str(), g_numControls);
#endif // NDEBUG
}

// EOF
