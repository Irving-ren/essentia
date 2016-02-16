/*
 * Copyright (C) 2006-2015  Music Technology Group - Universitat Pompeu Fabra
 *
 * This file is part of Essentia
 *
 * Essentia is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 */

#ifndef ESSENTIA_HPRMODELANAL_H
#define ESSENTIA_HPRMODELANAL_H

#include "algorithm.h"
#include "algorithmfactory.h"
#include <fstream>


namespace essentia {
namespace standard {

class HprModelAnal : public Algorithm {

 protected:

  Input<std::vector<Real> > _frame;
  Output<std::vector<Real> > _magnitudes;
  Output<std::vector<Real> > _frequencies;
  Output<std::vector<Real> > _phases;
  Output<std::vector<Real> >_res;

//  int _stocSize;
/*  Algorithm* _window;
  Algorithm* _fft;*/
  Algorithm* _harmonicModelAnal;
  Algorithm* _sineSubtraction;



 public:
  HprModelAnal() {
    declareInput(_frame, "frame", "the input frame");
    declareOutput(_frequencies, "frequencies", "the frequencies of the sinusoidal peaks [Hz]");
    declareOutput(_magnitudes, "magnitudes", "the magnitudes of the sinusoidal peaks");
    declareOutput(_phases, "phases", "the phases of the sinusoidal peaks");
    declareOutput(_res, "res", "output residual frame");

/*    _window = AlgorithmFactory::create("Windowing");
    _fft = AlgorithmFactory::create("FFT");*/
    _harmonicModelAnal = AlgorithmFactory::create("HarmonicModelAnal");
    _sineSubtraction = AlgorithmFactory::create("SineSubtraction");
  

  }

  ~HprModelAnal() {

/*  delete _window;
  delete _fft;*/
  delete _harmonicModelAnal;
  delete _sineSubtraction;

  }

  void declareParameters() {
    declareParameter("sampleRate", "the sampling rate of the audio signal [Hz]", "(0,inf)", 44100.);
    declareParameter("hopSize", "the hop size between frames", "[1,inf)", 512);
    declareParameter("fftSize", "the size of the internal FFT size (full spectrum size)", "[1,inf)", 2048);
    declareParameter("maxPeaks", "the maximum number of returned peaks", "[1,inf)", 100);
    declareParameter("maxFrequency", "the maximum frequency of the range to evaluate [Hz]", "(0,inf)", 5000.0);
    declareParameter("minFrequency", "the minimum frequency of the range to evaluate [Hz]", "[0,inf)", 0.0);
    declareParameter("magnitudeThreshold", "peaks below this given threshold are not outputted", "(-inf,inf)", 0.0);
    declareParameter("orderBy", "the ordering type of the outputted peaks (ascending by frequency or descending by magnitude)", "{frequency,magnitude}", "frequency");
    // sinusoidal tracking
    declareParameter("maxnSines", "maximum number of sines per frame", "(0,inf)", 100);
    declareParameter("freqDevOffset", "minimum frequency deviation at 0Hz", "(0,inf)", 20);
    declareParameter("freqDevSlope", "slope increase of minimum frequency deviation", "(-inf,inf)", 0.01);
    declareParameter("stocf", "decimation factor used for the stochastic approximation", "(0,1]", 0.2);
    // harmonic parameters
    declareParameter("nHarmonics", "maximum number of harmonics per frame", "(0,inf)", 100);
    declareParameter("harmDevSlope", "slope increase of minimum frequency deviation", "(-inf,inf)", 0.01);
 
  }

  void configure();
  void compute();

  void updateStocInFrame(const std::vector<Real> frameIn, std::vector<Real> &frameAccumulator);

  static const char* name;
  static const char* description;



 private:

};

} // namespace standard
} // namespace essentia

#include "streamingalgorithmwrapper.h"

namespace essentia {
namespace streaming {

class HprModelAnal : public StreamingAlgorithmWrapper {

 protected:
  
  Sink<std::vector<Real> > _frame; // input
  Source<std::vector<Real> > _frequencies;
  Source<std::vector<Real> > _magnitudes;
  Source<std::vector<Real> > _phases;
  Source<std::vector<Real> > _res;

 public:
  HprModelAnal() {
    declareAlgorithm("HprModelAnal");
    declareInput(_frame, TOKEN, "frame");
    declareOutput(_frequencies, TOKEN, "frequencies");
    declareOutput(_magnitudes, TOKEN, "magnitudes");
    declareOutput(_phases, TOKEN, "phases");
    declareOutput(_res, TOKEN, "res");
  }
};

} // namespace streaming
} // namespace essentia




#endif // ESSENTIA_HPRMODELANAL_H
