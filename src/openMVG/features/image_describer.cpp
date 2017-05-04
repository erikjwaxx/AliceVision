
// Copyright (c) 2015 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "image_describer.hpp"

#include "openMVG/features/image_describer_akaze.hpp"
#include "openMVG/features/cctag/CCTAG_describer.hpp"
#include "openMVG/features/openCV/AKAZE_openCV_describer.hpp"
#ifdef USE_OCVSIFT
#include "openMVG/features/openCV/SIFT_openCV_describer.hpp"
#endif

#include "nonFree/sift/SIFT_describer.hpp"
#include "nonFree/sift/SIFT_float_describer.hpp"

#include <exception>

namespace openMVG{
namespace features{

EDESCRIBER_PRESET describerPreset_stringToEnum(const std::string& sPreset)
{
  if(sPreset == "LOW")
    return LOW_PRESET;
  if (sPreset == "MEDIUM")
    return MEDIUM_PRESET;
  if(sPreset == "NORMAL")
    return NORMAL_PRESET;
  if (sPreset == "HIGH")
    return HIGH_PRESET;
  if (sPreset == "ULTRA")
    return ULTRA_PRESET;
  throw std::invalid_argument("Invalid descriptor preset: " + sPreset);
}

std::string describerPreset_enumToString(const EDESCRIBER_PRESET preset)
{
  if(preset == LOW_PRESET)
    return "LOW";
  if (preset == MEDIUM_PRESET)
    return "MEDIUM";
  if(preset == NORMAL_PRESET)
    return "NORMAL";
  if (preset == HIGH_PRESET)
    return "HIGH";
  if (preset == ULTRA_PRESET)
    return "ULTRA";
  throw std::invalid_argument("Unrecognized EDESCRIBER_PRESET "+std::to_string(preset));
}

std::ostream& operator<<(std::ostream& os, EDESCRIBER_PRESET p)
{
    return os << describerPreset_enumToString(p);
}

std::istream& operator>>(std::istream& in, EDESCRIBER_PRESET& p)
{
    std::string token;
    in >> token;
    p = describerPreset_stringToEnum(token);
    return in;
}

std::unique_ptr<Image_describer> createImageDescriber(EImageDescriberType imageDescriberType)
{
  std::unique_ptr<Image_describer> describerPtr;
  
  switch(imageDescriberType)
  {
    case EImageDescriberType::SIFT:        describerPtr.reset(new SIFT_Image_describer(SiftParams())); break;
    case EImageDescriberType::SIFT_FLOAT:  describerPtr.reset(new SIFT_float_describer(SiftParams())); break;
    case EImageDescriberType::AKAZE:       describerPtr.reset(new AKAZE_Image_describer(AKAZEParams(AKAZEConfig(), features::AKAZE_MSURF))); break;
    case EImageDescriberType::AKAZE_MLDB:  describerPtr.reset(new AKAZE_Image_describer(AKAZEParams(AKAZEConfig(), features::AKAZE_MLDB))); break;
    
#ifdef HAVE_CCTAG
    case EImageDescriberType::CCTAG3:      describerPtr.reset(new CCTAG_Image_describer(3)); break;
    case EImageDescriberType::CCTAG4:      describerPtr.reset(new CCTAG_Image_describer(4)); break;
#endif //HAVE_CCTAG

#ifdef HAVE_OPENCV
#ifdef USE_OCVSIFT
  case EImageDescriberType::SIFT_OCV:      describerPtr.reset(new SIFT_openCV_ImageDescriber()); break;
#endif
  case EImageDescriberType::AKAZE_OCV:     describerPtr.reset(new AKAZE_openCV_ImageDescriber()); break;
#endif //HAVE_OPENCV
    
    default: throw std::out_of_range("Invalid imageDescriber enum");
  }       
  assert(describerPtr != nullptr);
  
  return describerPtr;
}

}//namespace features
}//namespace openMVG
