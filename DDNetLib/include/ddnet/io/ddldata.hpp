#pragma once

#include <ddnet/util/utility.hpp>

#include <QtGlobal>
#include <QString>

#include <map>
#include <tuple>
#include <utility>

namespace ddnet::io {

class DDLProperty {
public:
    inline bool operator==(const DDLProperty& rhs) const = default;
    inline bool operator<(const DDLProperty& rhs) const noexcept { return std::tie(name, value) < std::tie(rhs.name, rhs.value); }

public:
    QString name;
    QString value;
};

class DDLData {
public:
    std::map<DDLProperty, std::map<QString, QString>> properties;

public:
    using type = decltype(properties);
    using value_type = type::value_type;
    using iterator = type::iterator;
    using const_iterator = type::const_iterator;

public:
    inline qint32 propertiesCount() const noexcept { return properties.size(); }
    qint32 attributesCount(const DDLProperty& property) const;
    qint32 attributesCount(const std::pair<QString, QString>& property) const;
    const_iterator property(const std::pair<QString, QString>& attribute) const;

    QByteArray toQss() const;

public:
    inline iterator begin() noexcept { return properties.begin(); }
    inline iterator end() noexcept { return properties.end(); }
    inline const_iterator begin() const noexcept { return properties.begin(); }
    inline const_iterator end() const noexcept { return properties.end(); }
    inline const_iterator cbegin() const noexcept { return properties.cbegin(); }
    inline const_iterator cend() const noexcept { return properties.cend(); }
};

} // ddnet::io::
