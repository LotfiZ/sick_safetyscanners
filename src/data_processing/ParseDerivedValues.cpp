// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------

/*!
*  Copyright (C) 2018, SICK AG, Waldkirch
*  Copyright (C) 2018, FZI Forschungszentrum Informatik, Karlsruhe, Germany
*
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.

*/

// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!
 * \file ParseDerivedValues.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include <sick_safetyscanners/data_processing/ParseDerivedValues.h>

namespace sick {
namespace data_processing {

ParseDerivedValues::ParseDerivedValues()
{
  m_reader_ptr = std::make_shared<sick::data_processing::ReadWriteHelper>();
}

datastructure::DerivedValues
ParseDerivedValues::parseUDPSequence(const datastructure::PacketBuffer& buffer,
                                     datastructure::Data& data) const
{
  datastructure::DerivedValues derived_values;


  if (!checkIfPreconditionsAreMet(data))
  {
    derived_values.setIsEmpty(true);
    return derived_values;
  }
  const uint8_t* data_ptr(buffer.getBuffer().data() +
                          data.getDataHeaderPtr()->getDerivedValuesBlockOffset());
  setDataInDerivedValues(data_ptr, derived_values);
  return derived_values;
}

bool ParseDerivedValues::checkIfPreconditionsAreMet(const datastructure::Data& data) const
{
  if (!checkIfDerivedValuesIsPublished(data))
  {
    return false;
  }
  if (!checkIfDataContainsNeededParsedBlocks(data))
  {
    return false;
  }

  return true;
}

bool ParseDerivedValues::checkIfDerivedValuesIsPublished(const datastructure::Data& data) const
{
  if (data.getDataHeaderPtr()->getDerivedValuesBlockOffset() == 0 &&
      data.getDataHeaderPtr()->getDerivedValuesBlockSize() == 0)
  {
    return false;
  }
  return true;
}

bool ParseDerivedValues::checkIfDataContainsNeededParsedBlocks(
  const datastructure::Data& data) const
{
  if (data.getDataHeaderPtr()->isEmpty())
  {
    return false;
  }
  return true;
}

void ParseDerivedValues::setDataInDerivedValues(const uint8_t*& data_ptr,
                                                datastructure::DerivedValues& derived_values) const
{
  setMultiplicationFactorInDerivedValues(data_ptr, derived_values);
  setNumberOfBeamsInDerivedValues(data_ptr, derived_values);
  setScanTimeInDerivedValues(data_ptr, derived_values);
  setStartAngleInDerivedValues(data_ptr, derived_values);
  setAngularBeamResolutionInDerivedValues(data_ptr, derived_values);
  setInterbeamPeriodInDerivedValues(data_ptr, derived_values);
}

void ParseDerivedValues::setMultiplicationFactorInDerivedValues(
  const uint8_t*& data_ptr, datastructure::DerivedValues& derived_values) const
{
  derived_values.setMultiplicationFactor(m_reader_ptr->readuint16_tLittleEndian(data_ptr, 0));
}

void ParseDerivedValues::setNumberOfBeamsInDerivedValues(
  const uint8_t*& data_ptr, datastructure::DerivedValues& derived_values) const
{
  derived_values.setNumberOfBeams(m_reader_ptr->readuint16_tLittleEndian(data_ptr, 2));
}

void ParseDerivedValues::setScanTimeInDerivedValues(
  const uint8_t*& data_ptr, datastructure::DerivedValues& derived_values) const
{
  derived_values.setScanTime(m_reader_ptr->readuint16_tLittleEndian(data_ptr, 4));
}

void ParseDerivedValues::setStartAngleInDerivedValues(
  const uint8_t*& data_ptr, datastructure::DerivedValues& derived_values) const
{
  derived_values.setStartAngle(m_reader_ptr->readint32_tLittleEndian(data_ptr, 8));
}

void ParseDerivedValues::setAngularBeamResolutionInDerivedValues(
  const uint8_t*& data_ptr, datastructure::DerivedValues& derived_values) const
{
  derived_values.setAngularBeamResolution(m_reader_ptr->readint32_tLittleEndian(data_ptr, 12));
}

void ParseDerivedValues::setInterbeamPeriodInDerivedValues(
  const uint8_t*& data_ptr, datastructure::DerivedValues& derived_values) const
{
  derived_values.setInterbeamPeriod(m_reader_ptr->readuint32_tLittleEndian(data_ptr, 16));
}

} // namespace data_processing
} // namespace sick
