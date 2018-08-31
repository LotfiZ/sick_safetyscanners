#include <sick_microscan3_ros_driver/data_processing/ParseData.h>

namespace sick {
namespace data_processing {

bool ParseData::parseUDPSequence(datastructure::PacketBuffer buffer, datastructure::Data &data)
{
  std::cout << "Beginn Parsing Data Header" << std::endl;

  const BYTE* dataPtr(buffer.getBuffer().data());

  sick::datastructure::DataHeader data_header = ParseDataHeader::parseUDPSequence(buffer, data);
  data.setDataHeaderPtr(boost::make_shared<sick::datastructure::DataHeader>(data_header));

  std::cout << "DATA: scanNumber:  " << data.getDataHeaderPtr()->getScanNumber() << std::endl;

  sick::datastructure::DerivedValues derived_values = ParseDerivedValues::parseUDPSequence(buffer,data);
  data.setDerivedValuesPtr(boost::make_shared<sick::datastructure::DerivedValues>(derived_values));

  sick::datastructure::MeasurementData measurement_data = ParseMeasurementData::parseUDPSequence(buffer, data);
  data.setMeasurementDataPtr(boost::make_shared<sick::datastructure::MeasurementData>(measurement_data));



  return true;
}

}
}
