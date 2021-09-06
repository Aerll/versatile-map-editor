#include <ddnet/io/ddldata.hpp>

#include <ddnet/util/constants.hpp>

#include <iterator>

namespace ddnet::io {

qint32 DDLData::attributesCount(const DDLProperty& property) const {
    auto iter = properties.find(property);

    if (iter == std::cend(properties))
        return constants::_notfound;
    return iter->second.size();
}



qint32 DDLData::attributesCount(const std::pair<QString, QString>& property) const {
    return attributesCount({ .name = property.first, .value = property.second });
}



DDLData::const_iterator DDLData::property(const std::pair<QString, QString>& attribute) const {
    return properties.find({ .name = attribute.first, .value = attribute.second });
}



QByteArray DDLData::toQss() const {
    QString qss_buffer;
    qss_buffer.reserve(4096);
    for (const auto& property : properties) {
        // generate either:
        //   QWidget
        // or:
        //   ddnet--editor--Widget
        if (property.first.name == "Qt") {
            qss_buffer.push_back(property.first.value);
        }
        else if (property.first.name == "DDNet") {
            qss_buffer.push_back("ddnet--editor--" + property.first.value);
        }
        else {
            continue;
        }

        // generate:
        //   { property1: value1; ... }
        qss_buffer.push_back('{');
        for (const auto& attribute : property.second)
            qss_buffer.push_back(attribute.first + ':' + attribute.second + ';');
        qss_buffer.push_back('}');
    }
    return qss_buffer.toUtf8();
}

} // ddnet::io::
