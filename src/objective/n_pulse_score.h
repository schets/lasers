/*
Copyright (c) 2014, Sam Schetterer, Nathan Kutz, University of Washington
Authors: Sam Schetterer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#ifndef CNLS_PULSE_H
#define CNLS_PULSE_H
#include "objective.h"
//!This class calculates the score of a solution with n pulses, tailored towards the quality of a laser pulse
/*!
 * This class calculates the quality of a laser pulse formed by n distinct pulses. First,
 * it calculates the magnitude of the pulse which results from the base pulses, and then finds
 * the quality of that resulting pulse. This is done by finding the energy divided by the kurtosis.
 * of the Fourier transform. This metric balances high energy with a tight pulse.
 */
class n_pulse_score:public objective{
    size_t n_pulse;
    double* restr help;
    size_t nts;
    comp* restr kurtosis_help;
    public:
    virtual std::string type() const;
    virtual double score(comp* invals);
    virtual std::vector<std::string> dependencies() const;
    virtual void postprocess(input& inval);
    ~n_pulse_score();
};
#endif
